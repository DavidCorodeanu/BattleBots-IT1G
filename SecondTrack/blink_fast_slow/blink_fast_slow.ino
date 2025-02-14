//declare the components
const int RED_LED = 13; 
const int BUTTON_1 = 10;
const int BUTTON_2 = 9;

bool _buttonState1 = false;
bool _buttonState2 = false;

//function to blink fast
void blinkFast () {
    digitalWrite(RED_LED, LOW);
    delay(100);
    digitalWrite(RED_LED, HIGH);
    delay(100);  
}

//function to blink slow
void blinkSlow () {
    digitalWrite(RED_LED, LOW);
    delay(3000);
    digitalWrite(RED_LED, HIGH);
    delay(3000);
}

//function to blink normal
void blinkNormal() {
    digitalWrite(RED_LED, LOW);
    delay(1000);
    digitalWrite(RED_LED, HIGH);
    delay(1000);
}


void setup() {
  //set components type
  pinMode(RED_LED, OUTPUT);
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
}

void loop() {
  //select buttons
  _buttonState1 = digitalRead(BUTTON_1);
  _buttonState2 = digitalRead(BUTTON_2);
 
  //set button1 to blink fast and button2 to slow, while button is not pressed blink normal
  if (_buttonState1 == LOW ) {
    blinkFast ();
  } 
  else if (_buttonState2 == LOW) {
    blinkSlow();
  }
  else {
    blinkNormal();
  }
}
