// Define the analog pins for the line sensors
const int SENSORS[] = {A7, A6, A5, A4, A3, A2, A1, A0};
const int NUM_SENSORS = 8;

// Define the motor pins
const int MOTOR_A1 = 11; // Left motor forward
const int MOTOR_A2 = 10; // Left motor backward
const int MOTOR_B1 = 9;  // Right motor forward
const int MOTOR_B2 = 3;  // Right motor backward

// Motor speed settings
const int BASE_SPEED = 100;
const int MAX_SPEED = 255;

void setup()
{
    pinMode(MOTOR_A1, OUTPUT);
    pinMode(MOTOR_A2, OUTPUT);
    pinMode(MOTOR_B1, OUTPUT);
    pinMode(MOTOR_B2, OUTPUT);
    Serial.begin(9600);
}

void loop()
{
    int position = readLineSensors();
    int error = position - (NUM_SENSORS / 2);
    int turnSpeed = error * 20;
    
    int leftSpeed = constrain(BASE_SPEED - turnSpeed, 0, MAX_SPEED);
    int rightSpeed = constrain(BASE_SPEED + turnSpeed, 0, MAX_SPEED);
    
    setMotorSpeed(leftSpeed, rightSpeed);
}

int readLineSensors()
{
    int weightedSum = 0;
    int sum = 0;
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int sensorValue = analogRead(SENSORS[i]);
        sum += sensorValue;
        weightedSum += sensorValue * i;
    }
    return sum == 0 ? NUM_SENSORS / 2 : weightedSum / sum;
}

void setMotorSpeed(int leftSpeed, int rightSpeed)
{
    analogWrite(MOTOR_A1, leftSpeed);
    analogWrite(MOTOR_A2, 0);
    analogWrite(MOTOR_B1, rightSpeed);
    analogWrite(MOTOR_B2, 0);
}

