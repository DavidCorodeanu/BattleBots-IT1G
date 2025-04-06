int redLED = 11;
int BUTTON_1 = 3;
int BUTTON_2 = 5;

void RedBlinkSlow(int pin) {

  digitalWrite(pin, LOW);
  delay(500);
  digitalWrite(pin, HIGH);
  delay(500);

}

void RedBlinkFast(int pin) {

  digitalWrite(pin, LOW);
  delay(50);
  digitalWrite(pin, HIGH);
  delay(50);

}

bool Button1Pressed() {


  if (digitalRead(BUTTON_1) == LOW) {

    return true;

  }
  return false;
  
}

bool Button2Pressed() {


  if (digitalRead(BUTTON_2) == LOW) {

    return true;

  }
  return false;
  
}

void controlRED_LED() {

  if (Button1Pressed()) {

    RedBlinkFast(redLED);

  }
  if(Button2Pressed()) {

    RedBlinkSlow(redLED);

  }
  else{

    digitalWrite(redLED, LOW);

  }

}

void setup() {
  
  pinMode(redLED, OUTPUT);


}

void loop() {

  controlRED_LED();

}
