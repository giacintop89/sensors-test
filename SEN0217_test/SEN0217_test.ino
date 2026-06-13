// SEN0217 YF-S201 Water Flow Sensor Test (1/2")
// DFRobot SEN0217
//
// Wiring (Arduino Uno):
//   Green (Signal) -> D2  (INT0)
//   Red   (VCC)   -> 5V
//   Black (GND)   -> GND
//
// No external library needed.
//
// Sensor spec: 1 litre = 450 pulses (square waves)
// Flow range: 1-30 L/min, accuracy ±5% (2-30 L/min)

#define FLOW_PIN 2

volatile uint32_t pulseCount = 0;
static float     totalLitres = 0.0f;

void onPulse() {
  pulseCount++;
}

void setup() {
  Serial.begin(9600);
  pinMode(FLOW_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(FLOW_PIN), onPulse, RISING);
  Serial.println("=== SEN0217 Water Flow Sensor Test ===");
  Serial.println("Waiting for flow...");
  Serial.println();
}

void loop() {
  delay(1000); // 1-second sampling window

  // Atomically snapshot and clear counter
  noInterrupts();
  uint32_t count = pulseCount;
  pulseCount = 0;
  interrupts();

  // 1 L = 450 pulses  →  flow rate (L/min) = pulses_per_sec / 450 * 60
  float flowRateLmin = (float)count / 450.0f * 60.0f;
  totalLitres       += (float)count / 450.0f;

  Serial.print("Flow Rate:    ");
  Serial.print(flowRateLmin, 3);
  Serial.println(" L/min");

  Serial.print("Pulse Count:  ");
  Serial.println(count);

  Serial.print("Total Volume: ");
  Serial.print(totalLitres, 4);
  Serial.println(" L");

  Serial.println();
}
