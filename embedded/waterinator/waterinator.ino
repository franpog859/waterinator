#include <WiFi.h>
#include <HTTPClient.h>

const int second = 1000;
const int waitTime = 15 * 60 * second;

const int ledPin = 2;

const int enginePin = 13;
const int engineOnTime = 10 * second;
const int engineBufferTime = 2 * second;

const int sensorPin = 34;
const float humidityLowThreshold = 71.5;
const float humidityHighThreshold = 72.2;
const int wateringCounterThreshold = 5;
const int betweenWateringTime = 30 * second;
const int sensorBlinkLedTime = 250;

const int serialBaud = 115200;
const int serialSetupTime = 4 * second;

const char* wifiSsid = "";
const char* wifiPassword = "";
const int connectRetries = 5;
const int connectRetryTime = 2 * second;
const int connectBlinkLedCounter = 3;
const int connectBlinkLedTime = 150;

const int hardwareID = 0;
const char* serviceURL = "";

bool connectToWiFi() {
  WiFi.begin(wifiSsid, wifiPassword);

  for (int i = 0; i < connectRetries && WiFi.status() != WL_CONNECTED; i++) {
    delay(connectRetryTime);
    Serial.println("Connecting to WiFi...");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return false;
  }
  Serial.println("Connected to WiFi network");
  blinkLed(connectBlinkLedCounter, connectBlinkLedTime);
  return true;
}

void blinkLed(int howMany, int howLong) {
  for (int i = 0; i < howMany; i++) {
    digitalWrite(ledPin, HIGH);
    delay(howLong);
    digitalWrite(ledPin, LOW);
    delay(howLong);
  }
}

float getHumidity() {
  int sensorMeasurement = map(analogRead(sensorPin), 0, 1023, 1000, 0);
  blinkSensorMeasurement(sensorMeasurement);
  float humidity = (float)sensorMeasurement / 10.0;
  Serial.printf("Humitity value: %.1f %s\n", humidity, "%");
  return humidity;
}

void blinkSensorMeasurement(int sensorMeasurement) {
  Serial.println("Blinking sensor measurement...");
  int hundreds = sensorMeasurement / 100;
  blinkLed(hundreds, sensorBlinkLedTime);
  delay(second);
  int tens = (sensorMeasurement - 100 * hundreds) / 10;
  blinkLed(tens, sensorBlinkLedTime);
  delay(second);
  int units = (sensorMeasurement - 100 * hundreds) - 10 * tens;
  blinkLed(units, sensorBlinkLedTime);
}

bool shouldStartWatering(float humidity) {
  return (humidity < humidityLowThreshold);
}

bool shouldContinueWatering(float humidity, int wateringCounter) {
  return (humidity < humidityHighThreshold && wateringCounter < wateringCounterThreshold);
}

void water() {
  Serial.println("Turning the engine on...");
  digitalWrite(enginePin, HIGH);
  delay(engineOnTime);
  digitalWrite(enginePin, LOW);
  Serial.println("Making sure that the engine is off...");
  delay(engineBufferTime);
  digitalWrite(enginePin, LOW);
  delay(engineBufferTime);
  digitalWrite(enginePin, LOW);
}

bool reconnectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    connectToWiFi();
  }
  return (WiFi.status() == WL_CONNECTED);
}

char* preparePayload(float humidity, bool didWater) {
  static char payload[128];
  sprintf(payload, "{\"hardwareID\":%d,\"humidityPercent\":%.1f,\"didWater\":%d}", hardwareID, humidity, didWater);
  return payload;
}

int sendPostRequest(HTTPClient* http, char* payload) {
  http->begin(serviceURL);
  http->addHeader("Content-Type", "application/json");
  int httpResponseCode = http->POST(payload);
  return httpResponseCode;
}

void printResponse(int responseCode, String response) {
  if (responseCode > 0) {
    Serial.printf("Response code: %d\nResponseBody: %s\n", responseCode, response);
  } else {
    Serial.printf("Error while sending POST request: %d\n", responseCode);
  }
}

void wait() {
  delay(waitTime);
}

void setup() {
  pinMode(ledPin, OUTPUT);
  pinMode(enginePin, OUTPUT);
  Serial.begin(serialBaud);
  delay(serialSetupTime);
  connectToWiFi();
}

void loop() {
  Serial.println("Checking humidity...");
  float humidity = getHumidity();

  bool didWater = false;
  if (shouldStartWatering(humidity)) {
    int wateringCounter = 0;
    while (shouldContinueWatering(humidity, wateringCounter)) {
      Serial.println("Watering plants...");
      water();
      delay(betweenWateringTime);

      wateringCounter++;
      didWater = true;
      humidity = getHumidity();
    }
  }

  if (!reconnectToWiFi()) {
    Serial.println("Waiting...");
    wait();
    return;
  }

  Serial.println("Preparing payload...");
  humidity = getHumidity();
  char* payload = preparePayload(humidity, didWater);

  Serial.println("Sending http request...");
  HTTPClient http;
  int responseCode = sendPostRequest(&http, payload);
  printResponse(responseCode, http.getString());
  http.end();

  Serial.println("Waiting...");
  wait();
}
