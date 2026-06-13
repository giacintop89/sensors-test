# INA226 Power Monitor Test

Arduino sketch for the **Texas Instruments INA226** 36V / 16-bit I2C power monitor.

## Wiring (Arduino Uno)

| INA226 Pin | Arduino / Circuit |
|------------|-------------------|
| SDA        | A4                |
| SCL        | A5                |
| VS         | 3.3V or 5V        |
| GND        | GND               |
| A0         | GND               |
| A1         | GND → I2C address **0x40** |
| IN+        | High side of shunt (supply side) |
| IN−        | Low side of shunt (load side)    |
| VBUS       | Same node as IN−  |

> Place a **0.1 Ω** shunt resistor between IN+ and IN−.  
> Maximum measurable current ≈ **819 mA** (±81.92 mV shunt range).

## Dependencies

`Wire` (built-in, no install needed).

## Configuration

| Parameter     | Value |
|---------------|-------|
| Averaging     | 1 sample |
| VBUS conv. time | 1.1 ms |
| VSHUNT conv. time | 1.1 ms |
| Mode          | Shunt + bus, continuous |
| Shunt resistor | 0.1 Ω |
| Current LSB   | 100 µA |
| Calibration register | 512 |

## What it does

1. Resets the INA226, writes the configuration and calibration registers.
2. Every second reads bus voltage, shunt voltage, current, and power from the on-chip registers.
3. Prints all four values to Serial at 9600 baud.

## Serial output example

```
=== INA226 Power Monitor Test ===
Shunt resistor: 100 mΩ
Current LSB:    100 µA

Bus Voltage:   5.0000 V
Shunt Voltage: 10.5000 mV
Current:       105.000 mA
Power:         525.000 mW
```

## Notes

- Change `A0`/`A1` pin connections to select a different I2C address (four options: 0x40–0x43).
- Adjust `R_SHUNT`, `CURRENT_LSB`, and `CALIB_VALUE` together if you use a different shunt resistor.
