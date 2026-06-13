# SEN0217 Water Flow Sensor Test

Arduino sketch for the **DFRobot SEN0217** (YF-S201) 1/2" water flow sensor.

## Wiring (Arduino Uno)

| Sensor Wire | Arduino |
|-------------|---------|
| Green (Signal) | D2 (INT0) |
| Red (VCC)      | 5V       |
| Black (GND)    | GND      |

## Dependencies

None — no external library required.

## Sensor specs

| Parameter | Value |
|-----------|-------|
| Flow range | 1–30 L/min |
| Accuracy  | ±5% (2–30 L/min) |
| Pulse rate | 450 pulses per litre |

## What it does

1. Counts rising-edge pulses from the sensor on D2 using a hardware interrupt.
2. Every second, snapshots and resets the counter atomically (interrupts disabled briefly).
3. Calculates:
   - **Flow rate** (L/min) = `pulses_per_sec / 450 × 60`
   - **Total volume** (L) = cumulative `pulses / 450`
4. Prints all values to Serial at 9600 baud.

## Serial output example

```
=== SEN0217 Water Flow Sensor Test ===
Waiting for flow...

Flow Rate:    2.400 L/min
Pulse Count:  18
Total Volume: 0.0400 L
```

## Notes

- `totalLitres` accumulates until the Arduino is reset or power-cycled.
- The interrupt-safe snapshot (`noInterrupts()` / `interrupts()`) prevents missed or double-counted pulses.
- D2 is the only INT0-capable pin on the Uno; use D3 (INT1) as an alternative and update `FLOW_PIN` and `attachInterrupt` accordingly.
