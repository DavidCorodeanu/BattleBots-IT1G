#include <Arduino.h>
#include <declare.cpp>
#include <debugLight.cpp>
#include <drive.cpp>
#include <sonarSensor.cpp>
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

    if (currentMillis - _lastTime < 1000)
    {
        gripperSetup();
    }
    else if (currentMillis - _lastTime >= 1000 && currentMillis - _lastTime < 1300)
    {
        Serial.println("Close gripper");
        closeGripper();
    }
    else if (currentMillis - _lastTime >= 1300 && currentMillis - _lastTime < 2300)
    {
        Serial.println("Drive forward");
        driveForward();
    }
    else if (currentMillis - _lastTime >= 2300 && currentMillis - _lastTime < 3300)
    {
        Serial.println("Turn right");
        turnRight();
    }
    else if (currentMillis - _lastTime >= 3300 && currentMillis - _lastTime < 4300)
    {
        Serial.println("Turn left");
        turnLeft();
    }
    else if (currentMillis - _lastTime >= 4300 && currentMillis - _lastTime < 5300)
    {
        Serial.println("Drive backward");
        driveBackward();
    }
    else if (currentMillis - _lastTime >= 5300 && currentMillis - _lastTime < 6300)
    {
        Serial.println("Stop");
        driveStop();
    }
    else if (currentMillis - _lastTime >= 6300 && currentMillis - _lastTime < 6600)
    {
        Serial.println("Open gripper");
        openGripper();
    }
    else if (currentMillis - _lastTime >= 6600)
    {
        Serial.println("Reset");
        setStandByColor();
        _lastTime = currentMillis;
    }
}
