// Motor pins
const int motor_A2 = 10; // Left forward 
const int motor_A1 = 11; // Left backward 
const int motor_B2 = 5;  // Right backward 
const int motor_B1 = 6;  // Right forward 

// Line sensor array
const int sensorPins[] = {A0, A1, A2, A3, A4, A5, A6, A7};
int _sensorValues[8];
int _sensorThreshold = 600;
int _sensorWeights[] = { -3500, -2500, -1500, -500, 500, 1500, 2500, 3500 };

// Variables for PID control
float _Kp = 0.12;  // Proportional gain
float _Ki = 0.002; // Integral gain
float _Kd = 0.05;  // Derivative gain

int _previousError = 0;
int _P = 0;
int _integral = 0;
int _baseSpeed = 175;

// Ultrasonic sensor pins
const int TRIG_PIN = 7;  // Trig pin for ultrasonic sensor
const int ECHO_PIN = 8;  // Echo pin for ultrasonic sensor

// Distance threshold for object detection (in centimeters)
const int objectDistanceThreshold = 10;  // Object detected if closer than 10 cm

// State variable
bool startTriggered = false;  // Indicates if the robot has started line following

void setup() {
    Serial.begin(9600);

    // Motor pins setup
    pinMode(motor_A1, OUTPUT); 
    pinMode(motor_A2, OUTPUT); 
    pinMode(motor_B1, OUTPUT); 
    pinMode(motor_B2, OUTPUT);

    // Line sensor pins setup
    for (int i = 0; i < 8; i++) {  
        pinMode(sensorPins[i], INPUT);
    }

    // Ultrasonic sensor pins setup
    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
}   

void loop() {
    // Wait until the object is removed
    if (!startTriggered) {
        if (objectDetected()) {
            stopMotors();
            Serial.println("Waiting for object to be taken...");
            while (objectDetected()) {
                delay(100);
            }
            Serial.println("Object removed. Starting line following.");
        }
        startTriggered = true;
    }

    // Normal line-following code
    int position = readSensors();
    int error = position;
    PIDcontrol(error);
}

int readSensors() {
    int weightedSum = 0; // Sum of sensor weights
    int sum = 0;         // Number of active sensors

    for (int i = 0; i < 8; i++) {
        _sensorValues[i] = analogRead(sensorPins[i]);

        if (_sensorValues[i] > _sensorThreshold) {
            weightedSum += _sensorWeights[i];
            sum++;
        }
    }

    if (sum == 0) {
        // If no sensor detects the line, use the previous error’s sign
        return (_previousError > 0) ? 1000 : -1000;
    }

    return weightedSum / sum;
}

void PIDcontrol(int error) {
    _P = error;
    _integral += error;

    // Prevent integral windup
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

    if (leftSpeed >= 0) {
        analogWrite(motor_A2, leftSpeed);
        analogWrite(motor_A1, 0);
    } else {
        analogWrite(motor_A2, 0);
        analogWrite(motor_A1, abs(leftSpeed));
    }

    if (rightSpeed >= 0) {
        analogWrite(motor_B1, rightSpeed);
        analogWrite(motor_B2, 0);
    } else {
        analogWrite(motor_B1, 0);
        analogWrite(motor_B2, abs(rightSpeed));
    }
}

void moveForward() { 
    analogWrite(motor_A1, 0); 
    analogWrite(motor_A2, 170); 
    analogWrite(motor_B1, 170); 
    analogWrite(motor_B2, 0); 
} 

void turnRight() { 
    analogWrite(motor_A1, 0); 
    analogWrite(motor_A2, 170);  
    analogWrite(motor_B1, 0); 
    analogWrite(motor_B2, 170); 
} 

void turnLeft() { 
    analogWrite(motor_A1, 170); 
    analogWrite(motor_A2, 0); 
    analogWrite(motor_B1, 170);  
    analogWrite(motor_B2, 0); 
} 

void stopMotors() { 
    analogWrite(motor_A1, 0); 
    analogWrite(motor_A2, 0); 
    analogWrite(motor_B1, 0); 
    analogWrite(motor_B2, 0); 
}

// Function to check if the object is present using the ultrasonic sensor
bool objectDetected() {
    long distance = getDistance();
    Serial.print("Distance to object: ");
    Serial.print(distance);
    Serial.println(" cm");

    // Return true if an object is detected within the threshold
    return (distance > 0 && distance <= objectDistanceThreshold);
}

// Function to measure distance using the ultrasonic sensor
long getDistance() {
    // Clear the TRIG_PIN
    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    // Send a 10us pulse to TRIG_PIN
    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG_PIN, LOW);

    // Read the ECHO_PIN, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO_PIN, HIGH, 30000); // Timeout after 30ms (approx 5m)

    // If no echo is received, return -1
    if (duration == 0) {
        Serial.println("No echo received!");
        return -1;
    }

    // Calculate the distance (speed of sound is 343 m/s, or 0.0343 cm/us)
    // Distance = (duration / 2) * 0.0343
    long distance = duration * 0.0343 / 2;
    return distance;
}