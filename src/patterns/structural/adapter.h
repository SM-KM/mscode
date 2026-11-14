#pragma once

// Adapter pattern

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
