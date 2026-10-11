#include <atomic>
#include <memory>
namespace dss {

namespace detail {
struct alignas(64) hp_record {
  std::atomic<const void*> ptr{nullptr};
  std::atomic<bool> active{false};
  hp_record* next{nullptr};
};

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
