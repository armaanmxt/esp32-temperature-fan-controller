# ESP32 Temperature Fan Controller

C++ firmware for a small temperature-based fan controller. An ESP32 samples an
NTC temperature sensor every **500 ms**, switches a fan command using **2 C
hysteresis**, and prints the temperature and output state over serial. A green
LED represents the fan in Wokwi.

## What it does

| Calculated temperature | Output |
| --- | --- |
| Above 30 C | Fan ON |
| Below 28 C | Fan OFF |
| 28 C through 30 C, including both endpoints | Keep the previous state |

The fan starts OFF. Separate thresholds keep it from repeatedly switching when
the temperature moves around one threshold. Sampling uses elapsed `millis()`
time instead of a blocking delay.

## Try it

Open a new [Wokwi ESP32 project](https://wokwi.com/projects/new/esp32), then add
`sketch.ino`, `diagram.json`, and the four `.h` files from this repository.
The [setup guide](docs/wokwi.md) has the wiring and editor steps.

Start the simulation and set the NTC sensor to **25 -> 31 -> 29 -> 27 C**.
The LED should show **OFF -> ON -> ON -> OFF**. Serial output uses **115200 baud**.

Example output format (illustrative, not a recorded simulator run):

```text
t=500 ms | temp=25.00 C | fan=OFF
t=1000 ms | temp=31.00 C | fan=ON
t=1500 ms | temp=29.00 C | fan=ON
t=2000 ms | temp=27.00 C | fan=OFF
```

## Code layout

| File | Purpose |
| --- | --- |
| `sketch.ino` | ESP32 setup, sensor polling, output pin, serial messages |
| `config.h` | Pins, thresholds, sample interval, sensor constants |
| `fan_controller.h` | Two-state hysteresis controller |
| `sample_timer.h` | Elapsed-time sampling with timer rollover handling |
| `thermistor.h` | NTC voltage-to-temperature conversion |
| `diagram.json` | Wokwi ESP32, NTC sensor, resistor, and LED wiring |
| `tests/test_controller.cpp` | Host tests for the shared firmware logic |
| `docs/` | Simulator setup and manual test procedure |

## Run the tests

Requires GNU Make and a C++11 compiler (GCC or Clang).

```sh
make test
```

**Latest local result: 39 checks passed.**

The tests cover threshold boundaries, hysteresis, repeated cycles, sampling,
timer rollover, and thermistor conversion. GitHub Actions runs the same command.
See the [test plan](docs/test-plan.md) for the simulator checks.

## Scope and validation

This is a small firmware learning project. There is no physical fan, PWM speed
control, RTOS, networking, or closed-loop motor control. The LED represents an
on/off command. Host tests can validate the shared control and conversion logic;
they do not replace compiling and running the sketch on an ESP32 simulator.

The Wokwi run and physical hardware checks are still pending. The setup and test
plan explain how to complete the simulator demonstration.
