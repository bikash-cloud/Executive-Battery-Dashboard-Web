#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// WiFi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Firebase
const char* FIREBASE_LIVE_URL =
"https://executive-battery-dashboard-default-rtdb.asia-southeast1.firebasedatabase.app/live.json";

const char* FIREBASE_HISTORY_URL =
"https://executive-battery-dashboard-default-rtdb.asia-southeast1.firebasedatabase.app/history.json";

// Pins
#define CELL1_PIN 33
#define CELL2_PIN 35
#define CELL3_PIN 34
#define CELL4_PIN 32
#define RELAY_PIN 25
#define BUZZER_PIN 26

// Limits
#define LOW_LIMIT 1.00
#define HIGH_LIMIT 3.20
#define IMBALANCE_LIMIT 30.0

// Timing
#define ADC_INTERVAL 300
#define LCD_INTERVAL 600
#define SERIAL_INTERVAL 1000
#define FIREBASE_INTERVAL 1500
#define HISTORY_INTERVAL 3000
#define BUZZER_INTERVAL 300

float cell[4];
float packVoltage = 0;
float avgVoltage = 0;
float maxVoltage = 0;
float minVoltage = 0;
float imbalance = 0;
float healthScore = 100;

bool fault = false;
bool relayState = true;
bool buzzerState = false;

String riskLevel = "LOW";
String lastFault = "No Fault";
String recommendation = "Battery operating normally";
String runtimeState = "NORMAL";

unsigned long lastADC = 0;
unsigned long lastLCD = 0;
unsigned long lastSerial = 0;
unsigned long lastFirebase = 0;
unsigned long lastHistory = 0;
unsigned long lastBuzzer = 0;

String lastLine0 = "";
String lastLine1 = "";

float readVoltage(int pin) {
  int adc = analogRead(pin);
  return (adc * 3.3) / 4095.0;
}

void printLCDLine(int row, String text) {
  if (text.length() > 16) text = text.substring(0, 16);
  while (text.length() < 16) text += " ";

  if (row == 0 && text != lastLine0) {
    lcd.setCursor(0, 0);
    lcd.print(text);
    lastLine0 = text;
  }

  if (row == 1 && text != lastLine1) {
    lcd.setCursor(0, 1);
    lcd.print(text);
    lastLine1 = text;
  }
}

void readCells() {
  cell[0] = readVoltage(CELL1_PIN);
  cell[1] = readVoltage(CELL2_PIN);
  cell[2] = readVoltage(CELL3_PIN);
  cell[3] = readVoltage(CELL4_PIN);
}

void analyzeBattery() {
  packVoltage = 0;
  maxVoltage = cell[0];
  minVoltage = cell[0];

  for (int i = 0; i < 4; i++) {
    packVoltage += cell[i];
    if (cell[i] > maxVoltage) maxVoltage = cell[i];
    if (cell[i] < minVoltage) minVoltage = cell[i];
  }

  avgVoltage = packVoltage / 4.0;
  imbalance = (avgVoltage > 0) ? ((maxVoltage - minVoltage) / avgVoltage) * 100.0 : 0;

  fault = false;
  riskLevel = "LOW";
  lastFault = "No Fault";
  recommendation = "Battery operating normally";
  runtimeState = "NORMAL";

  for (int i = 0; i < 4; i++) {
    if (cell[i] < LOW_LIMIT) {
      fault = true;
      riskLevel = "CRITICAL";
      lastFault = "Low Cell " + String(i + 1);
      recommendation = "Inspect or replace weak battery cell";
      runtimeState = "FAULT";
    }

    if (cell[i] > HIGH_LIMIT) {
      fault = true;
      riskLevel = "HIGH";
      lastFault = "Overvolt Cell " + String(i + 1);
      recommendation = "Stop charging and isolate battery pack";
      runtimeState = "FAULT";
    }
  }

  if (imbalance > IMBALANCE_LIMIT) {
    fault = true;
    riskLevel = "HIGH";
    lastFault = "High Imbalance";
    recommendation = "Perform cell balancing";
    runtimeState = "FAULT";
  }

  healthScore = 100.0 - imbalance;
  if (healthScore < 0) healthScore = 0;
  if (healthScore > 100) healthScore = 100;

  if (fault)
{
    relayState = false;
    buzzerState = true;

    digitalWrite(RELAY_PIN, LOW);
}
else
{
    runtimeState = "NORMAL";
    riskLevel = "LOW";
    lastFault = "No Fault";
    recommendation = "Battery operating normally";

    relayState = true;
    buzzerState = false;

    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
}
}

void updateLCD() {
  if (fault) {
    printLCDLine(0, "RISK:" + riskLevel);
    printLCDLine(1, lastFault);
  } else {
    printLCDLine(0, "HEALTH:" + String(healthScore, 0) + "%");
    printLCDLine(1, "Pack:" + String(packVoltage, 2) + "V");
  }
}

void updateBuzzer() {
  if (fault) {
    if (millis() - lastBuzzer >= BUZZER_INTERVAL) {
      lastBuzzer = millis();
      buzzerState = !buzzerState;
      digitalWrite(BUZZER_PIN, buzzerState);
    }
  } else {
    buzzerState = false;
    digitalWrite(BUZZER_PIN, LOW);
  }
}

String createTelemetryJson() {
  StaticJsonDocument<512> doc;

  doc["cell1"] = cell[0];
  doc["cell2"] = cell[1];
  doc["cell3"] = cell[2];
  doc["cell4"] = cell[3];
  doc["packVoltage"] = packVoltage;
  doc["averageVoltage"] = avgVoltage;
  doc["imbalance"] = imbalance;
  doc["healthScore"] = healthScore;
  doc["runtimeState"] = runtimeState;
  doc["riskLevel"] = riskLevel;
  doc["relayStatus"] = relayState ? "ON" : "OFF";
  doc["buzzerStatus"] = buzzerState ? "ON" : "OFF";
  doc["lastFault"] = lastFault;
  doc["recommendation"] = recommendation;
  doc["timestamp"] = millis();

  String jsonData;
  serializeJson(doc, jsonData);
  return jsonData;
}

void uploadLiveToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;

  String jsonData = createTelemetryJson();

  HTTPClient http;
  http.begin(FIREBASE_LIVE_URL);
  http.addHeader("Content-Type", "application/json");

  int responseCode = http.PUT(jsonData);

  Serial.print("Firebase Live Response: ");
  Serial.println(responseCode);

  http.end();
}

void uploadHistoryToFirebase() {
  if (WiFi.status() != WL_CONNECTED) return;

  String jsonData = createTelemetryJson();

  HTTPClient http;
  http.begin(FIREBASE_HISTORY_URL);
  http.addHeader("Content-Type", "application/json");

  int responseCode = http.POST(jsonData);

  Serial.print("Firebase History Response: ");
  Serial.println(responseCode);

  http.end();
}

void printSerialDashboard() {
  Serial.println("==========================================");
  Serial.println("EXECUTIVE BATTERY INTELLIGENCE DASHBOARD");
  Serial.println("==========================================");

  Serial.print("Cell1: "); Serial.println(cell[0], 2);
  Serial.print("Cell2: "); Serial.println(cell[1], 2);
  Serial.print("Cell3: "); Serial.println(cell[2], 2);
  Serial.print("Cell4: "); Serial.println(cell[3], 2);

  Serial.print("Pack Voltage: "); Serial.println(packVoltage, 2);
  Serial.print("Average Voltage: "); Serial.println(avgVoltage, 2);
  Serial.print("Imbalance: "); Serial.println(imbalance, 2);
  Serial.print("Health Score: "); Serial.println(healthScore, 1);

  Serial.print("Runtime State: "); Serial.println(runtimeState);
  Serial.print("Risk Level: "); Serial.println(riskLevel);
  Serial.print("Relay: "); Serial.println(relayState ? "ON" : "OFF");
  Serial.print("Buzzer: "); Serial.println(buzzerState ? "ON" : "OFF");
  Serial.print("Last Fault: "); Serial.println(lastFault);
  Serial.print("Recommendation: "); Serial.println(recommendation);

  Serial.print("WiFi: ");
  Serial.println(WiFi.status() == WL_CONNECTED ? "CONNECTED" : "DISCONNECTED");

  Serial.println();
}

void setup() {
  Serial.begin(115200);

  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(BUZZER_PIN, LOW);

  lcd.init();
  lcd.backlight();

  printLCDLine(0, "Executive BMS");
  printLCDLine(1, "Connecting WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");

  printLCDLine(0, "WiFi Connected");
  printLCDLine(1, "Firebase Ready");

  readCells();
  analyzeBattery();

  uploadLiveToFirebase();
  uploadHistoryToFirebase();
}

void loop() {
  unsigned long now = millis();

  if (now - lastADC >= ADC_INTERVAL) {
    lastADC = now;
    readCells();
    analyzeBattery();
  }

  if (now - lastLCD >= LCD_INTERVAL) {
    lastLCD = now;
    updateLCD();
  }

  if (now - lastSerial >= SERIAL_INTERVAL) {
    lastSerial = now;
    printSerialDashboard();
  }

  if (now - lastFirebase >= FIREBASE_INTERVAL) {
    lastFirebase = now;
    uploadLiveToFirebase();
  }

  if (now - lastHistory >= HISTORY_INTERVAL) {
    lastHistory = now;
    uploadHistoryToFirebase();
  }

  updateBuzzer();
}
