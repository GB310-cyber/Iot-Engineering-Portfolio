
#include <WiFi.h>
#include <time.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include "DHTesp.h"

// NURQEN sensors
#define MQ2_AO 4
#define MQ2_DO 5
#define DHT_PIN 6

// TFT display pins
#define TFT_CS 10
#define TFT_DC 9
#define TFT_MOSI 11
#define TFT_SCK 12

// Touch controller I2C
#define TOUCH_SDA 15
#define TOUCH_SCL 14

// Wokwi Wi-Fi and Nigeria time
const char* WIFI_NAME = "Wokwi-GUEST";
const char* WIFI_PASSWORD = "";

const long UTC_OFFSET = 3600;
const int DST_OFFSET = 0;

// Demonstration thresholds; calibrate for real hardware
const int GAS_THRESHOLD = 2000;
const float TEMP_HIGH = 35.0;
const float TEMP_CLEAR = 32.0;
const unsigned long TEMP_CONFIRM_MS = 5000;

Adafruit_ILI9341 tft(TFT_CS, TFT_DC);
Adafruit_FT6206 touch;
DHTesp dht;

int gasValue = 0;
int gasDigital = HIGH;

float temperature = NAN;
float humidity = NAN;

bool touchReady = false;
bool previousGasAlert = false;
bool tempAlert = false;

unsigned long hotSince = 0;
unsigned long lastSensorRead = 0;
unsigned long lastScreenUpdate = 0;

String lastEventTime = "No events yet";
String currentTime = "Time not synced";
String currentDate = "";

bool getClockTime(struct tm* info) {
  return getLocalTime(info, 100);
}

String getTimestamp() {
  struct tm info;

  if (!getClockTime(&info)) {
    return "TIME NOT SYNCED";
  }

  char buffer[25];
  strftime(buffer, sizeof(buffer),
           "%Y-%m-%d %H:%M:%S", &info);

  return String(buffer);
}

void updateClockStrings() {
  struct tm info;

  if (getClockTime(&info)) {
    char timeBuffer[16];
    char dateBuffer[16];

    strftime(timeBuffer, sizeof(timeBuffer),
             "%H:%M:%S", &info);

    strftime(dateBuffer, sizeof(dateBuffer),
             "%d/%m/%Y", &info);

    currentTime = String(timeBuffer);
    currentDate = String(dateBuffer);
  } else {
    currentTime = "Time not synced";
    currentDate = "";
  }
}

void recordEvent(const char* eventName) {
  lastEventTime = getTimestamp();

  Serial.println();
  Serial.println("=== HOME EVENT ===");
  Serial.print("Event: ");
  Serial.println(eventName);

  Serial.print("Timestamp: ");
  Serial.print(lastEventTime);
  Serial.println(" WAT");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");

  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.print("MQ-2 reading: ");
  Serial.println(gasValue);

  Serial.println("====================");
}

void drawScreen() {
  bool gasAlert =
      (gasValue >= GAS_THRESHOLD) ||
      (gasDigital == LOW);

  bool anyAlert = gasAlert || tempAlert;

  tft.fillScreen(0x0841);

  // Header
  tft.fillRect(0, 0, 320, 42, 0x115F);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.print("NURQEN");

  tft.setTextSize(1);
  tft.setCursor(210, 7);
  tft.print(currentDate);

  tft.setCursor(210, 23);
  tft.print(currentTime);

  // Temperature card
  tft.drawRoundRect(10, 50, 145, 65, 8, 0x7BEF);
  tft.setTextSize(1);
  tft.setTextColor(0x7BEF);
  tft.setCursor(20, 60);
  tft.print("TEMPERATURE");

  tft.setTextSize(2);
  tft.setTextColor(tempAlert ? ILI9341_RED : ILI9341_WHITE);
  tft.setCursor(20, 83);

  if (isnan(temperature)) {
    tft.print("--.- C");
  } else {
    tft.print(temperature, 1);
    tft.print(" C");
  }

  // Humidity card
  tft.drawRoundRect(165, 50, 145, 65, 8, 0x7BEF);
  tft.setTextSize(1);
  tft.setTextColor(0x7BEF);
  tft.setCursor(175, 60);
  tft.print("HUMIDITY");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(175, 83);

  if (isnan(humidity)) {
    tft.print("-- %");
  } else {
    tft.print(humidity, 1);
    tft.print(" %");
  }

  // Gas card
  tft.drawRoundRect(10, 125, 300, 48, 8, 0x7BEF);
  tft.setTextSize(1);
  tft.setTextColor(0x7BEF);
  tft.setCursor(20, 132);
  tft.print("MQ-2 GAS / SMOKE READING");

  tft.setTextSize(2);
  tft.setTextColor(gasAlert ? ILI9341_RED : ILI9341_WHITE);
  tft.setCursor(20, 147);
  tft.print(gasValue);

  // Overall alert
  tft.drawRoundRect(10, 180, 300, 32, 8, 0x7BEF);
  tft.setTextSize(2);
  tft.setCursor(20, 190);

  if (gasAlert) {
    tft.setTextColor(ILI9341_RED);
    tft.print("GAS / SMOKE ALERT");
  } else if (tempAlert) {
    tft.setTextColor(ILI9341_RED);
    tft.print("HIGH TEMP ALERT");
  } else {
    tft.setTextColor(ILI9341_GREEN);
    tft.print("STATUS: NORMAL");
  }

  // Last event
  tft.setTextSize(1);
  tft.setTextColor(0x7BEF);
  tft.setCursor(10, 220);
  tft.print("LAST EVENT:");

  tft.setCursor(10, 232);
  tft.print(lastEventTime);
}

void printReadings() {
  Serial.println("-------------------------");
  Serial.print("Time: ");
  Serial.print(getTimestamp());
  Serial.println(" WAT");

  Serial.print("Temperature: ");
  if (isnan(temperature)) {
    Serial.println(" 10.0 ");
  } else {
    Serial.print(temperature, 1);
    Serial.println(" C");
  }

  Serial.print("Humidity: ");
  if (isnan(humidity)) {
    Serial.println(" 15% ");
  } else {
    Serial.print(humidity, 1);
    Serial.println(" %");
  }

  Serial.print("MQ-2 analog: ");
  Serial.println(gasValue);

  Serial.print("MQ-2 digital: ");
  Serial.println(gasDigital);

  Serial.print("Temperature alert: ");
  Serial.println(tempAlert ? "ACTIVE" : "OFF");
}

void setup() {
  Serial.begin(115200);

  pinMode(MQ2_DO, INPUT);
  analogReadResolution(12);

  dht.setup(DHT_PIN, DHTesp::DHT22);

  SPI.begin(TFT_SCK, -1, TFT_MOSI, TFT_CS);
  tft.begin();
  tft.setRotation(1);

  Wire.begin(TOUCH_SDA, TOUCH_SCL);
  touchReady = touch.begin(40);

  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 30);
  tft.println("HOME Starting");

  WiFi.begin(WIFI_NAME, WIFI_PASSWORD, 6);

  Serial.println("Connecting to Wokwi Wi-Fi...");

  unsigned long start = millis();

  while (WiFi.status() != WL_CONNECTED &&
         millis() - start < 15000) {
    delay(250);
    Serial.print(".");
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nWi-Fi connected");

    configTime(UTC_OFFSET, DST_OFFSET,
               "pool.ntp.org", "time.nist.gov");

    struct tm info;
    unsigned long timeStart = millis();

    while (!getLocalTime(&info, 250) &&
           millis() - timeStart < 10000) {
      delay(100);
    }
  } else {
    Serial.println("\nWi-Fi unavailable; time not synced.");
  }

  updateClockStrings();
}

void loop() {
  unsigned long now = millis();

  // Read sensors every two seconds
  if (now - lastSensorRead >= 2000 ||
      lastSensorRead == 0) {
    lastSensorRead = now;

    gasValue = analogRead(MQ2_AO);
    gasDigital = digitalRead(MQ2_DO);

    TempAndHumidity data = dht.getTempAndHumidity();
    temperature = data.temperature;
    humidity = data.humidity;

    bool gasAlert =
        (gasValue >= GAS_THRESHOLD) ||
        (gasDigital == LOW);

    // Record the start of a new gas event
    if (gasAlert && !previousGasAlert) {
      recordEvent("GAS / SMOKE ALERT");
    }

    previousGasAlert = gasAlert;

    // Temperature warning with confirmation delay
    if (!isnan(temperature)) {
      if (temperature >= TEMP_HIGH) {
        if (hotSince == 0) {
          hotSince = millis();
        }

        if (!tempAlert &&
            millis() - hotSince >= TEMP_CONFIRM_MS) {
          tempAlert = true;
          recordEvent("HIGH TEMPERATURE WARNING");
        }
      } else {
        hotSince = 0;

        if (temperature < TEMP_CLEAR) {
          tempAlert = false;
        }
      }
    }

    printReadings();
  }

  // Refresh screen and clock every second
  if (now - lastScreenUpdate >= 1000 ||
      lastScreenUpdate == 0) {
    lastScreenUpdate = now;
    updateClockStrings();
    drawScreen();
  }

  // Touch detection
  if (touchReady && touch.touched()) {
    TS_Point point = touch.getPoint();

    Serial.print("Touch detected: ");
    Serial.print(point.x);
    Serial.print(", ");
    Serial.println(point.y);

    delay(250);
  }

  delay(20);
}
