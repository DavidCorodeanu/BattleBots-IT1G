// Define the analog pins for the line sensors
const int SENSORS[] = {A7, A6, A5, A4, A3, A2, A1, A0};

// Define the motor pins
const int MOTOR_A1 = 11;
const int MOTOR_A2 = 10;
const int MOTOR_B1 = 9;
const int MOTOR_B2 = 6;

// Motor speed levels
int motorSpeed = 80;  // Start slow
int maxSpeed = 180;   // Adjusted max speed
int speedIncrement = 5;  // Acceleration rate

// Fixed thresholds for detecting black and white lines
int blackThreshold = 600;  // Set based on your environment
int whiteThreshold = 400;  // Set based on your environment

bool robotRunning = true;  // Start robot immediately

// Function Prototypes (Declare Before Setup)
void stopMotors();
int getLinePosition(int sensorValues[]);
void applyLineFollowing(int position);
void setMotorSpeed(int leftSpeed, int rightSpeed);

void setup() {
  Serial.begin(9600);
  Serial.println("Robot ready.");

  // Set sensor pins as input
  for (int i = 0; i < 8; i++) {
    pinMode(SENSORS[i], INPUT);
  }

  // Set motor pins as output
  pinMode(MOTOR_A1, OUTPUT);
  pinMode(MOTOR_A2, OUTPUT);
  pinMode(MOTOR_B1, OUTPUT);
  pinMode(MOTOR_B2, OUTPUT);
}

void loop() {
  // If robot is stopped, do nothing
  if (!robotRunning) return;

  // Read sensor values
  int sensorValues[8];
  for (int i = 0; i < 8; i++) {
    sensorValues[i] = analogRead(SENSORS[i]);
  }

  // Calculate position
  int position = getLinePosition(sensorValues);

  // Apply motor control based on position error
  applyLineFollowing(position);

  delay(50);
}

int getLinePosition(int sensorValues[]) {
  int weightedSum = 0;
  int totalWeight = 0;

  for (int i = 0; i < 8; i++) {
    if (sensorValues[i] > blackThreshold) {  // If sensor detects black line
      weightedSum += (i - 4) * 10;  // Corrected indexing to center the weight
      totalWeight += 10;
    }
  }

  return (totalWeight > 0) ? (weightedSum / totalWeight) : 0;
}

void applyLineFollowing(int position) {
  int baseSpeed = motorSpeed;
  int turnSpeed = motorSpeed / 2;

  if (position < -3) {
    // Strong left turn
    setMotorSpeed(0, baseSpeed);
  } else if (position < -1) {
    // Slight left turn
    setMotorSpeed(turnSpeed, baseSpeed);
  } else if (position > 3) {
    // Strong right turn
    setMotorSpeed(baseSpeed, 0);
  } else if (position > 1) {
    // Slight right turn
    setMotorSpeed(baseSpeed, turnSpeed);
  } else {
    // Move forward
    setMotorSpeed(baseSpeed, baseSpeed);
  }
}

void setMotorSpeed(int leftSpeed, int rightSpeed) {
  analogWrite(MOTOR_A1, leftSpeed);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, rightSpeed);
  analogWrite(MOTOR_B2, 0);
}

void stopMotors() {
  analogWrite(MOTOR_A1, 0);
  analogWrite(MOTOR_A2, 0);
  analogWrite(MOTOR_B1, 0);
  analogWrite(MOTOR_B2, 0);
}



