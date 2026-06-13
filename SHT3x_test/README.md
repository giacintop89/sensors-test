# SHT3x-DIS Temperature & Humidity Sensor Test

Arduino sketch for the **Sensirion SHT30 / SHT31 / SHT35** I2C humidity and temperature sensor.

## Wiring (Arduino Uno)

| SHT3x Pin | Arduino |
|-----------|---------|
| SDA       | A4      |
| SCL       | A5      |
| VDD       | 3.3V    |
| GND       | GND     |
| ADDR      | GND → I2C address **0x44** |
|           | 3.3V → I2C address **0x45** |

## Dependencies

`Wire` (built-in, no install needed).

## What it does

1. Issues a soft-reset to the sensor on startup.
2. Every second triggers a **single-shot, high-repeatability** measurement with clock stretching enabled.
3. Reads 6 bytes: `[T_MSB, T_LSB, T_CRC, RH_MSB, RH_LSB, RH_CRC]`.
4. Validates both CRC-8 checksums (polynomial `0x31`, init `0xFF`).
5. Converts raw values using the datasheet formulas:
   - **Temperature (°C)** = −45 + 175 × raw_T / 65535
   - **Relative Humidity (%RH)** = 100 × raw_RH / 65535
6. Prints temperature and humidity to Serial at 9600 baud.

## Serial output example

```
=== SHT3x-DIS Test ===
Temperature: 24.53 °C
Humidity:    48.21 %RH
```

## Notes

- Supply voltage is **3.3V** — do not connect VDD to 5V.
- If the ADDR pin is left floating the address defaults to 0x44; tie to 3.3V for 0x45 and update `SHT3X_ADDR`.
- The sketch reports CRC errors and sensor-not-found conditions on Serial without halting, so it recovers automatically if the bus glitches.
