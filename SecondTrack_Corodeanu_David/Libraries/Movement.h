void stopMotors()
{
    analogWrite(MOTOR_A1_FORWARD, 0);    
    analogWrite(MOTOR_A2_BACKWARD, 0);
    analogWrite(MOTOR_B2_FORWARD, 0);
    analogWrite(MOTOR_B1_BACKWARD, 0);
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

void moveForward(int _rightSpeed, int _leftSpeed)
{
    if (!gameEnded)
    {
        analogWrite(MOTOR_A1_FORWARD, _rightSpeed);
        analogWrite(MOTOR_B2_FORWARD, _leftSpeed);
    }
}

void moveBackward(int _rightSpeed, int _leftSpeed)
{
    if (!gameEnded)
    {
        analogWrite(MOTOR_A2_BACKWARD, _rightSpeed);
        analogWrite(MOTOR_B1_BACKWARD, _leftSpeed);
    }
}

void turn180(int _leftSpeed, int _rightSpeed)
{
    analogWrite(MOTOR_A2_BACKWARD, 0);
    analogWrite(MOTOR_B2_FORWARD, 0);
    analogWrite(MOTOR_A1_FORWARD, _rightSpeed);
    analogWrite(MOTOR_B1_BACKWARD, _leftSpeed);
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

        moveForward(0, 225);
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

        moveForward(225, 0);         // Right turn: left motor moves forward, right motor stops
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

        turn180(225, 225);  // Left wheel moves backward, right moves forward
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