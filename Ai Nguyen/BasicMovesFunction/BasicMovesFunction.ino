// Pin connection
const int MOTOR_A1 = 11;  // PWM pin
const int MOTOR_A2 = 10;  // PWM pin
const int MOTOR_B1 = 9;   // PWM pin
const int MOTOR_B2 = 3;   // PWM pin
const int BUTTON_1 = 7;   // Button 1 (to move forward and backward)
const int BUTTON_2 = 4;   // Button 2 (to rotate right or left)
const int BUTTON_3 = 2;   // Button 3 (to move forward)

// Variables to store button states
bool _buttonState1 = false;
bool _lastButtonState1 = false;
bool _buttonState2 = false;
bool _lastButtonState2 = false;
bool _buttonState3 = false;
bool _lastButtonState3 = false;
bool _robotMoving = false;

void setup() {
  pinMode(MOTOR_A1, OUTPUT);
  digitalWrite(MOTOR_A1, HIGH);
  pinMode(MOTOR_A2, OUTPUT);
  digitalWrite(MOTOR_A2, HIGH);
  pinMode(MOTOR_B1, OUTPUT);
  digitalWrite(MOTOR_B1, HIGH);
  pinMode(MOTOR_B2, OUTPUT);
  digitalWrite(MOTOR_B2, HIGH);
  pinMode(BUTTON_1, INPUT);
  digitalWrite(BUTTON_1, HIGH);
  pinMode(BUTTON_2, INPUT);
  digitalWrite(BUTTON_2, HIGH);
  pinMode(BUTTON_3, INPUT);
  digitalWrite(BUTTON_3, HIGH);
}

void loop() {
  // Read button states
  _buttonState1 = digitalRead(BUTTON_1);
  _buttonState2 = digitalRead(BUTTON_2);
  _buttonState3 = digitalRead(BUTTON_3);

  // Toggle robot moving state when Button 3 is pressed
  if (_buttonState3 != _lastButtonState3) {
    if (_buttonState3 == LOW) {
      _robotMoving = !_robotMoving;
      if (_robotMoving) {
        moveForward(255, 128);  // Left wheel at 100% power, Right wheel at 50% power
      } else {
        stopMotors();
      }
    }
    _lastButtonState3 = _buttonState3;
    delay(50);  // Debounce delay
  }

  // Move forward and backward when Button 1 is pressed
  if (_buttonState1 != _lastButtonState1) {
    if (_buttonState1 == LOW) {
      moveForward(255, 245);  // Move forward
      delay(3000);
      moveBackwards(255, 245);  // Move backwards
      delay(3000);
      stopMotors();
    }
    _lastButtonState1 = _buttonState1;
    delay(50);  // Debounce delay
  }

  // Rotate when Button 2 is pressed
  if (_buttonState2 != _lastButtonState2) {
    if (_buttonState2 == LOW) {
      rotate90DegreesRight(255);  // Rotate 90 degrees to the right at full speed
      delay(1000);  // Short delay between rotations
      rotate90DegreesLeft(255);  // Rotate 90 degrees to the left at full speed
    }
    _lastButtonState2 = _buttonState2;
    delay(50);  // Debounce delay
  }
}

void moveForward(int leftSpeed, int rightSpeed) {
  // Set motor speeds to move forward
  analogWrite(MOTOR_A1, leftSpeed);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, rightSpeed);
  analogWrite(MOTOR_B2, 0);
}

void stopMotors() {
  // Set motor speeds to 0 to stop motors
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}

void moveBackwards(int leftSpeed, int rightSpeed) {
  // Set motor speeds to move backwards
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, leftSpeed);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, rightSpeed);
}

void rotate90DegreesRight(int speed) {
  // Rotate 90 degrees to the right
  analogWrite(MOTOR_A1, speed);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, speed);
  delay(500);  // Adjust the delay for a 90-degree rotation
  stopMotors();
}

void rotate90DegreesLeft(int speed) {
  // Rotate 90 degrees to the left
  analogWrite(MOTOR_A2, speed);
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_B1, speed);
  analogWrite(MOTOR_B2, 0);
  delay(500);  // Adjust the delay for a 90-degree rotation
  stopMotors();
}

