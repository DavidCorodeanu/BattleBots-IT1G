#include <Adafruit_NeoPixel.h>
// Include all custom libraries - these can be found in the Libraries folder
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

    // Set motor control pins as output and stop motors
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

    // Attach Interruptions for Encoders
    attachInterrupt(digitalPinToInterrupt(MOTOR_R1), leftEncoderISR, CHANGE);
    attachInterrupt(digitalPinToInterrupt(MOTOR_R2), rightEncoderISR, CHANGE);

    //Set Servo
    pinMode(SERVO, OUTPUT);
    digitalWrite(SERVO, LOW);
}

void loop()
{
    if(!robotCalibrated) // Indicate standby status using LEDs if the robot is not yet calibrated
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

    // If robot is following the line, check for obstacles ahead
    if (robotState == FOLLOW_LINE && gameStarted && !gameEnded)
    {
        float distance = measureDistance();

        // If obstacle detected within 15 cm, turn around
        if (distance < 15)
        {
            turn180(255, 255);
            return;  // Skip the rest of the loop to start turning
        }
    }

    // Control the gripper based on whether the cone has been picked up
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
        // Start calibration if a robot is detected for at least 300ms
        static unsigned long detectionStartTime = 0;

        if (measureDistance() < 30)
        {
            if (detectionStartTime == 0)
            {
                detectionStartTime = millis();  // Start timing
            }
            else if (millis() - detectionStartTime >= 300)
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

    // Start the game after calibration and picking up the cone
    if (sensorsCalibrated && !gameStarted && conePickedUp)
    {
        turnLeftMillis(105); // Initial turn to begin movement
        if (robotState != FOLLOW_LINE) return; // Ensure the robot is in the FOLLOW_LINE state before proceeding
        gameStarted = true;
    }

    // If game is ongoing, follow the line based on sensor input
    if (gameStarted && !gameEnded)
    {
        getLinePosition(); // Update current line position

        switch (linePosition)
        {
            case T_JUNCTION:
                turnLeftMillis(90); // Handle T-junction by turning left
                break;

            case LEFT_LINE:
                turnLeftMillis(105); // Turn left slightly to correct path
                break;

            case NO_LINE:
                turnAroundMillis(); // If robot has lost the line, turn around
                break;

            case RIGHT_LINE:
                robotState = FOLLOW_LINE;
                moveForwardPID(baseSpeed, baseSpeed, false, true);  // Slight right correction
                break;

            case CENTER_LINE:
                if (robotState == FOLLOW_LINE)
                {
                    moveForwardPID(baseSpeed, baseSpeed, false, true); // Continue forward
                    setDriveForwardColor(); // Update LED color
                }
                break;

            default:
                // Handle unexpected line positions, if necessary
                break;
        }
    }
}