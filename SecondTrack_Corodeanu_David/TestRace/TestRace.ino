#include <Adafruit_NeoPixel.h>

enum RobotState             {FOLLOW_LINE, TURNING, TURNING_LEFT, TURNING_RIGHT, TURNING_AROUND, CHECKING_FOR_PATH_AHEAD};
enum LinePosition           {T_JUNCTION, LEFT_LINE, RIGHT_LINE, NO_LINE, CENTER_LINE};

RobotState                  robotState = FOLLOW_LINE;
LinePosition                linePosition = CENTER_LINE;

//Line Sensors
const int                   NUM_SENSORS = 8;                                               // Number of sensors
int                         sensorPins[NUM_SENSORS] = { A0, A1, A2, A3, A4, A5, A6, A7 };  // Sensor pin mapping
int                         sensorValues[NUM_SENSORS];                                     // Array to store sensor readings
int                         sensorMin[NUM_SENSORS];
int                         sensorMax[NUM_SENSORS];
int                         sensorThreshold[NUM_SENSORS];                                  // Array to store thresholds for each sensor

//Line Positions
bool                        leftTurn;
bool                        rightTurn;
bool                        tJunctionOrBase;
bool                        deadEnd;

//Measurements
const float                 WHEEL_CIRCUMFERENCE = 20.4;
const int                   PULSE_PER_REVOLUTION = 20;
const float                 DISTANCE_BETWEEN_WHEELS = 22.75;
static const int            DISTANCE_FROM_BASE_TO_CONE = 55; // Distance is in ticks
const int                   TARGET = DISTANCE_FROM_BASE_TO_CONE;

const int                   TRIG = 13;
const int                   ECHO = 12;

// Encoder Pulse Counters
volatile signed int         _leftTicks = 0;
volatile signed int         _rightTicks = 8;

// Define NeoPixel
const int                   NEOPIXEL_PIN = 4;
const int                   NUM_PIXELS = 4;
const int                   PIXEL_BOTTOM_LEFT = 0;
const int                   PIXEL_BOTTOM_RIGHT = 1;
const int                   PIXEL_TOP_RIGHT = 2;
const int                   PIXEL_TOP_LEFT = 3;
Adafruit_NeoPixel           NeoPixel = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);

// Motor Pins
const int                   MOTOR_B_FORWARD = 11;   // Left Forward
const int                   MOTOR_B_BACKWARD = 10;   // Left Reverse
const int                   MOTOR_A_FORWARD = 5;  // Right Forward
const int                   MOTOR_A_BACKWARD = 6;   // Right Reverse
int                         baseSpeed = 255;

//Servo Control
const int                   GRIPPER_OPEN = 1800;
const int                   GRIPPER_CLOSE = 990;
const int                   SERVO = 9;
const int                   PULSE = 2000;
int                         previousTime = 0;
const int                   GRIPPER_INTERVAL = 20;

//Rotation Sensors
const int                   MOTOR_R1 = 2;
const int                   MOTOR_R2 = 3;

const int                   ISR_INTERVAL = 20;  // interval of 20 milli seconds to update counter by interupt

// Conditions
bool                        coneInSquare = true;
bool                        sensorsCalibrated = false;
bool                        conePickedUp = false;
bool                        gameStarted = false;
bool                        coneDroppedOff = false;
bool                        gameEnded = false;
bool                        motionComplete = true;
bool                        robotDetected = false;
bool                        blackSquareDetected = false;
bool                        robotCalibrated = false;

// Setup the minimum time to confirm a black square
const int                   MIN_SQUARE_TIME = 2000;  // Minimum time in ms to confirm we're on a black square

// PID Variables
int                         error = 0, lastError = 0;
float                       integral = 0;
float                       derivative = 0;
float                       Kp;  // Proportional Gain
float                       Ki;  // Integral Gain
float                       Kd;  // Derivative Gain
int                         correction;

// Robot variables
int                         pulses;
int                         angle;
int                         radius = DISTANCE_BETWEEN_WHEELS;
int                         turn_Circumference = 2 * 3.14 * radius;
float                       turnDistances = 0;  // ARC of a circle

// function to reset the encoder ticks
void resetTicks()
{
    _leftTicks = 0;
    _rightTicks = 0;
}

void setStandByColor()
{
    static unsigned long previousMillis = 0;
    static bool isOn = false;
    const unsigned long interval = 500;

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        isOn = !isOn;

        if (isOn)
        {
            NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
            NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
            NeoPixel.show();
        }
        else
        {
            NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
            NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
            NeoPixel.show();
        }
        NeoPixel.show();
    }
}

void setDriveForwardColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setDriveBackwardColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setTurnRightColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 100, 0);
    NeoPixel.show();
}

void setTurnLeftColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setTurnAroundColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 100, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 100, 0);
    NeoPixel.show();
}

void setDriveStopColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setCalibrateColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 85, 255);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 85, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 85, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 85, 255);
    NeoPixel.show();
}

// Function to measure distance using ultrasonic sensor
float measureDistance()
{
    // Clear the TRIG pin
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    // Set the TRIG pin HIGH for 10 microseconds
    digitalWrite(TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIG, LOW);

    // Read the ECHO pin, returns the sound wave travel time in microseconds
    long duration = pulseIn(ECHO, HIGH, 30000);  // Timeout after 30ms

    // Calculate the distance
    float distance = duration * 0.034 / 2;  // Speed of sound wave divided by 2 due to the echo

    // Filter out unreliable readings
    if (distance == 0 || distance > 200)
    {
        return 999;  // Return a large value if no echo received or out of range
    }

    return distance;
}

void endGame()
{
    // Only end game if we're not already ending it
    if (!gameEnded)
    {
        // Stop the robot
        stopMotors();
        long startTime = millis();
        while (millis() < startTime + 1000)
        {
            openGripper(); // Open gripper
            setDriveBackwardColor();
            moveBackward(200, 200);  // Move backward while dropping the cone
        }
        
        // Stop the robot after driving backwards
        stopMotors();
        setDriveStopColor();

        // Set flags to indicate game is over
        coneDroppedOff = true;
        gameEnded = true;

        // Ensure we can't accidentally start again
        motionComplete = true;
    }
}

/**
 *  Controls the gripper servo motor by generating a PWM signal.
 *
 *  This function generates a PWM signal to control a servo motor connected to the gripper.
 *  It updates the PWM signal at a fixed interval of 20 milliseconds.
 *
 *  The pulse width in microseconds to control the servo position. If the pulse
 *  width is greater than 0, it updates the last pulse width. If the pulse width
 *  is 0 or less, it uses the last pulse width.
 **/

 void gripper(int pulse)
 {
     static unsigned long timer;
     static int lastPulse;
     if (millis() > timer)
     {
         if (pulse > 0)
         {
             lastPulse = pulse;
         }
         else
         {
             pulse = lastPulse;
         }
 
         digitalWrite(SERVO, HIGH);
         delayMicroseconds(pulse);
         digitalWrite(SERVO, LOW);
         timer = millis() + 20;
     }
 }

void openGripper()
{
    gripper(GRIPPER_OPEN);
}

void closeGripper()
{
    gripper(GRIPPER_CLOSE);
}

void readSensors()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensorValues[i] = analogRead(sensorPins[i]);
    }
}

// Function to calculate the line position based on sensor values
int calculateLinePosition()
{
    long weightedSum = 0;
    long sum = 0;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int value = sensorValues[i];

        // Determine if the sensor is on the line
        weightedSum += (long)value * i * 1000;
        sum += value;
    }

    return weightedSum / sum;  // No line detected
}

// Initialize the endGame() function to get it in scope
void endGame();

void getLinePosition()
{
    readSensors();
    // Compare sensor values to thresholds to determine line position
    leftTurn = sensorValues[4] > sensorThreshold[4] && sensorValues[5] > sensorThreshold[5] && sensorValues[6] > sensorThreshold[6] && sensorValues[7] > sensorThreshold[7] && sensorValues[0] < sensorThreshold[0] && sensorValues[1] < sensorThreshold[1] && sensorValues[2] < sensorThreshold[2];
    rightTurn = sensorValues[5] < sensorThreshold[5] && sensorValues[6] < sensorThreshold[6] && sensorValues[7] < sensorThreshold[7] && sensorValues[0] > sensorThreshold[0] && sensorValues[1] > sensorThreshold[1] && sensorValues[2] > sensorThreshold[2];
    tJunctionOrBase = sensorValues[0] > sensorThreshold[0] && sensorValues[1] > sensorThreshold[1] && sensorValues[2] > sensorThreshold[2] && sensorValues[3] > sensorThreshold[3] && sensorValues[4] > sensorThreshold[4] && sensorValues[5] > sensorThreshold[5] && sensorValues[6] > sensorThreshold[6] && sensorValues[7] > sensorThreshold[7];
    deadEnd = sensorValues[0] < sensorThreshold[0] && sensorValues[1] < sensorThreshold[1] && sensorValues[2] < sensorThreshold[2] && sensorValues[3] < sensorThreshold[3] && sensorValues[4] < sensorThreshold[4] && sensorValues[5] < sensorThreshold[5] && sensorValues[6] < sensorThreshold[6] && sensorValues[7] < sensorThreshold[7];

    // Black square detection with 0.2 second timing requirement
    static unsigned long blackSquareStartTime = 0;
    static bool potentialBlackSquare = false;
    static int blackSquareOffset = 50;
    
    // Check if all sensors detect black (same as tJunctionOrBase)
    bool allSensorsBlack = sensorValues[0] > (sensorThreshold[0] + blackSquareOffset) && sensorValues[1] > (sensorThreshold[1] + blackSquareOffset) && sensorValues[2] > (sensorThreshold[2] + blackSquareOffset) && sensorValues[3] > (sensorThreshold[3] + blackSquareOffset) && sensorValues[4] > (sensorThreshold[4] + blackSquareOffset) && sensorValues[5] > (sensorThreshold[5] + blackSquareOffset) && sensorValues[6] > (sensorThreshold[6] + blackSquareOffset) && sensorValues[7] > (sensorThreshold[7] + blackSquareOffset);

    // Start timing when black square first detected
    if (allSensorsBlack && !potentialBlackSquare)
    {
        blackSquareStartTime = millis();
        potentialBlackSquare = true;
    }

    // Check if we've been on black square for at least 150ms
    if (allSensorsBlack && potentialBlackSquare && (millis() - blackSquareStartTime >= 150))
    {
        blackSquareDetected = true;
        if (!gameEnded)
        {
            endGame();
        }
    }

    // Reset if no longer on black square
    if (!allSensorsBlack)
    {
        potentialBlackSquare = false;
    }

    // When the motion is complete, lock the state of the line position
    if (motionComplete)
    {
        if (leftTurn)
        {
            linePosition = LEFT_LINE;
        }
        else if (rightTurn)
        {
            linePosition = RIGHT_LINE;
        }
        else if (deadEnd)
        {
            linePosition = NO_LINE;
        }
        else if (tJunctionOrBase && !blackSquareDetected)
        {
            linePosition = T_JUNCTION;
        }
        else
        {
            linePosition = CENTER_LINE;
        }
    }
}

void calibrateSensors()
{
    static bool firstRun = true;
    readSensors();
    setCalibrateColor();

    // Initialize sensor min/max values on the first run
    if (firstRun)
    {
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorMin[i] = 1023;  // Set min to max ADC value
            sensorMax[i] = 0;     // Set max to min ADC value
        }
        firstRun = false;
    }

    // Check if target distance is reached
    if (_leftTicks > TARGET || _rightTicks > TARGET)
    {
        stopMotors();
        sensorsCalibrated = true;

        // Calculate and store threshold values
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorThreshold[i] = (sensorMin[i] + sensorMax[i]) / 2;
            Serial.print("Threshold [");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(sensorThreshold[i]);
            Serial.println(sensorMin[i]);
            Serial.println(sensorMax[i]);
        }
        return;
    }

    // Read sensor values and update min/max
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int sensorValue = analogRead(sensorPins[i]);

        if (sensorValue < sensorMin[i])
        {
            sensorMin[i] = sensorValue;
        }
        if (sensorValue > sensorMax[i])
        {
            sensorMax[i] = sensorValue;
        }
    }

    moveForwardPID(200, 200, true, false);
}

void leftEncoderISR()
{
    static unsigned long timer;
    if (millis() > timer)
    {
        _leftTicks++;
        timer = millis() + ISR_INTERVAL;
    }
}

void rightEncoderISR()
{
    static unsigned long timer;
    if (millis() > timer)
    {
        _rightTicks++;
        timer = millis() + ISR_INTERVAL;
    }
}

void stopMotors()
{
    analogWrite(MOTOR_B_FORWARD, 0);
    analogWrite(MOTOR_B_BACKWARD, 0);
    analogWrite(MOTOR_A_FORWARD, 0);
    analogWrite(MOTOR_A_BACKWARD, 0);
}

void moveForward(int _rightSpeed, int _leftSpeed)
{
    if (!gameEnded)
    {
        analogWrite(MOTOR_B_FORWARD, _rightSpeed);
        analogWrite(MOTOR_A_FORWARD, _leftSpeed);
    }
}

void moveBackward(int _rightSpeed, int _leftSpeed)
{
    if (!gameEnded)
    {
        analogWrite(MOTOR_B_BACKWARD, _rightSpeed);
        analogWrite(MOTOR_A_BACKWARD, _leftSpeed);
    }
}

void turn180(int _leftSpeed, int _rightSpeed)
{
    analogWrite(MOTOR_B_BACKWARD, 0);
    analogWrite(MOTOR_A_FORWARD, 0);
    analogWrite(MOTOR_B_FORWARD, _rightSpeed);
    analogWrite(MOTOR_A_BACKWARD, _leftSpeed);
}

void turnLeftMillis(int angle)
{
    static unsigned long lastCheck = 0;
    const unsigned long checkInterval = 5;
    static int targetPulses;

    if (robotState != TURNING_LEFT)
    {
        resetTicks();
        targetPulses = 0;
        float turnDistance = (angle / 360.0) * turn_Circumference;
        targetPulses = (turnDistance / WHEEL_CIRCUMFERENCE) * PULSE_PER_REVOLUTION;

        stopMotors();

        moveForward(0, 200);
        setTurnLeftColor();
        robotState = TURNING_LEFT;  //Lock state to "TURNING_LEFT"
        motionComplete = false;
    }

    if (robotState == TURNING_LEFT)
    {
        lastCheck = millis();
        if (_rightTicks >= targetPulses)
        {
            stopMotors();
            robotState = FOLLOW_LINE;  // Unlock state after turn is complete
            motionComplete = true;
            linePosition = CENTER_LINE;
        }
    }
}

void turnRightMillis(int angle)
{
    static unsigned long lastCheck = 0;
    const unsigned long checkInterval = 5;
    static int targetPulses;

    if (robotState != TURNING_RIGHT)
    {
        resetTicks();  // Reset left encoder ticks
        targetPulses = 0;
        float turnDistance = (angle / 360.0) * turn_Circumference;
        targetPulses = (turnDistance / WHEEL_CIRCUMFERENCE) * PULSE_PER_REVOLUTION;
        stopMotors();

        moveForward(200, 0);         // Right turn: left motor moves forward, right motor stops
        setTurnRightColor();        // Set color for right turn
        robotState = TURNING_RIGHT;  // Lock state to "TURNING_RIGHT"
        motionComplete = false;
    }

    if (robotState == TURNING_RIGHT && millis() - lastCheck >= checkInterval)
    {
        lastCheck = millis();
        readSensors();
        if (_leftTicks >= targetPulses)
        {
            stopMotors();
            robotState = FOLLOW_LINE;  // Unlock state after turn is complete;
            motionComplete = true;
            linePosition = CENTER_LINE;
        }
    }
}

void turnAroundMillis()
{
    static unsigned long lastCheck = 0;
    const unsigned long checkInterval = 5;
    static int targetPulses;
    if (robotState != TURNING_AROUND)
    {
        resetTicks();
        targetPulses = 0;

        float turnDistance = (3.14 * (DISTANCE_BETWEEN_WHEELS / 2));  // Half the turning circumference
        targetPulses = (turnDistance / WHEEL_CIRCUMFERENCE) * PULSE_PER_REVOLUTION;

        turn180(200, 200);  // Left wheel moves backward, right moves forward
        setTurnAroundColor();

        robotState = TURNING_AROUND;  // Lock state to "TURNING_AROUND"
        motionComplete = false;
    }

    if (robotState == TURNING_AROUND)
    {
        sensorValues[0] = analogRead(sensorPins[0]);
        if (sensorValues[0] > sensorThreshold[0] || sensorValues[4] > sensorThreshold[4])
        {
            stopMotors();
            robotState = FOLLOW_LINE;  // Unlock state after turn is complete
            motionComplete = true;
            linePosition = CENTER_LINE;
        }
    }
}

void moveForwardPID(int _leftSpeed, int _rightSpeed, bool withOutLine, bool lineTracking)
{
    if (withOutLine)
    {
        Kp = 1.0;   // Proportional Gain
        Ki = 0.01;  // Integral Gain
        Kd = 1.0;   // Derivative Gain

        // Calculate error
        error = _leftTicks - _rightTicks;

        // Update PID terms
        integral += error;
        derivative = error - lastError;
        lastError = error;

        // Prevent integral windup
        integral = constrain(integral, -10, 10);
    }
    else if (lineTracking)
    {
        readSensors();
        int position = calculateLinePosition();

        // PID Variables
        Kp = 0.4;      // Proportional Gain
        Ki = 0.00001;  // Integral Gain
        Kd = 0.1;      // Derivative Gain

        int center = (NUM_SENSORS - 1) * 1000 / 2;  // Midpoint of sensor array
        // Calculate error
        error = position - center;

        // Update PID terms
        integral += error;
        derivative = error - lastError;
        lastError = error;
    }

    // Apply PID correction
    correction = (Kp * error) + (Ki * integral) + (Kd * derivative);
    _leftSpeed -= correction;
    _rightSpeed += correction;

    // Ensure PWM values are within valid range (0 - 255)
    _leftSpeed = constrain(_leftSpeed, 0, 255);
    _rightSpeed = constrain(_rightSpeed, 0, 255);

    // Move motors
    moveForward(_leftSpeed, _rightSpeed);
}

void setup()
{
    Serial.begin(9600);
    NeoPixel.begin();
    NeoPixel.setBrightness(40);

    // Set Motor Pins
    pinMode(MOTOR_B_FORWARD, OUTPUT);
    pinMode(MOTOR_B_BACKWARD, OUTPUT);
    pinMode(MOTOR_A_FORWARD, OUTPUT);
    pinMode(MOTOR_A_BACKWARD, OUTPUT);
    digitalWrite(MOTOR_B_FORWARD, LOW);
    digitalWrite(MOTOR_B_BACKWARD, LOW);
    digitalWrite(MOTOR_A_FORWARD, LOW);
    digitalWrite(MOTOR_A_BACKWARD, LOW);
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
            turn180(200, 200);
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
        turnLeftMillis(90);
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
                turnLeftMillis(70);
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

