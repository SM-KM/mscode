#include <atomic>
#include <memory>
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
  void (*reclaim)(retired_header){nullptr};
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

}; // namespace detail

template <typename T, typename D = std::default_delete<T>>
class hazard_pointer_obj_base {
 public:
  void retire(D d = D()) noexcept;

 protected:
  hazard_pointer_obj_base() = default;
  hazard_pointer_obj_base(const hazard_pointer_obj_base&) = default;
  hazard_pointer_obj_base(hazard_pointer_obj_base&&) = default;
  hazard_pointer_obj_base& operator=(const hazard_pointer_obj_base&) = default;
  hazard_pointer_obj_base& operator=(hazard_pointer_obj_base&&) = default;
  ~hazard_pointer_obj_base() = default;

 private:
  D deleter;
};

class hazard_pointer {
 public:
  hazard_pointer() noexcept;
  hazard_pointer(hazard_pointer&&) noexcept;
  hazard_pointer& operator=(const hazard_pointer&) noexcept;
  ~hazard_pointer();

  bool empty() const noexcept;
  template <typename T>
  T* protect(const std::atomic<T*>& src) noexcept;
  template <class T>
  bool try_protect(T*& ptr, const std::atomic<T*>& src) noexcept;
  template <class T>
  void reset_protection(const T* ptr) noexcept;
  void reset_protection(std::nullptr_t = nullptr) noexcept;
  void swap(hazard_pointer&) noexcept;
};

hazard_pointer make_hazard_pointer();
void swap(hazard_pointer&, hazard_pointer&) noexcept;

}; // namespace dss
