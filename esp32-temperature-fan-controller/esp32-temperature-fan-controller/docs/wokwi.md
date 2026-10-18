# Run it in Wokwi

1. Open https://wokwi.com/projects/new/esp32.
2. Replace the starter `sketch.ino` and `diagram.json` with the files here.
3. Use the editor's file menu to add `config.h`, `fan_controller.h`,
   `sample_timer.h`, and `thermistor.h` beside the sketch.
4. Start the simulation. The serial monitor uses 115200 baud.
5. Click the NTC sensor and change its temperature. Try 25, 31, 29, then 27 C.
   The expected fan states are OFF, ON, ON, OFF. The green LED is the fan output.

No extra Arduino libraries are needed. The sketch uses the ESP32 Arduino core.

## Connections

| Component pin | ESP32 / connection |
| --- | --- |
| NTC VCC | 3V3 |
| NTC GND | GND |
| NTC OUT | GPIO 34 (ADC1) |
| GPIO 23 | 220-ohm resistor, then LED anode |
| LED cathode | GND |

The module uses a 10k NTC, a 10k series resistor, and beta=3950.
The firmware reads calibrated millivolts and assumes the module supply is 3.3V.
It uses the beta equation to estimate temperature. This is an educational model,
not a calibrated physical temperature instrument.

The controller compares the calculated reading to the thresholds. ADC resolution
and rounding mean a simulator slider value exactly at a threshold may appear
slightly above or below it. Use the host tests to check exact threshold equality;
use 31 C and 27 C for clear simulator transitions.

A real fan is outside this project's scope: the LED only represents its on/off
command. The physical ESP32, sensor, and motor driver have not been validated.

## References

- https://docs.wokwi.com/parts/wokwi-ntc-temperature-sensor
- https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html
