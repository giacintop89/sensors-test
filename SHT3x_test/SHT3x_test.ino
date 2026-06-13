// SHT3x-DIS Humidity & Temperature Sensor Test
// Sensirion SHT30 / SHT31 / SHT35
//
// Wiring (Arduino Uno):
//   SDA  -> A4
//   SCL  -> A5
//   VDD  -> 3.3V
//   GND  -> GND
//   ADDR -> GND  (I2C address 0x44)
//           3.3V (I2C address 0x45)
//
// Library: Wire (built-in, no install needed)

#include <Wire.h>

#define SHT3X_ADDR 0x44

// Single-shot, high repeatability, clock stretching enabled
#define CMD_MSB 0x2C
#define CMD_LSB 0x06

// CRC-8 polynomial: x^8 + x^5 + x^4 + 1 (0x31), init 0xFF
static uint8_t crc8(const uint8_t *data, uint8_t len) {
  uint8_t crc = 0xFF;
  for (uint8_t i = 0; i < len; i++) {
    crc ^= data[i];
    for (uint8_t b = 0; b < 8; b++)
      crc = (crc & 0x80) ? (crc << 1) ^ 0x31 : (crc << 1);
  }
  return crc;
}

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Soft-reset
  Wire.beginTransmission(SHT3X_ADDR);
  Wire.write(0x30);
  Wire.write(0xA2);
  Wire.endTransmission();
  delay(2);

  Serial.println("=== SHT3x-DIS Test ===");
}

void loop() {
  // Trigger single-shot measurement (high repeatability, clock stretch)
  Wire.beginTransmission(SHT3X_ADDR);
  Wire.write(CMD_MSB);
  Wire.write(CMD_LSB);
  if (Wire.endTransmission() != 0) {
    Serial.println("ERROR: sensor not found (check wiring & address)");
    delay(2000);
    return;
  }

  delay(20); // max measurement time ~15 ms (high repeatability)

  // Read 6 bytes: [T_MSB, T_LSB, T_CRC, RH_MSB, RH_LSB, RH_CRC]
  uint8_t n = Wire.requestFrom((uint8_t)SHT3X_ADDR, (uint8_t)6);
  if (n < 6) {
    Serial.println("ERROR: incomplete read");
    delay(1000);
    return;
  }

  uint8_t buf[6];
  for (uint8_t i = 0; i < 6; i++) buf[i] = Wire.read();

  if (crc8(buf, 2) != buf[2]) {
    Serial.println("ERROR: temperature CRC mismatch");
    delay(1000);
    return;
  }
  if (crc8(buf + 3, 2) != buf[5]) {
    Serial.println("ERROR: humidity CRC mismatch");
    delay(1000);
    return;
  }

  uint16_t rawT  = ((uint16_t)buf[0] << 8) | buf[1];
  uint16_t rawRH = ((uint16_t)buf[3] << 8) | buf[4];

  // Datasheet formulas (section 4.13)
  float tempC = -45.0f + 175.0f * ((float)rawT  / 65535.0f);
  float rh    = 100.0f *           ((float)rawRH / 65535.0f);

  Serial.print("Temperature: ");
  Serial.print(tempC, 2);
  Serial.println(" °C");

  Serial.print("Humidity:    ");
  Serial.print(rh, 2);
  Serial.println(" %RH");

  Serial.println();
  delay(1000);
}
