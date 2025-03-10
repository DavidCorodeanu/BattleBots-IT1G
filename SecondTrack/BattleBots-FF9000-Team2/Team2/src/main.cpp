#include <Arduino.h>
#include <declare.cpp>
#include <debugLight.cpp>
#include <drive.cpp>
#include <sonarSensor.cpp>
#include <lineSensor.cpp>
#include <gripper.cpp>

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize the NeoPixel
    NeoPixel.begin();
    
    // Initialize the input and outputs
    pinMode(BLUETOOTH_TRANSMIT, OUTPUT);
    pinMode(NEOPIXEL_PIN, OUTPUT);
    pinMode(GRIPPER, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);
    declareLineSensorPins();

    // Initialize the outputs
    digitalWrite(MOTOR_A_BACKWARD, HIGH);
    digitalWrite(MOTOR_A_FORWARD, HIGH);
    digitalWrite(MOTOR_B_FORWARD, HIGH);
    digitalWrite(MOTOR_B_BACKWARD, HIGH);
    digitalWrite(GRIPPER, LOW);
}

void loop()
{
    followLine();
}