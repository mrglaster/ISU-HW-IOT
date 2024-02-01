#define PIN_LATCH 11
#define PIN_CLK 10 
#define PIN_DATA  12 

int current_number = 0;

byte digits[10] = {
  0b11111100, //0
  0b01100000, //1
  0b11011010, //2
  0b11110010, //3
  0b01100110, //4
  0b10110110, //5
  0b10111110, //6
  0b11100000, //7
  0b11111110, //8
  0b11110110 //9
};

void setup() {
  pinMode(PIN_LATCH, OUTPUT);
  pinMode(PIN_DATA, OUTPUT);  
  pinMode(PIN_CLK, OUTPUT);
  digitalWrite(PIN_CLK, LOW);

  Serial.begin(9600);
}

void loop() {
  if (Serial.available()) {
    char user_input = Serial.read(); 
    if (user_input == 'r'){
      for (int i = 0; i < 10; i++){
        show_number(i);
        delay(500);
      }

    } else {
      int digit = (int)(user_input - '0');
      show_number(digit);
    }
  }
}

void show_number(int digit) {
  digitalWrite(PIN_LATCH, LOW);
  if(digit < 0 or digit > 9) {
    return;
  }
  shiftOut(PIN_DATA, PIN_CLK, LSBFIRST, digits[digit]);
  digitalWrite(PIN_LATCH, HIGH);
}
