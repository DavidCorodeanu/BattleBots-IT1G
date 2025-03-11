//Include custom library files
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\PinConnections.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\Movement.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\Gripper.h>

void setup()
{   
    // Initialize the inputs and outputs
    pinMode(BLUETOOTH_TRANSMIT, OUTPUT);
    pinMode(NEOPIXEL_PIN, OUTPUT);
    pinMode(MOTOR_A1_LEFT_FORWARD, OUTPUT);
    pinMode(MOTOR_A2_LEFT_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B1_RIGHT_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B2_RIGHT_FORWARD, OUTPUT);
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);

    // Set the initial motor states to HIGH
    digitalWrite(MOTOR_A1_LEFT_FORWARD, HIGH);
    digitalWrite(MOTOR_A2_LEFT_BACKWARDS, HIGH);
    digitalWrite(MOTOR_B1_RIGHT_BACKWARDS, HIGH);
    digitalWrite(MOTOR_B2_RIGHT_FORWARD, HIGH);
}

void loop()
{
    unsigned long currentMillis = millis(); // Get the current time in milliseconds

    // Execute different actions based on the time passed
    if (currentMillis - _lastTime < 1000)
    {
        gripperSetup(); // Initialize the gripper(opened)
    }
    else if (currentMillis - _lastTime >= 1000 && currentMillis - _lastTime < 1500)
    {
        driveForward(); // Move forward for 0.5s
    }
    else if (currentMillis - _lastTime >= 1500 && currentMillis - _lastTime < 2500)
    {
        driveStop(); // Stop the robot
        closeGripper(); // Close the gripper
    }
    else if (currentMillis - _lastTime >= 2500 && currentMillis - _lastTime < 50000)
    {
        driveForward(); // Continue moving forward 
        closeGripper(); //while keeping the gripper closed
    }
    
}
