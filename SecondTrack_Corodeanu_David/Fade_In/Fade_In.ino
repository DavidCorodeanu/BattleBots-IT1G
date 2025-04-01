const int RED_LED = 13; 
int brightness = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (brightness = 0; brightness <= 255; brightness +=5){
    analogWrite(RED_LED, brightness);
    delay(30);
  }
}
