/**I2C Slave Firmware*/
#include <Wire.h>

#define PHOTO_RES_IN A0
#define I2C_ADDRES 9
// #define MAX_RECORDS 10
#define LED_PIN 13

const int STEP = 10;
int max_records = 0;
int min_range = -1;
int max_range = -1;
int currentRecord = 0;
int* x;
int* y;
// int* x_slave;
// int* y_slave;
bool calibrationStarted = false;
bool showData = false;
bool recordingRange = false;

/**Receives valuse from the photoresistor*/
int getMyPhotoResValue(){
  return analogRead(PHOTO_RES_IN);
}

/**Wire & Serial initialization, Pin mode configuration*/
void setup() {
  pinMode(LED_PIN, OUTPUT);
  Serial.begin(9600);
  Serial.println("Hello! I am the Slave MC!");
  Wire.begin(I2C_ADDRES);
  Wire.onReceive(processData);
}


float linearInterpolation(int x_new) {
    int i = 0;
    while (i < currentRecord - 1 && x[i + 1] < x_new) {
        i++;
    }
    int x0 = x[i];
    int y0 = y[i];
    int x1 = x[i + 1];
    int y1 = y[i + 1];
    return y0 + (y1 - y0) * (x_new - x0) / (x1 - x0);
}

long lastTime = 0;
bool state = HIGH;
/**The loop*/
void loop() {
  if (calibrationStarted) {
      blinkLed(lastTime, state);
    }
  if (Serial.available()) {
    char cmd = Serial.read();
    if (cmd == 'r') {
      int currentRecord = 0;
      calibrationStarted = false;
    }
    if (cmd == 's') {
      showData = !showData;
    }
  }
  if (showData) {
    if (max_records != 0) {
      Serial.print("My calibrated value: ");
      Serial.println(linearInterpolation(getMyPhotoResValue()));
    } else {
      Serial.print("My value: ");
      Serial.println(getMyPhotoResValue());
    }
  }
  // delay(100);
}


/**Receives other microcontroller's values via I2C and prints the result*/
void processData(int length){
  int last_val = 0;
  while (0 < Wire.available()){
    auto result = Wire.read();
    if (result == 'r') {
      max_records = Wire.read();
      Serial.print("max_records");
      Serial.println(max_records);
      x = new int[max_records];
      y = new int[max_records];
    } else
    if (result == 's') {
      Serial.println("Calibration finished");
      calibrationStarted = false;
      digitalWrite(LED_PIN, HIGH);
      return;
    } else 
    if (result == 'c') {
      currentRecord = 0;
      calibrationStarted = true;
      Serial.println("Calibration has started!");
    } else {
      if (currentRecord < max_records && calibrationStarted){
        int data = result * 5;
        int currentResistence = getMyPhotoResValue();
        x[currentRecord] = currentResistence;
        y[currentRecord] = data;
        currentRecord+=1;
        Serial.println("Calibrating...");
       } //else if (recordingRange) {
      //   recordingRange = false;
      //   max_records = (max_range - min_range) / STEP;
      //   delete[] x, y;
      //   x = new int[max_records];
      //   y = new int[max_records];
      // }
  }
}
}

void blinkLed(long &lastTime, bool &state) {
  long currentTime = millis();
  if (currentTime - lastTime > 100) {
    digitalWrite(LED_PIN, state);
    lastTime = currentTime;
    state = !state;
  }
}