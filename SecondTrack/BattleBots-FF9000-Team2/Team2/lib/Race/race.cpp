#include <Arduino.h>
#include <declare.cpp>
#include <drive.cpp>
#include <sonarSensor.cpp>

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
    unsigned long   currentMillis = millis();

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
            Serial.println("Turning left");
            turnLeft();
            delay(500);
            Serial.println("Driving forward");
            driveForward();
            delay(1000);
            Serial.println("Turning right");
            turnRight();
            delay(500);
            Serial.println("Driving forward");
            driveForward();
            delay(1000);
            Serial.println("Turning right");
            turnRight();
            delay(500);
            Serial.println("Driving forward");
            driveForward();
            delay(1000);
            Serial.println("Turning left");
            turnLeft();
            delay(500);
            currentMillis = millis();
            _startMillis = currentMillis;
            _avoidObject = true;
        }
        else
        {
            driveForward();
        }
    }
}