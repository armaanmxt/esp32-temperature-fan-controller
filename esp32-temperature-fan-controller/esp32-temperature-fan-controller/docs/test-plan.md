# Test plan

## Automated host tests

Run `make test` with a C++11-capable compiler. These tests exercise the actual
headers used by the firmware, without ESP32 dependencies:

- Startup state, heating and cooling, and repeated cycles.
- Strict threshold comparisons at exactly 28 C and 30 C.
- Retaining the previous output within the 2 C hysteresis band.
- A 500 ms sample interval, late polling, and 32-bit timer rollover.
- NTC conversion at known temperatures and invalid voltage inputs.

## Recorded host result

On October 18, 2026, `make test` completed successfully with GCC, using
`-std=c++11 -Wall -Wextra -Werror -pedantic`:

```text
PASS: 39 checks
```

## Manual Wokwi checks

These are steps to run, not recorded simulation results.

| Step | Input / action | Expected result |
| --- | --- | --- |
| 1 | Start at 25 C | LED off; serial line roughly every 500 ms |
| 2 | Raise to 31 C | LED on after the next sample |
| 3 | Lower to 29 C | LED stays on |
| 4 | Lower to 27 C | LED off after the next sample |
| 5 | Raise to 29 C | LED stays off |
| 6 | Repeat steps 2-4 | Same state sequence |

Check serial timestamps for the sampling interval. The loop polls elapsed time;
it does not use `delay()` or a hard real-time scheduler. Host tests do not prove
board timing, ESP32 compilation, or simulator wiring.
