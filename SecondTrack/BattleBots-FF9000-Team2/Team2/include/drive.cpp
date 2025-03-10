// Drive with calibration
int calibrate(int n, int offset)
{
    int calibratedValue = n - offset; // Subtract the offset from the input value
    return (calibratedValue < 0) ? 0 : calibratedValue; // Ensure the value doesn't go below 0
}

void drive(int motorAForward, int motorABackward, int motorBForward, int motorBBackward)
{
    setDriveForwardColor();
    analogWrite(MOTOR_A_FORWARD, calibrate(motorAForward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_A_BACKWARD, calibrate(motorABackward, CALIBRATION_OFFSET_A));
    analogWrite(MOTOR_B_FORWARD, calibrate(motorBForward, CALIBRATION_OFFSET_B));
    analogWrite(MOTOR_B_BACKWARD, calibrate(motorBBackward, CALIBRATION_OFFSET_B));
}

void driveForward()
{
    setDriveForwardColor();
    drive(255, 0, 255, 0);
}

void driveBackward()
{
    setDriveBackwardColor();
    drive(0, 255, 0, 255);
}

void turnRight()
{
    setTurnRightColor();
    drive(0, 255, 255, 0);
}

void turnLeft()
{
    setTurnLeftColor();
    drive(255, 0, 0, 255);
}

void driveStop()
{
    setDriveStopColor();
    drive(0, 0, 0, 0);
}