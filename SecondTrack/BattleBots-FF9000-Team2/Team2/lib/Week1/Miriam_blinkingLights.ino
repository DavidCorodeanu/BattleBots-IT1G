const int LED = 2;
const int BUTTON1 = 3;
// the setup function runs once when you press reset or power the board
void setup() {
// initialize digital pin LED_BUILTIN as an output.
  pinMode(LED, OUTPUT);
  pinMode(BUTTON1, INPUT);
}

// the loop function runs over and over again forever
void loop() {
  buttonPressed();        
}
void buttonPressed(){
  if(digitalRead(BUTTON1) == LOW) {
    digitalWrite(LED, HIGH);
    delay (100);       
    digitalWrite(LED, LOW);
    delay (100);   
  }
  else {
    digitalWrite(LED, HIGH);
    delay (300);       
    digitalWrite(LED, LOW);
    delay (300);       
  }
}