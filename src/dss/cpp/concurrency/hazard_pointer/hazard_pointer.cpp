#include <algorithm>
#include <atomic>
#include <memory>
#include <utility>
namespace dss {

namespace detail {
struct alignas(64) hp_record {
  std::atomic<const void*> ptr{nullptr};
  std::atomic<bool> active{false};
  hp_record* next{nullptr};
};

/// what an object needs to be in a retired list
struct retired_header {
  retired_header() noexcept = default;
  retired_header(const retired_header&) noexcept {};
  retired_header& operator=(const retired_header&) noexcept { return *this; };

  const void* object{nullptr};
  void (*reclaim)(retired_header*){nullptr};
  retired_header* next{nullptr};
};

struct domain {
  std::atomic<hp_record*> records{nullptr};
  std::atomic<std::size_t> record_count{0};
  std::atomic<retired_header*> orphans{nullptr};
};

/// constant initialized and trivially destructible, so no init order problems
/// and nothing is destroyed at exit under the feet of a late thread
inline domain& global_domain() noexcept {
  static domain d;
  return d;
}

inline hp_record* acquire_record() {
  domain& d = global_domain();

  for (hp_record* r = d.records.load(std::memory_order_acquire); r;
       r = r->next) {
    bool expected = false;
    if (!r->active.load(std::memory_order_relaxed) &&
        r->active.compare_exchange_strong(expected, true,
                                          std::memory_order_acquire))
      ;
  }

  // if there is none free make a new one and add it to the list
  auto r = new hp_record;
  r->active.store(true, std::memory_order_relaxed);
  r->next = d.records.load(std::memory_order_relaxed);
  while (!d.records.compare_exchange_weak(r->next, r, std::memory_order_release,
                                          std::memory_order_relaxed)) {
  }
  d.record_count.fetch_add(1, std::memory_order_relaxed);
  return r;
}

inline void release_record(hp_record* r) noexcept {
  r->ptr.store(nullptr, std::memory_order_release);
  r->active.store(false, std::memory_order_release);
}

inline bool is_protected(const void* p) noexcept {
  for (hp_record* r = global_domain().records.load(std::memory_order_acquire);
       r; r = r->next)
    if (r->ptr.load(std::memory_order_acquire) == p) return true;
  return false;
}

struct retire_list {
  retired_header* head{nullptr};
  std::size_t count{0};
  bool scanning{false};

  void push(retired_header* n) noexcept {
    n->next = head;
    head = n;
    ++count;
  }

  // scan when we have about twice as many retired objects as hazards, then
  // at most half survive, so the cost of a scan is amortised over the retires
  std::size_t threshold() const noexcept {
    return std::max<std::size_t>(
        16, 2 * global_domain().record_count.load(std::memory_order_relaxed));
  }

  void scan() noexcept {
    scanning = true;
    retired_header* o =
        global_domain().orphans.exchange(nullptr, std::memory_order_acquire);
    while (o) {
      retired_header* nx = o->next;
      push(o);
      o = nx;
    }

    // pairs with the fence in try_protect: either the protecting thread sees
    // that the object was already removed (and will not use it), or we see
    // its hazard here. This is what makes the whole scheme correct
    std::atomic_thread_fence(std::memory_order_seq_cst);
    retired_header* list = std::exchange(head, nullptr);
    count = 0;
    retired_header* keep = nullptr;

    while (list) {
      retired_header* n = list;
      list = n->next;
      if (is_protected(n->object)) {
        n->next = keep;
        keep = n;
      } else {
        n->reclaim(n);
      }
    }

    while (keep) {
      retired_header* nx = keep->next;
      push(keep);
      keep = nx;
    }
    scanning = false;
  };

  void retire(retired_header* n) noexcept {
    push(n);
    if (!scanning && count >= threshold()) scan();
  }

  ~retire_list() {
    if (!head) return;
    scan();
    if (!head) return;
    retired_header* tail = head;
    while (tail->next) tail = tail->next;
    domain& d = global_domain();
    tail->next = d.orphans.load(std::memory_order_relaxed);

    while (d.orphans.compare_exchange_weak(tail->next, head,
                                           std::memory_order_release,
                                           std::memory_order::relaxed)) {
    }
    head = nullptr;
    count = 0;
  };
};

inline retire_list& local_retire_list() noexcept {
  thread_local retire_list list;
  return list;
}

}; // namespace detail

template <typename T, typename D = std::default_delete<T>>
class hazard_pointer_obj_base : private detail::retired_header {
 public:
  void retire(D d = D()) noexcept {
    deleter = std::move(d);
    T* obj = static_cast<T*>(this);
    object = static_cast<const void*>(obj);
    reclaim = &reclaim_fn;
    detail::local_retire_list().retire(this);
  };

 protected:
  hazard_pointer_obj_base() = default;
  hazard_pointer_obj_base(const hazard_pointer_obj_base&) = default;
  hazard_pointer_obj_base(hazard_pointer_obj_base&&) = default;
  hazard_pointer_obj_base& operator=(const hazard_pointer_obj_base&) = default;
  hazard_pointer_obj_base& operator=(hazard_pointer_obj_base&&) = default;
  ~hazard_pointer_obj_base() = default;

 private:
  static void reclaim_fn(detail::retired_header* h) {
    auto* self = static_cast<hazard_pointer_obj_base*>(h);
    T* obj = static_cast<T*>(self);
    D d = std::move(self->deleter);
    d(obj);
  }
  D deleter;
};

class hazard_pointer {
 public:
  hazard_pointer() noexcept = default;
  hazard_pointer(hazard_pointer&& other) noexcept
      : m_rec{std::exchange(other.m_rec, nullptr)} {};
  hazard_pointer& operator=(hazard_pointer&& other) noexcept {
    if (this != &other) {
      if (m_rec) detail::release_record(m_rec);
      m_rec = std::exchange(other.m_rec, nullptr);
    }
    return *this;
  };
  ~hazard_pointer() {
    if (m_rec) detail::release_record(m_rec);
  };

  bool empty() const noexcept { return m_rec == nullptr; };

  /// loads src and protects the value, returns the pointer that is now safe
  /// to dereference, it must not ve empty
  template <typename T>
  T* protect(const std::atomic<T*>& src) noexcept {
    T* p = src.load(std::memory_order_relaxed);
    while (!try_protect(p, src)) {
    }
    return p;
  };

  /// publish ptr as hazard, then check src still holds it. if not
  /// ptr is updated with the new value and false is returned
  template <class T>
  bool try_protect(T*& ptr, const std::atomic<T*>& src) noexcept {
    T* p = ptr;
    reset_protection(p);

    // hazard_pointer has to be visible before we re-read src
    // store -> load ordering, a plain release/acquire is not enough
    // for this
    std::atomic_thread_fence(std::memory_order_seq_cst);
    ptr = src.load(std::memory_order_acquire);
    if (p == ptr) return true;
    reset_protection();
    return false;
  };

  template <class T>
  void reset_protection(const T* ptr) noexcept {
    m_rec->ptr.store(static_cast<const void*>(ptr), std::memory_order_release);
  };
  void reset_protection(std::nullptr_t = nullptr) noexcept {
    m_rec->ptr.store(nullptr, std::memory_order_release);
  };
  void swap(hazard_pointer& other) noexcept { std::swap(m_rec, other.m_rec); };

 private:
  friend hazard_pointer make_hazard_pointer();
  explicit hazard_pointer(detail::hp_record* r) noexcept : m_rec{r} {};
  detail::hp_record* m_rec{nullptr};
};

inline hazard_pointer make_hazard_pointer() {
  return hazard_pointer(detail::acquire_record());
}
inline void swap(hazard_pointer& lhs, hazard_pointer& rhs) noexcept {
  lhs.swap(rhs);
};

/// scan the retired objects of the calling thread right now. Handy for
/// tests and for shutdown, normally the threshold in retire() decides
/// when to scan
inline void reclaim_retired() noexcept {
  detail::local_retire_list().scan();
};

}; // namespace dss
