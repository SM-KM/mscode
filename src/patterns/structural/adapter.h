#pragma once

// Adapter pattern

#include <algorithm>
#include <concepts>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>
struct ITempSensor {
  virtual ~ITempSensor() = default;
  virtual double celsius() const = 0;
};

struct LegacyThermometer {
  explicit LegacyThermometer(double f = 98.6) : f_{f} {}
  double read_fahrenheit() const { return f_; }

private:
  double f_;
};

constexpr double f_to_c(double f) { return (f - 32) * 5.0 / 9.0; };

/// Object adapter, hloding the adaptee
class FarenheitAdapter final : public ITempSensor {
  LegacyThermometer legacy_;

public:
  explicit FarenheitAdapter(LegacyThermometer l) : legacy_{l} {}
  double celsius() const override { return f_to_c(legacy_.read_fahrenheit()); };
};

/// Class adaptor
class PrivateAdaptor final : public ITempSensor, private LegacyThermometer {
public:
  using LegacyThermometer::LegacyThermometer;
  double celsius() const override { return f_to_c(read_fahrenheit()); };
};

/// Static adapter with concepts
template <typename S>
concept CelsiusSensor = requires(const S &s) {
  { s.celsius() } -> std::convertible_to<double>;
};

template <typename S>
concept FahrenheitSensor = !CelsiusSensor<S> && requires(const S &s) {
  { s.read_fahrenheit() } -> std::convertible_to<double>;
};

template <FahrenheitSensor S> class StaticFahrenheitSensor {
  S s_;

public:
  explicit StaticFahrenheitSensor(S s) : s_{std::move(s)} {}
  double celcius() const { return f_to_c(s_.read_fahrenheit()); };
};

// Adapt a sensor of any supported kind to the celcius concept
// clang-format off
template <typename S> requires CelsiusSensor<S> || FahrenheitSensor<S>
auto as_celsius(S s) {
    if constexpr (CelsiusSensor<S>) return s;
    else return StaticFahrenheitSensor<S>{std::move(s)};
}

// Adapter for plain callable
template <std::invocable F> requires std::convertible_to<std::invoke_result<F>, double>
class KelvinCallableAdapter {
  F f_;

public:
  explicit KelvinCallableAdapter(F f) : f_{std::move(f)} {}
  double celsius() const { return static_cast<double>(std::invoke(f_)) - 273.16; }
};

struct NativeSensor { double celsius() const { return 21.5; } };
// clang-format on

// Works with any mix of sensors -> resolved at compile time, zero virtual calls
double average_celsius(const CelsiusSensor auto &...sensors) {
  return (sensors.celsius() + ...) / static_cast<double>(sizeof...(sensors));
}

// Bridge from static to dynamic
template <CelsiusSensor S> class AnySensor final : public ITempSensor {
  S s_;

public:
  explicit AnySensor(S s) : s_{std::move(s)} {}
  double celsius() const override { return s_.celsius(); }
};

template <CelsiusSensor S> std::unique_ptr<ITempSensor> erase(S s) {
  return std::make_unique<AnySensor<S>>(std::move(s));
}
