//Pin connections
const int TRIG_PIN = 13;  
const int ECHO_PIN = 12; 
const int MOTOR_A1 = 11;  
const int MOTOR_A2 = 10;  
const int MOTOR_B1 = 9;  
const int MOTOR_B2 = 3;   

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, HIGH); 
  pinMode(ECHO_PIN, INPUT);
  digitalWrite(ECHO_PIN, HIGH); 
  
  pinMode(MOTOR_A1, OUTPUT);
  digitalWrite(MOTOR_A1, HIGH); 
  pinMode(MOTOR_A2, OUTPUT);
  digitalWrite(MOTOR_A2, HIGH); 
  pinMode(MOTOR_B1, OUTPUT);
  digitalWrite(MOTOR_B1, HIGH); 
  pinMode(MOTOR_B2, OUTPUT);
  digitalWrite(MOTOR_B2, HIGH); 
 
  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  // Send a pulse to the TRIG_PIN
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  // Read the echo duration
  duration = pulseIn(ECHO_PIN, HIGH);
   // Calculate the distance in cm
  distance = (duration / 2) / 29.1;


  // Check if an object is detected within 15 cm
  if (distance < 15) {
    stopMotors();   // Stop the motors
    delay(500);     // Wait for half a second
    turnRight();    // Turn right
    delay(360);     // Turn for half a second
    moveForward();  // Move forward
    delay(1000);
    turnLeft();     // Turn left
    delay(360);
    moveForward();  // Move forward
    delay(1000);
    turnLeft();     // Turn left
    delay(360);
    moveForward();  // Move forward
    delay(1000);
    turnRight();    // Turn right
    delay(300);
  } else {
    // If no object is detected, keep moving forward
    moveForward();
  }
  delay(100); // Short delay before the next loop iteration
}

void stopMotors() {
   // Set motor speeds to 0 to stop motors
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

void moveForward() {
  // Set motor speeds to move forward
  analogWrite(MOTOR_A1, 255); // 100% duty cycle
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 245); // ~95% duty cycle
  analogWrite(MOTOR_B2, 0);
}

void reverseMotors() {
  // Set motor speeds to move backward
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 255); // 100% duty cycle
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 245); // ~95% duty cycle
}

void turnRight() {
   // Set motor speeds to turn right
  analogWrite(MOTOR_A1, 255); // 100% duty cycle
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 245); // ~95% duty cycle
}

void turnLeft() {
   // Set motor speeds to turn left
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 255); // 100% duty cycle
  analogWrite(MOTOR_B1, 245); // ~95% duty cycle
  analogWrite(MOTOR_B2, 0); 
}

