# Arduino Sensor Testing Repository

A collection of Arduino sketches for testing four sensors across temperature, humidity, voltage/current/power, and water flow. Each sketch targets an Arduino Uno, validates sensor communication and data integrity, and prints results to the Serial Monitor at 9600 baud.

## Sensors

| Sensor | Manufacturer | Type | Interface | Folder |
|--------|-------------|------|-----------|--------|
| SHT3x-DIS (SHT30/31/35) | Sensirion | Temperature & Humidity | I2C | [SHT3x_test/](SHT3x_test/) |
| DS18B20 | Maxim Integrated | Temperature | 1-Wire | [DS18B20_test/](DS18B20_test/) |
| INA226 | Texas Instruments | Voltage / Current / Power | I2C | [INA226_test/](INA226_test/) |
| SEN0217 (YF-S201) | DFRobot | Water Flow | Digital Pulse | [SEN0217_test/](SEN0217_test/) |

## Getting Started

**Hardware:** Arduino Uno + the sensor(s) you want to test. Wiring details are in each folder's `README.md`.

**Software:**
1. Install the [Arduino IDE](https://www.arduino.cc/en/software).
2. For the DS18B20 sketch only, install the **OneWire** library: Sketch → Include Library → Manage Libraries → search "OneWire" by Paul Stoffregen.
3. All other sketches use the built-in `Wire` library — no extra install needed.

## Repository Layout

```
sensors-test/
├── README.md
├── SENSORS.md
├── SHT3x_test/
│   ├── SHT3x_test.ino
│   └── README.md
├── DS18B20_test/
│   ├── DS18B20_test.ino
│   └── README.md
├── INA226_test/
│   ├── INA226_test.ino
│   └── README.md
├── SEN0217_test/
│   ├── SEN0217_test.ino
│   └── README.md
├── Datasheet_SHT3x_DIS.pdf
├── ds18b20.pdf
├── ina226.pdf
└── sen0217_web.pdf
```

## Datasheets

- [SHT3x-DIS](Datasheet_SHT3x_DIS.pdf)
- [DS18B20](ds18b20.pdf)
- [INA226](ina226.pdf)
- [SEN0217 / YF-S201](sen0217_web.pdf)

## License

MIT License
