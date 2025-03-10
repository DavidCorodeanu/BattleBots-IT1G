// Motor control pins 
const int MOTOR_A_FORWARD = 3;
const int MOTOR_A_BACKWARD = 10;
const int MOTOR_B_FORWARD = 6;
const int MOTOR_B_BACKWARD = 5;

// Time tracking variable
unsigned long startTime;

const int CALIBRATION_OFFSET_A = 10;
const int CALIBRATION_OFFSET_B = 5;

void setup() {
  Serial.begin(9600); // Initialize Serial Monitor communication for debugging
  pinMode(MOTOR_A_FORWARD, OUTPUT);//put it in a state
  pinMode(MOTOR_A_BACKWARD, OUTPUT);
  pinMode(MOTOR_B_FORWARD, OUTPUT);
  pinMode(MOTOR_B_BACKWARD, OUTPUT);
  
  startTime = millis();  // Start the timer
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - startTime < 3000) {
    Serial.println("Go forward");
    drive(255, 0, 255, 0);
  }
  else if (currentMillis - startTime < 5000) {
    Serial.println("Turn right");
    drive(160, 0, 255, 0);
  }
  else if (currentMillis - startTime < 8000) {
    Serial.println("Go backward");
    drive(0, 255, 0, 255);
  }
  else if (currentMillis - startTime < 10000) {
    Serial.println("Turn left");
    drive(255, 0, 160, 0);
  }
  else {
    Serial.println("Stop");
    drive(0, 0, 0, 0); // Stop motors
  }
}

// Function to control the motors
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward) {
  analogWrite(MOTOR_A_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_A));
  analogWrite(MOTOR_A_BACKWARD, calibrate(motorABackward, CALIBRATION_OFFSET_A));
  analogWrite(MOTOR_B_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_B));
  analogWrite(MOTOR_B_BACKWARD, calibrate(motorBBackward, CALIBRATION_OFFSET_B));
}

// Calibration function
int calibrate(int n, int offset) {
  int calibratedValue = n - offset;
  return (calibratedValue < 0) ? 0 : calibratedValue;
}
