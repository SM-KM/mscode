#ifndef ANY_H
#define ANY_H

#include <any>
#include <initializer_list>
#include <new>
#include <type_traits>
#include <typeinfo>
#include <utility>

namespace dss {
class any;

namespace detail {
union any_storage {
  void* ptr;
  alignas(void*) unsigned char buf[sizeof(void*) * 3];
};

// Type erasure -> any does not know the type it holds, it knows a "vtable", a
// table of function pointers that were instantiated for that type (one
// table per type, shared by all the anys holding it):
//   type()    -> typeid of the value
//   get()     -> address of the value
//   destroy() -> destructor
//   copy()    -> copy construct into another storage
//   move()    -> move into another storage and destroy the source
struct any_vtable {
  const std::type_info& (*type)() noexcept;
  void* (*get)(const any_storage&) noexcept;
  void (*destroy)(any_storage&) noexcept;
  void (*copy)(const any_storage& src, any_storage& dst);
  void (*move)(any_storage& src, any_storage& dst) noexcept;
};

template <typename T>
inline constexpr bool any_is_small =
    sizeof(T) <= sizeof(any_storage) && alignof(T) <= alignof(any_storage) &&
    std::is_nothrow_move_constructible_v<T>;

template <typename T, bool Small = any_is_small<T>>
struct any_manager;

/// The value is inside the any instance
template <typename T>
struct any_manager<T, true> {
  static T* ptr(const any_storage& s) noexcept {
    return std::launder(
        reinterpret_cast<T*>(const_cast<unsigned char*>(s.buf)));
  };

  template <typename... Args>
  static T& create(any_storage& s, Args&&... args) {
    return *::new (static_cast<void*>(s.buf)) T(std::forward<Args>(args)...);
  }
  static const std::type_info& type() noexcept { return typeid(T); };
  static void* get(const any_storage& s) noexcept { return ptr(s); };
  static void destroy(any_storage& s) noexcept { ptr(s)->T(); };
  static void copy(const any_storage& src, any_storage& dst) {
    ::new (static_cast<void*>(dst.buf)) T(*ptr(src));
  }
  static void move(any_storage& src, any_storage& dst) noexcept {
    ::new (static_cast<void*>(dst.buf)) T(std::move(*ptr(src)));
    ptr(src)->T();
  }
  static const any_vtable* vtable() noexcept {
    static constexpr any_vtable vt{&type, &get, &destroy, &copy, &move};
    return &vt;
  }
};

template <typename T>
struct any_manager<T, false> {
  static T* ptr(const any_storage& s) noexcept {
    return static_cast<T*>(s.ptr);
  }
  template <typename... Args>
  static T& create(any_storage& s, Args&&... args) {
    s.ptr = new T(std::forward<Args>(args)...);
    return *ptr(s);
  }
  static const std::type_info& type() noexcept { return typeid(T); }
  static void* get(const any_storage& s) noexcept { return s.ptr; }
  static void destroy(any_storage& s) noexcept { delete ptr(s); }
  static void copy(const any_storage& src, any_storage& dst) {
    dst.ptr = new T(*ptr(src));
  }
  static void move(any_storage& src, any_storage& dst) noexcept {
    dst.ptr = src.ptr; // just the pointer changes owner
    src.ptr = nullptr;
  }
  static const any_vtable* vtable() noexcept {
    static constexpr any_vtable vt{&type, &get, &destroy, &copy, &move};
    return &vt;
  }
};

template <typename T>
struct is_in_place_type : std::false_type {};
template <typename T>
struct is_in_place_type<std::in_place_type_t<T>> : std::true_type {};

/// what converting constructor / assignment
template <typename ValueType>
inline constexpr bool any_accepts =
    !std::is_same_v<std::decay_t<ValueType>, any> &&
    !is_in_place_type<std::decay_t<ValueType>>::value &&
    std::is_copy_constructible_v<std::decay_t<ValueType>> &&
    std::is_constructible_v<std::decay_t<ValueType>, ValueType>;
struct any_access;

}; // namespace detail

// Any is not a templated class is an implementation from C++17,
// that allows to hold any time
class any {
  constexpr any() noexcept {};
  any(const any& other) {
    if (other.m_vtable) {
      other.m_vtable->copy(other.m_storage, m_storage);
      m_vtable = other.m_vtable;
    }
  };
  any(any&& other) noexcept { take(other); };
  ~any() { reset(); };

  template <typename ValueType>
    requires detail::any_accepts<ValueType>
  any(ValueType&& value) {
    emplace<std::decay_t<ValueType>>(std::forward<ValueType>(value));
  };

  template <typename ValueType, class... Args>
  explicit any(std::in_place_type_t<ValueType>, Args&&... args);

  template <typename ValueType, typename U, typename... Args>
  explicit any(std::in_place_type_t<ValueType>, std::initializer_list<U> il,
               Args&&... args);

  any& operator=(const any& rhs);
  any& operator=(any&& rhs) noexcept;
  template <typename ValueType>
  any& operator=(ValueType&& rhs);

  // Most operations on std::any decays the valuetype
  template <typename ValueType, typename... Args>
  std::decay_t<ValueType>& emplace(Args&&... args);
  template <typename ValueType, typename U, typename... Args>
  std::decay_t<ValueType>& emplace(std::initializer_list<U> il, Args&&... args);

  // Destroy the contained object
  void reset() noexcept;

  void swap(any& other) noexcept;
  bool has_value() const noexcept;

  // std::type_info describes the information of the type stored if any
  const std::type_info& type() const noexcept;

 private:
  friend struct detail::any_access;
  void take(any& src) noexcept {
    if (src.m_vtable) {
      src.m_vtable->move(src.m_storage, m_storage);
      m_vtable = src.m_vtable;
      src.m_vtable = nullptr;
    }
  }

  const detail::any_vtable* m_vtable{nullptr};
  detail::any_storage m_storage{};
};

// bad_any_cast
class bad_any_cast : std::bad_cast {
  bad_any_cast() noexcept;
  bad_any_cast(const bad_any_cast& other) noexcept;
};
// any_cast
template <typename T>
T any_cast(const any& operand);

template <typename T>
T any_cast(any& operand);

template <typename T>
T any_cast(any&& operand);

template <typename T>
const T* any_cast(const any* operand) noexcept;

template <typename T>
T* any_cast(any* operand) noexcept;

// make_any
template <typename T, typename... Args>
std::any make_any(Args... args);

template <typename T, typename U, typename... Args>
std::any make_any(std::initializer_list<U> il, Args... args);

} // namespace dss

#endif // ANY_H
