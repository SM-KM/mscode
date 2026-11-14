#pragma once

// Adapter pattern

#include <concepts>
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
concept CelciusSensor = requires(const S &s) {
  { s.celsius() } -> std::convertible_to<double>;
};

template <typename S>
concept FahrenheitSensor = !CelciusSensor<S> && requires(const S &s) {
  { s.read_fahrenheit() } -> std::convertible_to<double>;
};
