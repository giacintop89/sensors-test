// DS18B20 1-Wire Digital Thermometer Test
// Maxim Integrated DS18B20
//
// Wiring (Arduino Uno):
//   GND (pin 1) -> GND
//   DQ  (pin 2) -> D2  + 4.7 kΩ pullup to 5V (external power mode)
//   VDD (pin 3) -> 5V
//
// Library: OneWire  (install via Library Manager: "OneWire" by Paul Stoffregen)

#include <OneWire.h>

#define ONE_WIRE_PIN 2
#define DS18B20_FAMILY_CODE 0x28

OneWire ds(ONE_WIRE_PIN);

void setup() {
  Serial.begin(9600);
  Serial.println("=== DS18B20 Test ===");
}

void loop() {
  uint8_t addr[8];

  // Discover first device on the bus
  ds.reset_search();
  if (!ds.search(addr)) {
    Serial.println("ERROR: no 1-Wire device found (check wiring & pullup)");
    delay(2000);
    return;
  }

  // Validate ROM CRC
  if (OneWire::crc8(addr, 7) != addr[7]) {
    Serial.println("ERROR: ROM CRC mismatch");
    delay(1000);
    return;
  }

  // Confirm DS18B20 family code
  if (addr[0] != DS18B20_FAMILY_CODE) {
    Serial.print("ERROR: unexpected family code 0x");
    Serial.println(addr[0], HEX);
    delay(1000);
    return;
  }

  // Print 64-bit ROM address
  Serial.print("ROM: ");
  for (uint8_t i = 0; i < 8; i++) {
    if (addr[i] < 0x10) Serial.print('0');
    Serial.print(addr[i], HEX);
    if (i < 7) Serial.print(':');
  }
  Serial.println();

  // Start temperature conversion (12-bit resolution, max 750 ms)
  ds.reset();
  ds.select(addr);
  ds.write(0x44); // Convert T command

  delay(800); // wait > 750 ms for 12-bit conversion

  // Read scratchpad (9 bytes)
  ds.reset();
  ds.select(addr);
  ds.write(0xBE); // Read Scratchpad command

  uint8_t data[9];
  for (uint8_t i = 0; i < 9; i++) data[i] = ds.read();

  // Validate scratchpad CRC
  if (OneWire::crc8(data, 8) != data[8]) {
    Serial.println("ERROR: scratchpad CRC mismatch");
    delay(1000);
    return;
  }

  // Temperature is 16-bit signed two's complement, LSB = 0.0625 °C (12-bit)
  int16_t rawTemp = ((int16_t)data[1] << 8) | data[0];
  float tempC = (float)rawTemp / 16.0f;

  Serial.print("Temperature: ");
  Serial.print(tempC, 4);
  Serial.println(" °C");

  Serial.println();
  delay(1000);
}
