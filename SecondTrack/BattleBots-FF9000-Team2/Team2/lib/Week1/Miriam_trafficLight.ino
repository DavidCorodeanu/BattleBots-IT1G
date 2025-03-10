const int RED_LED = 2;
const int BUTTON1 = 3;
const int YELLOW_LED = 8;
const int GREEN_LED = 9; 
void setup() {
  //setup code to run once:
  pinMode (RED_LED, OUTPUT);
  pinMode (YELLOW_LED, OUTPUT);
  pinMode (GREEN_LED, OUTPUT);
}

void loop() {
  sequence();
}

void sequence(){
  if(digitalRead(BUTTON1) == LOW) {
    digitalWrite (YELLOW_LED, HIGH);
    digitalWrite (RED_LED, LOW);
    delay (2000);
    digitalWrite (RED_LED, HIGH);
    digitalWrite (GREEN_LED, LOW);
    delay (2000);
    digitalWrite (GREEN_LED, HIGH);
    digitalWrite (YELLOW_LED, LOW);
    delay (2000);
  }
  else {
    digitalWrite (RED_LED, LOW);
    digitalWrite (GREEN_LED, HIGH);
    digitalWrite (YELLOW_LED, HIGH);
  }
}
