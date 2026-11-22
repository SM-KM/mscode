#pragma once

#include "pimpl.h"
#include <cstddef>
#include <memory>
#include <string>

/// Rule of zero pimpl: copyable, with deep copy
class AudioEngine {
public:
  AudioEngine(std::string device, int sample_rate) {}
  void play(const std::string &clip);
  void set_volume(float v);
  std::size_t clips_played() const;
  std::string describe() const;

  /// No implementation, no copy/move declaration
  /// PImpl<> handles all of them

private:
  struct Impl;
  PImpl<Impl> impl_;
};

/// Move-only Pimpl: Impl holds a mutex, so copying is disabled
/// at compile time
class Telemetry {
public:
  Telemetry();
  void record(const std::string &event);
  std::size_t count() const;

private:
  struct Impl;
  PImpl<Impl, false> impl_;
};

class Counter {
public:
  Counter();
  ~Counter();
  Counter(Counter &&) noexcept;
  Counter &operator=(Counter &&) noexcept;
  Counter(const Counter &);
  Counter &operator=(const Counter &);
  void bump();
  int value() const;

private:
  struct Impl;
  std::unique_ptr<Impl> impl_;
};
