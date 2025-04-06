int redLED = 10;
int yellowLED = 9;
int greenLED = 11;
int buttonPin = 2; // the number of the pushbutton pin
int buttonState = 0; // variable for reading the pushbutton status

void setup() {
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // if button is not pressed, follow traffic light sequence:
  if (buttonState == LOW) {
    digitalWrite(greenLED, HIGH);
    delay(5000);
    digitalWrite(greenLED, LOW);

    digitalWrite(yellowLED, HIGH);
    delay(2000);
    digitalWrite(yellowLED, LOW);

    digitalWrite(redLED, HIGH);
    delay(5000);
    digitalWrite(redLED, LOW);
  } else {
    // turn off all LEDs if button is pressed
    digitalWrite(greenLED, LOW);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
  }
}



