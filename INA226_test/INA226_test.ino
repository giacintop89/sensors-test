// INA226 36V / 16-bit I2C Power Monitor Test
// Texas Instruments INA226
//
// Wiring (Arduino Uno):
//   SDA  -> A4
//   SCL  -> A5
//   VS   -> 3.3V or 5V (device supply)
//   GND  -> GND
//   A0   -> GND  |
//   A1   -> GND  |  I2C address 0x40
//   IN+  -> high side of shunt resistor (supply side)
//   IN-  -> low  side of shunt resistor (load side)
//   VBUS -> bus voltage measurement point (same as IN-)
//
// Shunt: 0.1 Ω resistor between IN+ and IN-
//   Max measurable current: ~819 mA (limited by ±81.92 mV shunt range)
//
// Library: Wire (built-in, no install needed)

#include <Wire.h>

#define INA226_ADDR 0x40  // A0=GND, A1=GND

// Register addresses
#define REG_CONFIG  0x00
#define REG_SHUNT_V 0x01  // signed, 2.5 µV/LSB
#define REG_BUS_V   0x02  // unsigned, 1.25 mV/LSB
#define REG_POWER   0x03  // unsigned, 25 * CURRENT_LSB per LSB
#define REG_CURRENT 0x04  // signed, CURRENT_LSB per LSB
#define REG_CALIB   0x05

// Configuration: AVG=1, VBUS_CT=1.1ms, VSHUNT_CT=1.1ms, mode=shunt+bus continuous
// Bits [15]: RST=0
// Bits [14:12]: AVG=000 (1 sample)
// Bits [11:9]:  VBUSCT=100 (1.1 ms)
// Bits [8:6]:   VSHCT=100  (1.1 ms)
// Bits [2:0]:   MODE=111   (shunt+bus, continuous)
#define CONFIG_VALUE 0x4127

// Calibration for 0.1 Ω shunt, 100 µA/LSB current resolution
// Cal = trunc(0.00512 / (CURRENT_LSB * R_SHUNT))
//     = trunc(0.00512 / (0.0001 * 0.1)) = 512
#define R_SHUNT      0.1f      // Ω
#define CURRENT_LSB  0.0001f   // A/LSB = 100 µA
#define CALIB_VALUE  512       // dimensionless
#define POWER_LSB    (25.0f * CURRENT_LSB)  // W/LSB = 2.5 mW

static void writeReg(uint8_t reg, uint16_t value) {
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(reg);
  Wire.write((uint8_t)(value >> 8));
  Wire.write((uint8_t)(value & 0xFF));
  Wire.endTransmission();
}

static uint16_t readReg(uint8_t reg) {
  Wire.beginTransmission(INA226_ADDR);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom((uint8_t)INA226_ADDR, (uint8_t)2);
  uint16_t val = ((uint16_t)Wire.read() << 8) | Wire.read();
  return val;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Reset device
  writeReg(REG_CONFIG, 0x8000);
  delay(1);

  writeReg(REG_CONFIG, CONFIG_VALUE);
  writeReg(REG_CALIB,  CALIB_VALUE);

  Serial.println("=== INA226 Power Monitor Test ===");
  Serial.print("Shunt resistor: "); Serial.print(R_SHUNT * 1000.0f, 0); Serial.println(" mΩ");
  Serial.print("Current LSB:    "); Serial.print(CURRENT_LSB * 1000000.0f, 0); Serial.println(" µA");
  Serial.println();
}

void loop() {
  // Bus voltage: unsigned, 1.25 mV/LSB
  float busV   = (float)readReg(REG_BUS_V) * 0.00125f;

  // Shunt voltage: signed, 2.5 µV/LSB
  float shuntV = (float)(int16_t)readReg(REG_SHUNT_V) * 0.0000025f;

  // Current: signed, CURRENT_LSB per LSB (set by calibration register)
  float current = (float)(int16_t)readReg(REG_CURRENT) * CURRENT_LSB;

  // Power: unsigned, POWER_LSB per LSB
  float power  = (float)readReg(REG_POWER) * POWER_LSB;

  Serial.print("Bus Voltage:   "); Serial.print(busV, 4);            Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntV * 1000.0f, 4); Serial.println(" mV");
  Serial.print("Current:       "); Serial.print(current * 1000.0f, 3); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power * 1000.0f, 3);  Serial.println(" mW");
  Serial.println();

  delay(1000);
}
