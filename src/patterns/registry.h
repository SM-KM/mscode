#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>
#include <typeindex>
#include <unordered_map>

// Service locator / type-keyed registry with concepts

/// A service is lookup by its interface abstract with virtual destructor
template <typename T>
concept Service = std::is_abstract_v<T> && std::has_virtual_destructor_v<T>;

template <typename Null, typename I>
concept NullServiceFor =
    std::derived_from<Null, I> && std::default_initializable<Null> &&
    !std::is_abstract_v<Null>;

class ServiceLocator {
  // clang-format off
  using Key = std::type_index;
  mutable std::unordered_map<Key, std::shared_ptr<void>> services_;
  mutable std::unordered_map<Key, std::function<std::shared_ptr<void>()>> factories_;
  std::unordered_map<Key, std::shared_ptr<void>> fallbacks_;
  mutable std::recursive_mutex m_;
  // clang-format on

public:
  ServiceLocator() = default;
  ServiceLocator(const ServiceLocator &) = delete;
  ServiceLocator &operator=(const ServiceLocator &) = delete;

  // clang-format off
  template <Service I> void provide_lazy(std::function<std::shared_ptr<I>()> make) {
    std::lock_guard lk{m_};
    services_.erase(typeid(I));
    factories_[typeid(I)] = [make = std::move(make)] { return std::static_pointer_cast<void>(make()); };
  }

  template<Service I, typename Null> requires NullServiceFor<Null, I>
  void set_fallback();

  // services_[typeid(I)] = std::static_pointer_cast<void>(std::move(impl));
  // clang-format on
};
