#include <WiFi.h>
#include <HTTPClient.h>

const int second = 1000;
const int waitTime = 10 * second;

const int hardwareID = 0;
const int serialBaud = 115200;
const int serialSetupTime = 4 * second;

const char* wifiSsid = "frun.";
const char* wifiPassword = "nodemcutest1";
const int connectRetries = 5;
const int retryTime = 2 * second;

bool connectToWiFi() {
  WiFi.begin(wifiSsid, wifiPassword);

  for (int i = 0; i < connectRetries && WiFi.status() != WL_CONNECTED; i++) {
    delay(retryTime);
    Serial.println("Connecting to WiFi...");
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect to WiFi");
    return false;
  }
  Serial.println("Connected to WiFi network");
  return true;
}

bool reconnectToWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Reconnecting to WiFi...");
    connectToWiFi();
  }
  return (WiFi.status() == WL_CONNECTED);
}

char* preparePayload(float humidity, bool didWater) {
  char humidityStr[5];
  dtostrf(humidity, 4, 2, humidityStr);
  static char payload[128];
  sprintf(payload, "{\"hardwareID\":%d,\"humidityPercent\":%s,\"didWater\":%d}", hardwareID, humidityStr, didWater);
  return payload;
}

int sendPostRequest(HTTPClient* http, char* payload) {
  http->begin("http://jsonplaceholder.typicode.com/posts"); //TODO: Change it later to lambda
  http->addHeader("Content-Type", "application/json");
  int httpResponseCode = http->POST(payload);
  return httpResponseCode;
}

void printResponse(int responseCode, String response) {
  if(responseCode>0) {
    Serial.println(responseCode);   
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
  Serial.begin(serialBaud);
  delay(serialSetupTime);
  connectToWiFi();
}

void loop() {
  Serial.println("Checking humidity...");
  float humidity = 0.0; //TODO: Implement real function
  
  Serial.println("Watering plants...");
  bool didWater = false; //TODO: Implement real function
  
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
