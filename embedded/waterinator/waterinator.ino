#include <WiFi.h>
#include <HTTPClient.h>

const int second = 1000;
const int waitTime = 30 * 60 * second;

const int enginePin = 13;
const int engineOnTime = 5 * second;
const int engineBufferTime = 2 * second;

const int sensorPin = 34;
const int humidityLowThreshold = 62; // It should be defined via testing.
const int humidityHighThreshold = 63; // It should be defined via testing.
const int wateringCounterThreshold = 10;
const int betweenWateringTime = 30 * second;

const int serialBaud = 115200;
const int serialSetupTime = 4 * second;

const char* wifiSsid = "";
const char* wifiPassword = "";
const int connectRetries = 5;
const int connectRetryTime = 2 * second;

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
  return true;
}

int getHumidity() {
  int humidity = map(analogRead(sensorPin), 0, 1023, 100, 0);
  Serial.print("Humitity value: ");
  Serial.print(humidity);
  Serial.println("%");
  return humidity;
}

bool shouldStartWatering(int humidity) {
  return (humidity < humidityLowThreshold);
}

bool shouldContinueWatering(int humidity, int wateringCounter) {
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

char* preparePayload(int humidity, bool didWater) {
  static char payload[128];
  sprintf(payload, "{\"hardwareID\":%d,\"humidityPercent\":%d,\"didWater\":%d}", hardwareID, humidity, didWater);
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
    Serial.print("Response code: ");
    Serial.println(responseCode);
    Serial.print("Response body: ");
    Serial.println(response);
  } else {
    Serial.print("Error while sending POST request: ");
    Serial.println(responseCode);
  }
}

void wait() {
  delay(waitTime);
}

void setup() {
  pinMode(enginePin, OUTPUT);
  Serial.begin(serialBaud);
  delay(serialSetupTime);
  connectToWiFi();
}

void loop() {
  Serial.println("Checking humidity...");
  int humidity = getHumidity();

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
