/**
   PostHTTPClient.ino

    Created on: 21.11.2016

*/

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

/* this can be run with an emulated server on host:
        cd esp8266-core-root-dir
        cd tests/host
        make ../../libraries/ESP8266WebServer/examples/PostServer/PostServer
        bin/PostServer/PostServer
   then put your PC's IP address in SERVER_IP below, port 9080 (instead of default 80):
*/
#define SERVER_IP "192.168.0.13:8000" // PC address with emulation on host

#ifndef STASSID
#define STASSID "RT-GPON-6BDA"
#define STAPSK "KDfH6e3N"
#endif

const String DEVICE_ID = "123";
const String LOCATION = "Irkutsk";

void postDataFloat(float value) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    http.begin(client, "http://" + String(SERVER_IP) + "/data/");  // HTTP
    http.addHeader("Content-Type", "application/json");

    JsonDocument data;
    data["device_id"] = DEVICE_ID;
    data["location"] = LOCATION;
    data["value"] = value;

    String stringData;
    serializeJson(data, stringData);
    int httpCode = http.POST(stringData);

    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] POST... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
  }
}

JsonDocument getData() {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;
  
    http.begin(client, "http://" + String(SERVER_IP) + "/data/" + String(LOCATION));  // HTTP
    http.addHeader("Content-Type", "application/json");

    int httpCode = http.GET();

    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      
      if (httpCode == HTTP_CODE_OK) {
        const String& payload = http.getString();
        Serial.println("received payload:\n<<");
        Serial.println(payload);
        Serial.println(">>");

        JsonDocument data;
        DeserializationError err = deserializeJson(data, payload);
  
        http.end();
        return data;
      }
    } else {
      Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }
  return JsonDocument();
}

double avg(JsonDocument data) {
  int length = data["values"].size();
  float avg = 0;
  for (int i = 0; i < length; i++) {
    float t = data["values"][i];
    avg += t;
  }
  if (length != 0) {
    avg /= length;
  }
  return avg;
}

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (Serial.available()) {
    String cmd = Serial.readString();
    char c = cmd.charAt(0);
    if (isdigit(c)) {
      postDataFloat(cmd.toFloat());
      Serial.println("Pushed: " + cmd);
    } else if (c == 'a') {
      JsonDocument data = getData();
      Serial.println(avg(data));
    } else {
      Serial.println("Unknown command");
      Serial.println("Input float value for pushing and 'a' for print avg value");
    }
  }
}
