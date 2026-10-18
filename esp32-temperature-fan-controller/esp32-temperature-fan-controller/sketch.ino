#include <Arduino.h>
#include "config.h"
#include "fan_controller.h"
#include "sample_timer.h"
#include "thermistor.h"

FanController fan;
SampleTimer sample_timer;

void setup() {
  Serial.begin(config::serial_baud);
  pinMode(config::fan_pin, OUTPUT);
  digitalWrite(config::fan_pin, LOW);
  pinMode(config::temperature_pin, INPUT);
  analogReadResolution(12);
  analogSetPinAttenuation(config::temperature_pin, ADC_11db);
  Serial.println("Temperature controller: sample=500ms, on>30C, off<28C");
}

void loop() {
  const std::uint32_t now_ms = millis();
  if (!sample_timer.ready(now_ms)) {
    return;
  }

  const float voltage_mv = analogReadMilliVolts(config::temperature_pin);
  const float temperature_c = temperature_from_mv(voltage_mv);
  if (!std::isfinite(temperature_c)) {
    Serial.printf("t=%lu ms | invalid sensor reading | fan=%s (unchanged)\n",
                  static_cast<unsigned long>(now_ms), fan.is_on() ? "ON" : "OFF");
    return;
  }

  const bool fan_on = fan.update(temperature_c);
  digitalWrite(config::fan_pin, fan_on ? HIGH : LOW);
  Serial.printf("t=%lu ms | temp=%.2f C | fan=%s\n",
                static_cast<unsigned long>(now_ms), temperature_c,
                fan_on ? "ON" : "OFF");
}
