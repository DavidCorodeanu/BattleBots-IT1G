//Assign pins
const int RED_LED = 13;
const int GREEN_LED = 11;
const int YELLOW_LED= 12;
const int BUTTON = 10;
bool _buttonState = false;

void setup() {
  //assign pin modes
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
 
  //set initial LED states
  digitalWrite(GREEN_LED,HIGH);
  digitalWrite(YELLOW_LED,HIGH);
}

void loop() {
  _buttonState = digitalRead(BUTTON); // Read the state of the button
 if (_buttonState == LOW) {
  digitalWrite (RED_LED,HIGH);    //Making the light to switch from red to green for 4 seconds then to yellow 1 second then back to red until the button is pressed again
  digitalWrite (GREEN_LED, LOW);
  delay (4000);
  digitalWrite (GREEN_LED, HIGH);
  digitalWrite (YELLOW_LED, LOW);
  delay (1000);
  digitalWrite (YELLOW_LED, HIGH);
  digitalWrite (RED_LED, LOW);
 }
}
