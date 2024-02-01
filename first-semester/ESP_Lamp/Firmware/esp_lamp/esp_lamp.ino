#include "Config.h"
#include "Wifi.h"
#include "MQTT.h"
#include "Server.h"

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  bool is_ap_on = start_AP_MODE();
  // bool is_wifi_on = init_WIFI(false);
  if (is_ap_on) {
    server_init();
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  if (mqtt_client.connected()) {
    mqtt_client.loop();
  }
  delay(20);
}
