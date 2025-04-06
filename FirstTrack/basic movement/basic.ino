#define MOTOR_A1_FORWARD 5
#define MOTOR_A2_BACKWARD 3
#define MOTOR_B1_FORWARD 2
#define MOTOR_B2_BACKWARD 4

void setup() {
  pinMode(MOTOR_A1_FORWARD, LOW);
  pinMode(MOTOR_A2_BACKWARD, LOW);
  pinMode(MOTOR_B1_FORWARD, HIGH);
  pinMode(MOTOR_B2_BACKWARD, HIGH);
  digitalWrite(MOTOR_A1_FORWARD, LOW);
}

void loop() {
  moveForward();
  delay(2000);
  stopBot();
  delay(1000);

  moveBackward();
  delay(2000);
  stopBot();
  delay(1000);

  turnLeft();
  delay(1000);
  stopBot();
  delay(1000);

  turnRight();
  delay(1000);
  stopBot();
  delay(1000);

  moveLeft();
  delay(1000);
  stopBot();
  delay(1000);

  moveRight();
  delay(1000);
  stopBot();
  delay(1000);
}

void moveForward() {
  digitalWrite(MOTOR_A1_FORWARD, HIGH);
  digitalWrite(MOTOR_B1_FORWARD, HIGH);
  digitalWrite(MOTOR_A2_BACKWARD, LOW);
  digitalWrite(MOTOR_B2_BACKWARD, LOW);
}

void moveBackward() {
  digitalWrite(MOTOR_A2_BACKWARD, HIGH);
  digitalWrite(MOTOR_B2_BACKWARD, HIGH);
  digitalWrite(MOTOR_A1_FORWARD, LOW);
  digitalWrite(MOTOR_B1_FORWARD, LOW);
}

void turnLeft() {
  digitalWrite(MOTOR_A1_FORWARD, LOW);
  digitalWrite(MOTOR_A2_BACKWARD, HIGH);
  digitalWrite(MOTOR_B1_FORWARD, HIGH);
  digitalWrite(MOTOR_B2_BACKWARD, LOW);
}

void turnRight() {
  digitalWrite(MOTOR_A1_FORWARD, HIGH);
  digitalWrite(MOTOR_A2_BACKWARD, LOW);
  digitalWrite(MOTOR_B1_FORWARD, LOW);
  digitalWrite(MOTOR_B2_BACKWARD, HIGH);
}

void moveLeft() {
  digitalWrite(MOTOR_A1_FORWARD, LOW);
  digitalWrite(MOTOR_A2_BACKWARD, HIGH);
  digitalWrite(MOTOR_B1_FORWARD, LOW);
  digitalWrite(MOTOR_B2_BACKWARD, LOW);
}

void moveRight() {
  digitalWrite(MOTOR_A1_FORWARD, LOW);
  digitalWrite(MOTOR_A2_BACKWARD, LOW);
  digitalWrite(MOTOR_B1_FORWARD, HIGH);
  digitalWrite(MOTOR_B2_BACKWARD, LOW);
}

void stopBot() {
  digitalWrite(MOTOR_A1_FORWARD, LOW);
  digitalWrite(MOTOR_A2_BACKWARD, LOW);
  digitalWrite(MOTOR_B1_FORWARD, LOW);
  digitalWrite(MOTOR_B2_BACKWARD, LOW);
}
