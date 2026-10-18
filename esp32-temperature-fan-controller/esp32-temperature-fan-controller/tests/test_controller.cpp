#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include "fan_controller.h"
#include "sample_timer.h"
#include "thermistor.h"

int checks = 0;
void check(bool condition, const char* message) {
  ++checks;
  if (!condition) {
    std::cerr << "FAIL: " << message << '\n';
    std::exit(1);
  }
}

int main() {
  FanController fan;
  check(!fan.is_on(), "starts off");
  check(!fan.update(25.0f), "room temperature stays off");
  check(!fan.update(28.0f), "lower threshold stays off");
  check(!fan.update(29.0f), "deadband stays off");
  check(!fan.update(30.0f), "exactly 30 stays off");
  check(fan.update(30.01f), "above 30 turns on");
  check(fan.update(35.0f), "high temperature stays on");
  check(fan.update(30.0f), "exactly 30 stays on");
  check(fan.update(29.0f), "deadband stays on");
  check(fan.update(28.0f), "exactly 28 stays on");
  check(!fan.update(27.99f), "below 28 turns off");
  check(!fan.update(29.9f), "warming within deadband stays off");
  check(fan.update(31.0f), "second heating cycle turns on");
  for (float value : {29.9f, 30.0f, 29.8f, 30.0f, 28.1f}) {
    check(fan.update(value), "noise inside deadband does not switch off");
  }
  check(!fan.update(27.0f), "second cooling cycle turns off");

  SampleTimer timer;
  check(!timer.ready(0), "no sample at startup");
  check(!timer.ready(499), "no early sample");
  check(timer.ready(500), "sample at 500ms");
  check(!timer.ready(500), "no duplicate sample");
  check(!timer.ready(999), "wait for next interval");
  check(timer.ready(1000), "sample at 1000ms");
  check(timer.ready(1750), "late poll samples once");
  check(!timer.ready(1751), "no catch-up burst");
  const auto max_time = std::numeric_limits<std::uint32_t>::max();
  SampleTimer wrapping(max_time - 249u);
  check(!wrapping.ready(249u), "499ms across wrap is early");
  check(wrapping.ready(250u), "500ms across wrap is due");

  check(std::fabs(temperature_from_mv(1650.0f) - 25.0f) < 0.01f,
        "half supply is 25C");
  check(std::isnan(temperature_from_mv(0.0f)), "zero voltage rejected");
  check(std::isnan(temperature_from_mv(3300.0f)), "supply voltage rejected");
  check(std::isnan(temperature_from_mv(-1.0f)), "negative voltage rejected");
  check(std::isnan(temperature_from_mv(NAN)), "NaN rejected");
  check(std::isnan(temperature_from_mv(INFINITY)), "infinite voltage rejected");
  // Independently generate divider voltages from the NTC resistance equation.
  for (float temperature : {20.0f, 28.0f, 30.0f, 40.0f}) {
    const float ratio = std::exp(3950.0f * (1.0f / (temperature + 273.15f)
                                          - 1.0f / 298.15f));
    const float voltage = 3300.0f * ratio / (1.0f + ratio);
    check(std::fabs(temperature_from_mv(voltage) - temperature) < 0.01f,
          "temperature conversion matches reference");
  }
  std::cout << "PASS: " << checks << " checks\n";
}
