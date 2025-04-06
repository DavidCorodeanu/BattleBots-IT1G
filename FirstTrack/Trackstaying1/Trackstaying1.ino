#define GRIPPER_OPEN 1850 
#define GRIPPER_CLOSE 1150 

const int SERVO = 12; // Define servo pin

// MOTOR A (Left)
const int MOTOR_A_1 = 11;
const int MOTOR_A_2 = 10;

// MOTOR B (Right)
const int MOTOR_B_1 = 6;
const int MOTOR_B_2 = 5;

// Speed settings (0-255)
const int BASE_SPEED = 175;

// Sensor pins
const int SENSOR_PINS[8] = {A0, A1, A2, A3, A4, A5, A6, A7};
int SENSOR_THRESHOLD = 600;
int blackValue = 1023;
int whiteValue = 0;

// PID control variables
float Kp = 0.12, Ki = 0.002, Kd = 0.05;
int previousError = 0, integral = 0;

// Forward declarations
int readSensors();
bool detectDoubleLine();
void PIDcontrol(int error);
void motor_control(int leftSpeed, int rightSpeed);
void stopMotors();
void turnLeft();
void moveServo(int pulse);

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_A_1, OUTPUT);
  pinMode(MOTOR_A_2, OUTPUT);
  pinMode(MOTOR_B_1, OUTPUT);
  pinMode(MOTOR_B_2, OUTPUT);
  pinMode(SERVO, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(SENSOR_PINS[i], INPUT);
  }

  // *Calibrate Sensors*
  Serial.println("Calibrating sensors...");
  delay(2000);

  for (int i = 0; i < 8; i++) {
    int value = analogRead(SENSOR_PINS[i]);
    if (value < blackValue) blackValue = value;
    if (value > whiteValue) whiteValue = value;
  }

  SENSOR_THRESHOLD = (blackValue + whiteValue) / 2 + 50;
  Serial.print("Black Value: "); Serial.println(blackValue);
  Serial.print("White Value: "); Serial.println(whiteValue);
  Serial.print("New SENSOR_THRESHOLD: "); Serial.println(SENSOR_THRESHOLD);

  stopMotors();
  moveServo(GRIPPER_OPEN); // Open gripper before detecting double lines
}

void loop() {
  int position = readSensors();
  int error = position;

  Serial.print("Position: "); Serial.println(position);
  Serial.print("Error: "); Serial.println(error);

  if (detectDoubleLine()) {
    Serial.println("P-Spot detected!");
    stopMotors();
    moveServo(GRIPPER_OPEN); // Open gripper
    delay(500);

    unsigned long startTime = millis();
    while (millis() - startTime < 2000) { // Move forward for 2 seconds
      motor_control(BASE_SPEED, BASE_SPEED);
    }
    stopMotors();
    delay(500);
    
    moveServo(GRIPPER_CLOSE); // Grab the cone
    delay(1000);
    
    turnLeft(); // Turn left after grabbing the cone
  } else {
    PIDcontrol(error);
  }
}

int readSensors() {
  int weightedSum = 0;
  int sum = 0;
  int sensorWeights[8] = { -3500, -2500, -1500, -500, 500, 1500, 2500, 3500 };

  for (int i = 0; i < 8; i++) {
    int sensorValue = analogRead(SENSOR_PINS[i]);
    Serial.print("Sensor "); Serial.print(i);
    Serial.print(": "); Serial.println(sensorValue);

    if (sensorValue > SENSOR_THRESHOLD) {
      weightedSum += sensorWeights[i];
      sum++;
    }
  }
  return sum == 0 ? (previousError > 0 ? 1000 : -1000) : weightedSum / sum;
}

bool detectDoubleLine() {
  int left = analogRead(SENSOR_PINS[2]);
  int right = analogRead(SENSOR_PINS[5]);
  Serial.print("Left sensor: "); Serial.println(left);
  Serial.print("Right sensor: "); Serial.println(right);

  return left > SENSOR_THRESHOLD && right > SENSOR_THRESHOLD;
}

void PIDcontrol(int error) {
  int P = error;
  integral += error;
  integral = constrain(integral, -1000, 1000);
  int D = error - previousError;
  int PIDvalue = (Kp * P) + (Ki * integral) + (Kd * D);
  previousError = error;

  Serial.print("PID Value: "); Serial.println(PIDvalue);

  int leftSpeed = BASE_SPEED - PIDvalue;
  int rightSpeed = BASE_SPEED + PIDvalue;
  motor_control(leftSpeed, rightSpeed);
}

void motor_control(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  Serial.print("Left Speed: "); Serial.println(leftSpeed);
  Serial.print("Right Speed: "); Serial.println(rightSpeed);

  analogWrite(MOTOR_A_1, leftSpeed < 0 ? abs(leftSpeed) : 0);
  analogWrite(MOTOR_A_2, leftSpeed > 0 ? leftSpeed : 0);

  analogWrite(MOTOR_B_1, rightSpeed > 0 ? rightSpeed : 0);
  analogWrite(MOTOR_B_2, rightSpeed < 0 ? abs(rightSpeed) : 0);
}

void stopMotors() {
  Serial.println("Stopping motors...");
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 0);
  analogWrite(MOTOR_B_1, 0);
  analogWrite(MOTOR_B_2, 0);
}

void turnLeft() {
  Serial.println("Turning Left...");
  analogWrite(MOTOR_A_1, 0);
  analogWrite(MOTOR_A_2, 150);
  analogWrite(MOTOR_B_1, 150);
  analogWrite(MOTOR_B_2, 0);
  delay(300);
  stopMotors();
}

void moveServo(int pulse) {
  Serial.print("Moving servo to pulse: "); Serial.println(pulse);
  digitalWrite(SERVO, HIGH);
  delayMicroseconds(pulse);
  digitalWrite(SERVO, LOW);
  delay(20);
}