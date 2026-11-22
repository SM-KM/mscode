#pragma once

#include <concepts>
#include <memory>
#include <utility>

template <typename T, bool Copyable = true> class PImpl {
  std::unique_ptr<T, void (*)(T *)> p_{nullptr, nullptr};
  T *(*clone_)(const T *) = nullptr;

public:
  PImpl() = default;

  /// Call this from the, where T is a complete type.
  // clang-format off
  template <typename... Args> requires std::constructible_from<T, Args...>
  static PImpl make(Args &&...args) {
    static_assert(!Copyable || std::copy_constructible<T>);
    PImpl p;
    p.p_ = std::unique_ptr<T, void (*)(T *)>{new T{std::forward<Args>(args)...},
                                             [](T *t) { delete t; }};

    if constexpr (Copyable) p.clone_ = [](const T* t) { return new T(*t); };
    return p;
  };

  PImpl(PImpl&&) noexcept = default;
  PImpl& operator=(PImpl&&) noexcept = default;
  PImpl(const PImpl&) requires(!Copyable) = delete;
  PImpl& operator=(const PImpl&) requires(!Copyable) = delete;

  PImpl(const PImpl& o) requires(Copyable) 
      : p_{o.p_ ? std::unique_ptr<T, void (*)(T*)>{o.clone_(o.p_.get()), o.p_.get_deleter()}
		: std::unique_ptr<T, void (*)(T*)>{nullptr, nullptr}},
      clone_{o.clone_} {}

  PImpl& operator=(const PImpl& o) requires(Copyable) {
      PImpl tmp{o}; std::swap(p_, tmp.p_);
      std::swap(clone_, tmp.clone_); return *this;
  }

  T* operator->() const { return p_.get(); }
  T& operator*() const { return *p_; }
  explicit operator bool() const { return static_cast<bool>(p_); }
};





