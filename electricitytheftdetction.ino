// author: CoreComet industries pvt ltd.
// Electricity theft detection system using ESP32 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Initialize LCD with I2C address 0x27 (change to 0x3F if needed)
LiquidCrystal_I2C lcd(0x27, 16, 2);

#define SENSOR_PIN 34   // ACS712 OUT connected to GPIO 34 (ADC1_CH6)
float sensitivity = 0.185;       // For ACS712-5A: 0.185 V/A; 20A: 0.100; 30A: 0.066
float vRef = 0.0;                // Auto-calibrated zero-current voltage
const int sampleWindow = 1000;   // Sampling duration in milliseconds

// Divider: 1k to ACS712 output, 2k to GND
const float voltageDividerFactor = 1.5;

// ADC settings
const float ADC_REF_VOLTAGE = 3.3;   // ESP32 ADC reference
const int ADC_RESOLUTION = 4095;     // 12-bit ADC

// -----------------------------
// Added GSM + Relay configuration
// -----------------------------
#define RELAY_PIN 27           // Relay connected to GPIO 27
#define RXD2 16                // ESP32 RX2 ← SIM800L TX
#define TXD2 17                // ESP32 TX2 → SIM800L RX
#define PHONE_NUMBER "+91XXXXXXXXXX"  // Replace with your number
#define CURRENT_THRESHOLD 2.0  // Overcurrent threshold in Amps
bool alertSent = false;        // To avoid multiple SMS for same event
// -----------------------------

void setup() {
  Serial.begin(115200);
  Serial2.begin(9600, SERIAL_8N1, RXD2, TXD2);  // Initialize GSM serial
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);  // Relay ON (assuming active LOW relay)

  // ----------------------
  // LCD Initialization
  // ----------------------
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Initializing...");
  
  // ----------------------

  delay(2000); // Allow sensor to stabilize
  vRef = readZeroCurrentVoltage(SENSOR_PIN);

  Serial.print("Calibrated vRef (no current): ");
  Serial.print(vRef, 3);
  Serial.println(" V");

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Ready...");

  sendSMS("System Ready. Monitoring current...");
}

void loop() {
  float rmsCurrent = getRMSCurrent(SENSOR_PIN);
  Serial.print("RMS Current: ");
  Serial.print(rmsCurrent, 3);
  Serial.println(" A");

  // ---------- Added Overcurrent Protection ----------
  if (rmsCurrent > CURRENT_THRESHOLD) {
    digitalWrite(RELAY_PIN, LOW); // Trip relay (cutoff)
    if (!alertSent) {
      Serial.println(" Overcurrent detected! Relay tripped.");
      sendSMS(" Overcurrent detected. electricity theft detected! Supply cutoff activated.");
      alertSent = true;
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Overcurrent!");
    lcd.setCursor(0, 1);
    lcd.print(rmsCurrent, 2);
    lcd.print("A  Relay OFF");
  } 
  else {
    digitalWrite(RELAY_PIN, HIGH); // Normal ON
    alertSent = false; // Reset flag when current back to normal

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Current:");
    lcd.setCursor(9, 0);
    lcd.print(rmsCurrent, 2);
    lcd.print("A");
    lcd.setCursor(0, 1);
    lcd.print("Relay: ON ");
  }
  // --------------------------------------------------

  delay(1000);
}

// ----------------------
// Original Current Code (Unchanged)
// ----------------------

// Auto-calibrate zero-current voltage
float readZeroCurrentVoltage(int pin) {
  long sum = 0;
  const int samples = 500;

  for (int i = 0; i < samples; i++) {
    sum += analogRead(pin);
    delay(2);
  }

  float avgADC = sum / (float)samples;
  float vADC = (avgADC * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
  return vADC * voltageDividerFactor; // Convert to actual ACS712 voltage
}

// Measure RMS current
float getRMSCurrent(int pin) {
  unsigned long start = millis();
  float sumSquares = 0;
  int count = 0;

  while (millis() - start < sampleWindow) {
    int adc = analogRead(pin);
    float vADC = (adc * ADC_REF_VOLTAGE) / ADC_RESOLUTION;
    float vSensor = vADC * voltageDividerFactor;
    float current = (vSensor - vRef) / sensitivity;
    sumSquares += current * current;
    count++;
  }

  float meanSquare = sumSquares / count;
  return sqrt(meanSquare);
}

// ----------------------
// GSM SMS Function (Unchanged)
// ----------------------
void sendSMS(String message) {
  Serial.println("Sending SMS: " + message);
  Serial2.println("AT+CMGF=1"); // Set SMS to text mode
  delay(1000);
  Serial2.print("AT+CMGS=\"");
  Serial2.print(PHONE_NUMBER);
  Serial2.println("\"");
  delay(1000);
  Serial2.print(message);
  delay(500);
  Serial2.write(26); // Ctrl+Z to send SMS
  delay(3000);
  Serial.println("SMS sent.");
}
