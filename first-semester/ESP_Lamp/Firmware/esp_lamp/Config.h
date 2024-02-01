String AP_NAME = "Esp_LAMP"; // AP - Access Point
String AP_PASSWORD = "ESP8266_138";
const int LED_PIN = 2;

// CLI WiFi client
const char* CLI_SSID = "Redmi 9T"; // turn phone 5 GHz -> 2.4 GHz
const char* CLI_PASS = "7253beadc76a";

// Access login and password
const char* ACCESS_LOGIN = "user";
const char* ACCESS_PASSWORD = "user2022";

int WEB_SERVER_PORT = 80;
bool mqtt_started = false;
const int mqtt_port = 1883;
char* mqtt_broker = "broker.emqx.io";