#pragma once
#include <cstdint>

namespace config {
constexpr std::uint8_t temperature_pin = 34;
constexpr std::uint8_t fan_pin = 23;
constexpr std::uint32_t sample_interval_ms = 500;
constexpr float fan_on_c = 30.0f;
constexpr float fan_off_c = 28.0f;
constexpr float supply_mv = 3300.0f;
constexpr float thermistor_beta = 3950.0f;
constexpr std::uint32_t serial_baud = 115200;
}
