#pragma once
#include "config.h"

class FanController {
 public:
  bool update(float temperature_c) {
    if (temperature_c > config::fan_on_c) {
      on_ = true;
    } else if (temperature_c < config::fan_off_c) {
      on_ = false;
    }
    return on_;
  }
  bool is_on() const { return on_; }

 private:
  bool on_ = false;
};
