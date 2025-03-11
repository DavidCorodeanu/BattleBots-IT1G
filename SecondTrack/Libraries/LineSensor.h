void declareLineSensorPins()
{
    /*
    For each sensor in the array set the pin as an input.
    The && is used to avoid copying the value with each iteration, which improves the performance.
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
        int leftSensor = analogRead(LINE_SENSORS[2]); // Read left sensor value
        int rightSensor = analogRead(LINE_SENSORS[5]); // Read right sensor value
        int error = leftSensor - rightSensor;
        int derivative = error - lastError; // Store the current error for next iteration
        lastError = error;
        int correction = (Kp * error) + (Kd * derivative);

        int leftSpeed = constrain(baseSpeed - correction, 0, 255);
        int rightSpeed = constrain(baseSpeed + correction, 0, 255);

        drive(leftSpeed, 0, rightSpeed, 0);
    }
}