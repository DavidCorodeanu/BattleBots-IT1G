// Drive with calibration
int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    return (calibratedValue < 0) ? 0 : calibratedValue; // Ensure the value doesn't go below 0
}

// Function to control motor movement
// Apply calibration offsets and write PWM values to motor driver
void drive(int motorBForward, int motorBBackwards, int motorAForward, int motorABackwards)
{
    analogWrite(MOTOR_B2_RIGHT_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_A)); // Right motor forward
    analogWrite(MOTOR_B1_RIGHT_BACKWARDS, calibrate(motorBBackwards, CALIBRATION_OFFSET_A)); // Right motor backward
    analogWrite(MOTOR_A1_LEFT_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_B)); // Left motor forward
    analogWrite(MOTOR_A2_LEFT_BACKWARDS, calibrate(motorABackwards, CALIBRATION_OFFSET_B)); // Left motor backward
}

// Move the robot forward at full speed
void driveForward()
{
    drive(255, 0, 255, 0);
}

// Move the robot backward at full speed
void driveBackward()
{
    drive(0, 255, 0, 255);
}

// Rotate the robot to the right
void driveRight()
{
    drive(0, 255, 255, 0);
}

// Rotate the robot to the left
void driveLeft()
{
    drive(255, 0, 0, 255);
}

// Stop all motor movement
void driveStop()
{
    drive(0, 0, 0, 0);
}