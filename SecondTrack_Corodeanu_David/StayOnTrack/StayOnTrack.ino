//Include custom library files
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\PinConnections.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\Movement.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\Gripper.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack\Libraries\Detection.h>

void setup()
{
    // Initialize serial communication
    Serial.begin(9600);

    // Initialize the input and outputs
    pinMode(BLUETOOTH_TRANSMIT, OUTPUT);
    pinMode(NEOPIXEL_PIN, OUTPUT);
    pinMode(GRIPPER, OUTPUT);
    pinMode(MOTOR_A1_LEFT_FORWARD, OUTPUT);
    pinMode(MOTOR_A2_LEFT_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B1_RIGHT_BACKWARDS, OUTPUT);
    pinMode(MOTOR_B2_RIGHT_FORWARD, OUTPUT);
    pinMode(SONAR_SENSOR_TRIGGER, OUTPUT);
    pinMode(SONAR_SENSOR_ECHO, INPUT);
    declareLineSensorPins();

    // Initialize the outputs
    digitalWrite(MOTOR_A1_LEFT_FORWARD, HIGH);
    digitalWrite(MOTOR_A2_LEFT_BACKWARDS, HIGH);
    digitalWrite(MOTOR_B1_RIGHT_BACKWARDS, HIGH);
    digitalWrite(MOTOR_B2_RIGHT_FORWARD, HIGH);
    digitalWrite(GRIPPER, LOW);
}

void loop()
{
    followLine();
}

void declareLineSensorPins()
{
    /*
    For each sensor in the array set the pin as an input
    && is used to avoid copying the value with each iteration
    */
    for (auto &&sensor : LINE_SENSORS)
    {
        pinMode(sensor, INPUT);
    }
}

void followLine() 
{
    while (true) 
    {
        int leftSensor = analogRead(LINE_SENSORS[2]); // Read the left sensor value
        int rightSensor = analogRead(LINE_SENSORS[5]); // Read the right sensor value
        int error = leftSensor - rightSensor; // Calculate the error
        int derivative = error - lastError; // Store the current error for next iteration
        lastError = error; // Update lastError for the next loop iteration
        int correction = (Kp * error) + (Kd * derivative);  // Compute the correction using proportional (Kp) and derivative (Kd) control

        int leftSpeed = constrain(baseSpeed - correction, 0, 255); // Reduce left speed if turning right
        int rightSpeed = constrain(baseSpeed + correction, 0, 255); // Increase right speed if turning right

        drive(leftSpeed, 0, rightSpeed, 0); // Drive the motors with the calculated speeds
    }
}