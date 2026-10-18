#pragma once
#include <cstdint>
#include "config.h"

class SampleTimer {
 public:
  explicit SampleTimer(std::uint32_t start_ms = 0) : last_ms_(start_ms) {}
  bool ready(std::uint32_t now_ms) {
    // Unsigned subtraction also handles the millis() counter wrapping.
    if (static_cast<std::uint32_t>(now_ms - last_ms_) < config::sample_interval_ms) {
      return false;
    }
    last_ms_ = now_ms;
    return true;
  }

 private:
  std::uint32_t last_ms_;
};
