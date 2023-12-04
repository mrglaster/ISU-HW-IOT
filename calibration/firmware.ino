#define US_TRIG 2
#define US_ECHO 3
#define IR_OUT  A0

long duration, cm;


void setup() {
  pinMode(US_TRIG, OUTPUT);
  pinMode(US_ECHO, INPUT);
  pinMode(IR_OUT, INPUT);
  Serial.begin(9600);
}

long get_us_distance(){
  digitalWrite(US_TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(US_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(US_TRIG, LOW);
  duration = pulseIn(US_ECHO, HIGH);
  return (duration / 2) / 29.1;
}



void loop() {

  if (Serial.available()){
    char cmd = Serial.read();
      if (cmd == 'n'){
                  long us_distance = get_us_distance();
                  Serial.print("Got value: ");
                  Serial.print(us_distance);
                  Serial.print(" cm");
                  Serial.println(" ");
              // } else {
              //   ir_distances[current_measurment - 1 - end_distance] = get_us_distance();
              // }
        } else if (cmd == 'i') {
          long us_distance = analogRead(IR_OUT);
          Serial.print("Got value: ");
          Serial.print(us_distance);
          Serial.print(" cm");
          Serial.println(" ");
        }
      }
}
