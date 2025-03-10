#include <Arduino.h>

// Define pins
const int       BLUETOOTH_TRANSMIT= 1;
const int       MOTOR_A_BACKWARD = 10;
const int       MOTOR_A_FORWARD = 3;
const int       MOTOR_B_FORWARD = 6;
const int       MOTOR_B_BACKWARD = 5;
const int       SONAR_SENSOR_ECHO = 8;
const int       SONAR_SENSOR_TRIGGER = 7;

// Define state variables for the millis
unsigned long   _lastTime = 0;
unsigned long   _startMillis = 0;
bool            _avoidObject = false;

// Calibration offsets
const int CALIBRATION_OFFSET_A = 10; // Adjust this value as needed
const int CALIBRATION_OFFSET_B = 5;  // Adjust this value as needed

// Define the function prototype
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward);
void buttonPress();
int calibrate(int n, int offset);
long readSonarSensor();

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize the input and outputs
    pinMode(BLUETOOTH_TRANSMIT, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);

    // Initialize the outputs
    digitalWrite(MOTOR_A_BACKWARD, HIGH);
    digitalWrite(MOTOR_A_FORWARD, HIGH);
    digitalWrite(MOTOR_B_FORWARD, HIGH);
    digitalWrite(MOTOR_B_BACKWARD, HIGH);
}

void loop()
{
    unsigned long currentMillis = millis();

    if (_avoidObject && currentMillis - _startMillis >= 1000)
    {
        _avoidObject = false;
    }
    else if (!_avoidObject)
    {
        long distance = readSonarSensor();
        if (distance < 15)
        {
            Serial.print("Obstacle detected at distance: ");
            Serial.print(distance);
            Serial.println(". Avoiding object...");
            Serial.println("Turn left");
            drive(255, 0, 0, 255);
            delay(500);

            Serial.println("Go forward");
            drive(255, 0, 255, 0);
            delay(1000);

            Serial.println("Turn right");
            drive(0, 255, 255, 0);
            delay(500);

            Serial.println("Go forward");
            drive(255, 0, 255, 0);
            delay(1000);

            Serial.println("Turn right");
            drive(0, 255, 255, 0);
            delay(500);

            Serial.println("Go forward");
            drive(255, 0, 255, 0);
            delay(1000);

            Serial.println("Turn left");
            drive(255, 0, 0, 255);
            delay(500);
            _startMillis = currentMillis;
            _avoidObject = true;
        }
        else
        {
            drive(255, 0, 255, 0); // Drive forward
        }
    }

}

long readSonarSensor()
{
    // Send a pulse to trigger the sonar sensor
    digitalWrite(SONAR_SENSOR_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(SONAR_SENSOR_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR_SENSOR_TRIGGER, LOW);

    // Read the echo pin and calculate the distance
    long duration = pulseIn(SONAR_SENSOR_ECHO, HIGH);
    return duration * 0.034 / 2; // Convert duration to distance in cm by using the speed of sound (0.034 cm per microsecond)
    // This does not convert to actual centimeters because of hardware compatibility issues or some other factors
}

// Drive with calibration
void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward)
{
    analogWrite(MOTOR_A_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_A_BACKWARD, calibrate(motorABackward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_B_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_B));
    analogWrite(MOTOR_B_BACKWARD, calibrate(motorBBackward, CALIBRATION_OFFSET_B));
}

int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    return (calibratedValue < 0) ? 0 : calibratedValue; // Ensure the value doesn't go below 0
}