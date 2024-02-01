#define FIRST_ROW 8
#define SECOND_ROW 9
#define FIRST_COL 10
#define SECOND_COL 11

int firstRowConditions[] = {0, 1};
int secondRowConditions[] = {1, 0};

void setup() {
  pinMode(FIRST_COL, INPUT_PULLUP);
  pinMode(SECOND_COL, INPUT_PULLUP);
  pinMode(FIRST_ROW, OUTPUT);
  pinMode(SECOND_ROW, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int buttonStates[4] = {0, 0, 0, 0};

  for (int i = 0; i < 2; i++){
      digitalWrite(FIRST_ROW, firstRowConditions[i]);
      digitalWrite(SECOND_ROW, secondRowConditions[i]);

      for (int button = 0; button < 4; button++) {
        int rowState = digitalRead(button < 2 ? FIRST_ROW : SECOND_ROW);
        int colState = digitalRead(button % 2 == 0 ? FIRST_COL : SECOND_COL);
        if (rowState == 0 && colState == 0) {
          buttonStates[button] = 1;
        }
      }

      delay(100);
  }

  bool wasFirstPartPressed = false;
  for (int i = 0; i < 4; i++) {
    if (buttonStates[i] == 1) {
      if (!wasFirstPartPressed){
         Serial.print("Pressed button(s): ");
         wasFirstPartPressed = true;
      }
      Serial.print(i + 1);
      Serial.print(" ");
    }
  }
  if (wasFirstPartPressed){
    Serial.println(" ");
  }
  delay(100);
}
