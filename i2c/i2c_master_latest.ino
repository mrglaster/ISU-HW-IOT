/**I2C Master Firmware*/
#include <Wire.h>

#define PHOTO_RES_IN A0
#define SLAVE_ADDRES 9
#define RESPONSE_SIZE 1


bool is_calibration = false;
bool is_report_sended = false;
bool data_send_aborted = false;
bool showData = false;
bool minRangeChecked = false;
bool maxRangeChecked = false;
int min_range = -1;
int max_range = -1;

int previous = -1;
int cntr = 0;
int records_to_do = 0;


const int STEP = 10;
const int DIFF = 10;
const int ARCHIVE_FACTOR = 5;

const int epsilon = 3;


/**Receives valuse from the photoresistor*/
int getMyPhotoResValue(){
  return analogRead(PHOTO_RES_IN);
}

/**Wire & Serial initialization, Pin mode configuration*/
void setup() {
  pinMode(PHOTO_RES_IN, INPUT);
  Serial.begin(9600);
  Serial.println("Hello! I am Master MC!");
  Wire.begin(D1, D2);
}

/**The loop*/
void loop() {
   if (showData) {
      Serial.print("My source value: ");
      Serial.println(getMyPhotoResValue());
    }

  if (Serial.available()) {
    char cmd = Serial.read();

     if (cmd == 's') {
      showData = !showData;
    }

    if (cmd == 'c') {
      Serial.println("Turn off the light and input 'o' for confirmation");
    }
    if (cmd == 'r'){
      is_calibration=false;
      is_report_sended = false;
      data_send_aborted = false;
      min_range = -1;
      max_range = -1;
      maxRangeChecked = false;
      minRangeChecked = false;
      previous = -1;
      cntr = 0;
      records_to_do=0;
      Serial.println("Calibration has been reset by Master");
    }
    if (cmd == 'o') {
      if (min_range == -1) {
        min_range = getMyPhotoResValue() + DIFF;
        Serial.println("Turn on the light and input 'o' for confirmation");
      } else if (max_range == -1) {
        max_range = getMyPhotoResValue() - DIFF;
        is_calibration = true;

        Wire.beginTransmission(SLAVE_ADDRES);
        Wire.write('r');
        records_to_do  = (int)((max_range - min_range + 1) / STEP);
        Wire.write(records_to_do);
        Serial.print("Records to do: ");

        Serial.println(records_to_do);


        Wire.endTransmission(SLAVE_ADDRES);
        Serial.println(min_range);
        Serial.println(max_range);
        Serial.println("Calibration started. Start from turning off the light and slowly add more light every time");
        delay(5000); // Чтобы пользовател  прочитал и успел отреагировать
      }
    }
  }
  if (is_calibration) {
    Wire.beginTransmission(SLAVE_ADDRES);
    if (!is_report_sended) {
      Wire.write("c");
      is_report_sended = true;
    }
    if (is_report_sended && !data_send_aborted){
      int res = getMyPhotoResValue();
      if (res <= min_range && !minRangeChecked) {
        minRangeChecked = true;
      }
      if (res >= min_range && !maxRangeChecked) {
        maxRangeChecked = true;
      }


      if (res - previous >= STEP){
        Serial.print("Value for calibration: ");
        Serial.println(getMyPhotoResValue());
        Wire.write((int)(res / ARCHIVE_FACTOR));
        cntr += 1;
        previous = res;
      }
      Wire.endTransmission(SLAVE_ADDRES);

      if (previous == -1){
        previous = res;
      }
    }
    if (minRangeChecked && maxRangeChecked && cntr >= records_to_do ) {
      is_calibration=false;
      is_report_sended = false;
      data_send_aborted = false;

      min_range = -1;
      max_range = -1;

      Wire.beginTransmission(SLAVE_ADDRES);
      Wire.write('s');
      Wire.endTransmission(SLAVE_ADDRES);
      Serial.println("Calibration finished");
    }
  }
 //delay(100);
}

