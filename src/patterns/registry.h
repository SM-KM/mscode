#pragma once

#include <concepts>
#include <functional>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <typeindex>
#include <unordered_map>
#include <utility>

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
  void set_fallback() {
      std::lock_guard lk{m_};
      fallbacks_[typeid(I)] = std::static_pointer_cast<void>(std::make_shared<Null>());
  }

  template<Service I>
  std::shared_ptr<I> try_get_shared() const  {
      std::lock_guard lk{m_};
      if(auto it = services_.find(typeid(I)); it !=services_.end())
	  return std::static_pointer_cast<I>(it->second);
      if(auto f = factories_.find(typeid(I)); f != factories_.end()) {
	  auto made = f->second();
	  services_[typeid(I)] = made;
	  factories_.erase(f);
	  return std::static_pointer_cast<I>(made);
      }

      if(auto it = fallbacks_.find(typeid(I)); it != fallbacks_.end())
	  return std::static_pointer_cast<I>(it->second);
      return nullptr;
  }

  // services_[typeid(I)] = std::static_pointer_cast<void>(std::move(impl));
  template <Service I> I *try_get() const { return try_get_shared<I>().get(); };
  template <Service I> I &get() const {
    if (I *p = try_get<I>()) return *p;
    throw std::runtime_error(std::string("service not registered: ") + typeid(I).name());
  }

  // clang-format on
  template <Service I> bool has() const {
    std::lock_guard lk{m_};
    return services_.contains(typeid(I)) || factories_.contains(typeid(I));
  };

  class [[nodiscard]] Override {
    ServiceLocator *loc_;
    Key key_;
    std::shared_ptr<void> previous_;

  public:
    Override(ServiceLocator *l, Key k, std::shared_ptr<void> prev)
        : loc_{l}, key_{k}, previous_{std::move(prev)} {}
    Override(const Override &) = delete;
    Override &operator=(const Override &) = delete;
    Override(Override &&o) noexcept
        : loc_{std::exchange(o.loc_, nullptr)}, key_{o.key_},
          previous_{std::move(o.previous_)} {}

    // clang-format off
    ~Override() {
      if (!loc_) return;
      std::lock_guard lk{loc_->m_};
      if(previous_) loc_->services_[key_] = std::move(previous_);
      else loc_->services_.erase(key_);
    }
  };
};
