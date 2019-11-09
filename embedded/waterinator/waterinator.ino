#include <WiFi.h>
#include <HTTPClient.h>

const int second = 1000;
const int waitTime = 30 * 60 * second;

const int enginePin = 13;
const int engineOnTime = 1 * second;
const int engineBufferTime = 2 * second;

const int sensorPin = 34;
const int humidityThreshold = 30;

const int serialBaud = 115200;
const int serialSetupTime = 4 * second;

const char* wifiSsid = "frun.";
const char* wifiPassword = "nodemcutest1";
const int connectRetries = 5;
const int connectRetryTime = 2 * second;

const int hardwareID = 0;
const char* serviceURL = "https://us-central1-directed-portal-233418.cloudfunctions.net/SaveSensorDataToDatabase";

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
  return humidity;
}

bool shouldWater(int humidity) {
  Serial.print("Humitity value: ");
  Serial.print(humidity);
  Serial.println("%");
  return (humidity < humidityThreshold);
}

bool water() {
  Serial.println("Turning the engine on...");
  digitalWrite(enginePin, HIGH);
  delay(engineOnTime);
  digitalWrite(enginePin, LOW);
  Serial.println("Making sure that the engine is off...");
  delay(engineBufferTime);
  digitalWrite(enginePin, LOW);
  delay(engineBufferTime);
  digitalWrite(enginePin, LOW);
  return true;
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
  if (shouldWater(humidity)) {
    Serial.println("Watering plants...");
    didWater = water();
  }

  if (!reconnectToWiFi()) {
    wait();
    return;
  }

  Serial.println("Preparing payload...");
  char* payload = preparePayload(humidity, didWater);

  Serial.println("Sending http request...");
  HTTPClient http;
  int responseCode = sendPostRequest(&http, payload);
  printResponse(responseCode, http.getString());
  http.end();

  Serial.println("Waiting...");
  wait();
}
