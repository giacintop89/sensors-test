# DS18B20 Temperature Sensor Test

Arduino sketch for the **Maxim Integrated DS18B20** 1-Wire digital thermometer.

## Wiring (Arduino Uno)

| DS18B20 Pin | Arduino |
|-------------|---------|
| GND (pin 1) | GND |
| DQ  (pin 2) | D2 + 4.7 kΩ pullup to 5V |
| VDD (pin 3) | 5V |

> External power mode. The 4.7 kΩ pullup resistor on DQ is required.

## Dependencies

| Library | Install via |
|---------|-------------|
| `OneWire` by Paul Stoffregen | Arduino Library Manager |

## What it does

1. Scans the 1-Wire bus and finds the first device.
2. Validates the 64-bit ROM address CRC and confirms the DS18B20 family code (`0x28`).
3. Triggers a 12-bit temperature conversion (waits 800 ms).
4. Reads and CRC-validates the 9-byte scratchpad.
5. Prints the ROM address and temperature (°C, 4 decimal places) to Serial at 9600 baud every ~2 s.

## Serial output example

```
=== DS18B20 Test ===
ROM: 28:AA:3F:12:00:00:00:CB
Temperature: 23.1250 °C
```

## Notes

- Resolution is 12-bit → 0.0625 °C per LSB.
- Multiple sensors on the same bus are supported by the OneWire library, but this sketch only reads the first device found.
