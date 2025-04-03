#include <Adafruit_NeoPixel.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\PinConnections.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\LineSensor.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\Gripper.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\debugLight.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\Movement.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\Detection.h>
#include <C:\School\NHL_Stenden_PHP_Docker_Env-0.4 Group IT1D\app\public\BattleBots-IT1G\SecondTrack_Corodeanu_David\Libraries\lineSeonsorCalibration.h>

void setup()
{
    Serial.begin(9600);
    NeoPixel.begin();
    NeoPixel.setBrightness(40);

    // Set Motor Pins
    pinMode(MOTOR_A1_FORWARD, OUTPUT);
    pinMode(MOTOR_A2_BACKWARD, OUTPUT);
    pinMode(MOTOR_B2_FORWARD, OUTPUT);
    pinMode(MOTOR_B1_BACKWARD, OUTPUT);
    digitalWrite(MOTOR_A1_FORWARD, LOW);
    digitalWrite(MOTOR_A2_BACKWARD, LOW);
    digitalWrite(MOTOR_B2_FORWARD, LOW);
    digitalWrite(MOTOR_B1_BACKWARD, LOW);
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);

    // Attach Interrupts for Encoders
    attachInterrupt(digitalPinToInterrupt(MOTOR_R1), leftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MOTOR_R2), rightEncoderISR, CHANGE);

    //SERVO
    pinMode(SERVO, OUTPUT);
    digitalWrite(SERVO, LOW);
}

void loop()
{
    if(!robotCalibrated)
    {
        setStandByColor();
    }

    int unsigned currentTime = millis();
    // Check if the game has ended
    if (gameEnded)
    {

        if (currentTime - previousTime >= GRIPPER_INTERVAL)
        {
            previousTime = currentTime;
            openGripper();  // Keep the gripper open to drop the cone
        }
        return;  // Skip the rest of the loop
    }

    // Check for obstacles (only when following line, not during turns)
    if (robotState == FOLLOW_LINE && gameStarted && !gameEnded)
    {
        float distance = measureDistance();

        // If obstacle detected within 12cm, turn around
        if (distance < 15)
        {
            turn180(255, 255);
            return;  // Skip the rest of the loop to start turning
        }
    }

    // Check if the gripper should be opened or closed based on the conePickUp state
    if (conePickedUp)
    {
        if (currentTime - previousTime >= GRIPPER_INTERVAL)
        {
            previousTime = currentTime;
            closeGripper();
        }
    }
    else
    {
        if (currentTime - previousTime >= GRIPPER_INTERVAL)
        {
            previousTime = currentTime;
            openGripper();
        }
    }

    // Check if the cone is in the square and if the robot is not calibrated
    if (coneInSquare && !sensorsCalibrated)
    {
        // Start calibrating the line sensors when a robot is detected for 200ms
        static unsigned long detectionStartTime = 0;

        if (measureDistance() < 30)
        {
            if (detectionStartTime == 0)
            {
                detectionStartTime = millis();  // Start timing
            }
            else if (millis() - detectionStartTime >= 350)
            {
                robotDetected = true;
                if (robotDetected)
                {
                    calibrateSensors();
                    robotCalibrated = true;  // Set the robot as calibrated
                }
                detectionStartTime = 0;  // Reset the timer after calibration
            }
        }
        else
        {
            detectionStartTime = 0;  // Reset the timer if no robot is detected
        }
        return;  // Skip the rest of the loop to allow for calibration
    }

    // Check if the sensors are calibrated and if the cone is not picked up
    if (sensorsCalibrated && !conePickedUp)
    {
        //pickUpCone
        conePickedUp = true;
        return;
    }

    // Check if the sensors are calibrated and if the game has not started yet
    if (sensorsCalibrated && !gameStarted && conePickedUp)
    {
        // Start the game by turning left for 90 milliseconds
        turnLeftMillis(105);
        if (robotState != FOLLOW_LINE) return; // Ensure the robot is in the FOLLOW_LINE state before proceeding
        gameStarted = true;
    }

    // Check if the game has started and if the game has not ended yet
    if (gameStarted && !gameEnded)
    {
        // Get the line position and take action based on it
        getLinePosition();

        switch (linePosition)
        {
            case T_JUNCTION:
                turnLeftMillis(90);
                break;

            case LEFT_LINE:
                turnLeftMillis(105);
                break;

            case NO_LINE:
                turnAroundMillis();
                break;

            case RIGHT_LINE:
                robotState = FOLLOW_LINE;
                moveForwardPID(baseSpeed, baseSpeed, false, true);
                break;

            case CENTER_LINE:
                if (robotState == FOLLOW_LINE)
                {
                    moveForwardPID(baseSpeed, baseSpeed, false, true);
                    setDriveForwardColor();
                }
                break;

            default:
                // Handle unexpected line positions, if necessary
                break;
        }
    }
}