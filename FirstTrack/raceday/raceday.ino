#include <Adafruit_NeoPixel.h>

#define NeoLed 13
#define NUM_PIXELS 4
#define OPEN 150
#define CLOSE 30
// --- Motor Pin Definitions ---
const int motor_A2 = 10; // Left forward 
const int motor_A1 = 11; // Left backward 
const int motor_B2 = 5;  // Right backward 
const int motor_B1 = 6;  // Right forward 

// --- Servo ---
const int servoPin = 12;

// --- Line Sensor Definitions ---
const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int _sensorValues[8];
int _sensorThreshold = 600;  // This value will be updated by calibration
int _sensorWeights[] = { -3500, -2500, -1500, -500, 500, 1500, 2500, 3500 };

// --- PID Control Variables ---
float _Kp = 0.12;  
float _Ki = 0.002; 
float _Kd = 0.05;  

int _previousError = 0;
int _P = 0;
int _integral = 0;
int _baseSpeed = 255;

// --- Ultrasonic Sensor Definitions ---
#define ECHO_PIN 8  // Echo pin (input)
#define TRIG_PIN 7  // Trigger pin (output)
const int objectDistanceThreshold = 10; // in centimeters

Adafruit_NeoPixel strip(NUM_PIXELS, NeoLed, NEO_GRB + NEO_KHZ800);
// -- avoid --
int avoidTimeCheck = 0;
int timeAvoidCheck = 200;



// Flags for start trigger and calibration
bool startTriggered = false;
bool isCalibrating = false;
bool isCalibrated = false;
unsigned long calibrationStartTime = 0;
const unsigned long calibrationDuration = 1500; // in milliseconds

// Variables for debouncing black square detection
unsigned long blackSquareDetectedTime = 0;
const unsigned long blackSquareDebounce = 300; // ms required for valid detection

// Variables to detect when the black square ends
bool inBlackSquareState = false;      // true if robot was in a black square
unsigned long squareEndTime = 0;        // timer for confirming square end
const unsigned long squareEndDebounce = 100; // ms to confirm the square has ended

// --- Function Prototypes ---
void moveForward();
void stopMotors();
void motor_control(int leftSpeed, int rightSpeed);
int readSensors();
void PIDcontrol(int error);
int calibrate();    // Now returns an int value for _sensorThreshold
int getDistance();
void turnLeft();
void doStartRoutine();
void isBlackSquare();  // Function to detect if all sensors are over black
void writeServo(int angle);
void setup() {
  Serial.begin(9600);
  
  // Setup motor pins
  pinMode(motor_A1, OUTPUT);
  pinMode(motor_A2, OUTPUT);
  pinMode(motor_B1, OUTPUT);
  pinMode(motor_B2, OUTPUT);
  
  // Setup line sensor pins
  for (int i = 0; i < 8; i++) {
    pinMode(sensorPins[i], INPUT);
  }
  
  // Setup ultrasonic sensor pins
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  strip.begin();
  setLEDColor(0,0,0);
  strip.show();

  // Ensure robot is stationary at startup
  stopMotors();
  writeServo(OPEN);
  delay(100);

}

void loop() {
  // --- Wait for the Start Trigger (flag removal) ---
  if (!startTriggered) {
    stopMotors();
    int dist = getDistance();
    Serial.print("Distance: ");
    Serial.println(dist);
    
    // When the object is removed (flag raised)
    if (dist > objectDistanceThreshold) {
      // Start the calibration routine (non-blocking with millis)
      analogWrite(motor_A1, 0);
      analogWrite(motor_A2, 230);
      analogWrite(motor_B1, 250);
      analogWrite(motor_B2, 0);
      delay(1200);
      writeServo(CLOSE);
      turnLeft();
      delay(750);
      analogWrite(motor_A1, 0);
      analogWrite(motor_A2, 170);
      analogWrite(motor_B1, 170);
      analogWrite(motor_B2, 0);
      delay(400);
      
          isCalibrated = true;
          isCalibrating = false;
          startTriggered = true;
      
    } else {
      Serial.println("Waiting for object removal...");
      delay(100);  // Short delay to avoid excessive polling
      return;
    }
    
    // Exit loop until calibration is done
    if (!isCalibrated) return;
  }
  
  // --- Normal Line Following Code with Black Square Detection (No Turn) ---
  
  // Update sensor readings
  for (int i = 0; i < 8; i++){
    _sensorValues[i] = analogRead(sensorPins[i]);
  }
  
  // Check for black square (for logging purposes only)
  isBlackSquare();
  avoidObject();
  
  // Continue with normal line following
  int position = readSensors();
  int error = position;
  PIDcontrol(error);
}

// --- Functions ---

int readSensors() {
  int weightedSum = 0;
  int sum = 0;
  
  for (int i = 0; i < 8; i++) {
    if (_sensorValues[i] > _sensorThreshold) {
      weightedSum += _sensorWeights[i];
      sum++;
    }
  }
  
  if (sum == 0) {
    return (_previousError > 0) ? 1000 : -1000;
  }
  return weightedSum / sum;
}

void PIDcontrol(int error) {
  _P = error;
  _integral += error;
  _integral = constrain(_integral, -1000, 1000);
  int D = error - _previousError;
  int PIDvalue = (_Kp * _P) + (_Ki * _integral) + (_Kd * D);
  _previousError = error;
  
  int leftSpeed = _baseSpeed - PIDvalue;
  int rightSpeed = _baseSpeed + PIDvalue;
  motor_control(leftSpeed, rightSpeed);
}

void motor_control(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  // Control left motor
  if (leftSpeed >= 0) {
    analogWrite(motor_A2, leftSpeed);
    analogWrite(motor_A1, 0);
  } else {
    analogWrite(motor_A2, 0);
    analogWrite(motor_A1, abs(leftSpeed));
  }
  
  // Control right motor
  if (rightSpeed >= 0) {
    analogWrite(motor_B1, rightSpeed);
    analogWrite(motor_B2, 0);
  } else {
    analogWrite(motor_B1, 0);
    analogWrite(motor_B2, abs(rightSpeed));
  }
}

void setLEDColor(int g, int r, int b){
  for(int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(g, r, b));
  }
  strip.show();
}

void moveForward() {
  analogWrite(motor_A1, 0);
  analogWrite(motor_A2, 170);
  analogWrite(motor_B1, 170);
  analogWrite(motor_B2, 0);

  setLEDColor(255, 0, 0); // Green LED for forward
}

void stopMotors() {
  analogWrite(motor_A1, 0);
  analogWrite(motor_A2, 0);
  analogWrite(motor_B1, 0);
  analogWrite(motor_B2, 0);

  setLEDColor(50, 255, 0); // Green LED for forward
}

// Calibration: sets _sensorThreshold based on current sensor readings
int calibrate() {
  int minReading = 1023;
  int maxReading = 0;
  
  for (int i = 0; i < 8; i++) {
    _sensorValues[i] = analogRead(sensorPins[i]);
    if (_sensorValues[i] < minReading) {
      minReading = _sensorValues[i];
    }
    if (_sensorValues[i] > maxReading) {
      maxReading = _sensorValues[i];
    }
  }
  _sensorThreshold = (minReading + maxReading) / 2;
  
  Serial.print("Calibration complete. Min: ");
  Serial.print(minReading);
  Serial.print(" Max: ");
  Serial.print(maxReading);
  Serial.print(" Threshold: ");
  Serial.println(_sensorThreshold);
  
  return _sensorThreshold;
}

// Ultrasonic distance measurement function
int getDistance() {
  long duration;
  int distance;
  
  digitalWrite(TRIG_PIN, LOW);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;
  
  return distance;
}

// Left turn maneuver (only used at startup)
void turnLeft() {
  analogWrite(motor_A1, 200);  // Left motor backward
  analogWrite(motor_A2, 0);
  analogWrite(motor_B1, 200);  // Right motor forward
  analogWrite(motor_B2, 0);

  setLEDColor(255, 0, 0); // Green LED for forward
}

// Debounced black square detection function
void isBlackSquare() {
  bool allSensorsBlack = true;
  for (int i = 0; i < 8; i++) {
    if (_sensorValues[i] < _sensorThreshold) {
      allSensorsBlack = false;
      break;
    }
  }
  
  if (allSensorsBlack) {
    if (blackSquareDetectedTime == 0) {
      blackSquareDetectedTime = millis();
    }
    if (millis() - blackSquareDetectedTime >= blackSquareDebounce) {
      motor_control(-255, -255);
      delay(200);
      writeServo(OPEN);
      motor_control(-255, -255);
      delay(2000);
      while (true) {
        writeServo(OPEN);
        delay(50);
        stopMotors();
      }
    }
  } else {
    blackSquareDetectedTime = 0;
  }
  
}
void writeServo(int angle) {
  int pulseWidth = map(angle, 0, 180, 1000, 2000); // Convert angle to microseconds

  digitalWrite(servoPin, HIGH);
  unsigned long start = micros();
  while (micros() - start < pulseWidth); // Wait for pulse width
  digitalWrite(servoPin, LOW);
}

void avoidObject() {
  if (millis() - avoidTimeCheck >= timeAvoidCheck) {
    avoidTimeCheck = millis();
    if (getDistance() < 15) {
      motor_control(255, 0);
      delay(400);
      motor_control(255, 255);
      delay(550);
      motor_control(0, 255);
      delay(750);
      motor_control(255, 255);
      delay(400);
      motor_control(0, 255);
      delay(500);
      //motor_control(255, 255);
      //delay(200);
      motor_control(255, 0);
      delay(400);
    }
  }
}