// Motor Driver Pins
const int MOTOR_A1 = 11;
const int MOTOR_A2 = 10;
const int MOTOR_B1 = 6;
const int MOTOR_B2 = 5;

// Gripper Servo Pin
const int GRIPPER_SERVO = 9;

// Rotation Sensors
const int SENSOR_R1 = 8;
const int SENSOR_R2 = 7;

// Servo positions
#define GRIPPER_OPEN 1600
#define GRIPPER_CLOSED 1000

int currentPulse = GRIPPER_OPEN;  // Start with the gripper open

void setup()
{
    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);
    
    pinMode(GRIPPER_SERVO, OUTPUT);
    
    pinMode(SENSOR_R1, INPUT);
    pinMode(SENSOR_R2, INPUT);

    Serial.begin(9600);
}

void loop()
{
    // Move forward for 2 seconds at 50% speed
    moveForward(2000);
    stopMotors();
    delay(2000);
    
    // Close the gripper and keep it engaged
    currentPulse = GRIPPER_CLOSED;
    delay(2000);

    // Move forward for another 2 seconds at 50% speed
    moveForward(2000);
    
    // Stop
    stopMotors();

    // Keep the gripper engaged indefinitely
    while (true)
    {
        gripper(currentPulse);
    }
}

// Move forward for a specified time (milliseconds)
void moveForward(int timeMs)
{
    unsigned long startTime = millis();
    
    while (millis() - startTime < timeMs)
    {
        digitalWrite(MOTOR_A1, HIGH);  // Forward
        digitalWrite(MOTOR_A2, LOW);
        digitalWrite(MOTOR_B1, LOW);  // Forward
        digitalWrite(MOTOR_B2, HIGH);

        gripper(currentPulse);  // Keep the gripper updated
    }
}

// Stop the motors
void stopMotors()
{
    digitalWrite(MOTOR_A1, LOW);
    digitalWrite(MOTOR_A2, LOW);
    digitalWrite(MOTOR_B1, LOW);
    digitalWrite(MOTOR_B2, LOW);
}

// Keeps the gripper engaged
void gripper(int pulse)
{
    static unsigned long timer = 0;
    static int lastPulse = GRIPPER_OPEN;

    if (millis() - timer >= 20)
    {
        digitalWrite(GRIPPER_SERVO, HIGH);
        delayMicroseconds(pulse);
        digitalWrite(GRIPPER_SERVO, LOW);
        
        timer = millis();
    }
}

