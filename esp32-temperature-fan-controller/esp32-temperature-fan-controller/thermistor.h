#pragma once
#include <cmath>
#include "config.h"

inline float temperature_from_mv(float voltage_mv) {
  if (!std::isfinite(voltage_mv) || voltage_mv <= 0.0f ||
      voltage_mv >= config::supply_mv) {
    return NAN;
  }
  // Wokwi's NTC module: 10k thermistor and 10k series resistor.
  const float resistance_ratio = voltage_mv / (config::supply_mv - voltage_mv);
  const float inverse_kelvin = std::log(resistance_ratio) / config::thermistor_beta
                              + 1.0f / 298.15f;
  return 1.0f / inverse_kelvin - 273.15f;
}
