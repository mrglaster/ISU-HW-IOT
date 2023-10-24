#include <PubSubClient.h>

PubSubClient mqtt_client(wifiClient);

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message is: ");
  char cmd = (char) payload[0];
  Serial.println(cmd);
  if (cmd == '0') {
    digitalWrite(LED_PIN, HIGH);
    Serial.print("CMD: Led is DOWN!"); 
  } else {
    digitalWrite(LED_PIN, LOW);
    Serial.print("CMD: Led is UP!");
  }
}

bool init_MQTT() {
  mqtt_client.setServer(mqtt_broker, mqtt_port); // connect to broker
  mqtt_client.setCallback(callback);
  while (!mqtt_client.connected()) {
    String client_id = "esp8266_138" + id();
    if (mqtt_client.connect(client_id.c_str())) {
      Serial.println("MQTT client connected with id " + client_id);
      Serial.println("Listening to topic: isu/tyt1/bulb1");
    } else {
      Serial.println("Failed to connect MQTT with id " + client_id);
      delay(500);
    }
  }
  return true;
}