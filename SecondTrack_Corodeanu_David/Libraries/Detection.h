// Function to measure distance using ultrasonic sensor
float measureDistance()
{
    // Clear the TRIG pin to start a clean signal
    digitalWrite(TRIG, LOW);
    delayMicroseconds(2);

    // Send a 10 microsecond pulse to trigger the ultrasonic sensor
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

void endGame() // Function to end the game after dropping the cone
{
    // Only execute this block once if the game hasn't ended already
    if (!gameEnded)
    {
        stopMotors(); // Stop the robot
        long startTime = millis();
        while (millis() < startTime + 1000)
        {
            openGripper(); // Open gripper
            setDriveBackwardColor();
            moveBackward(255, 255);  // Move backward while dropping the cone
        }
        
        // After backing up, stop the robot completely
        stopMotors();
        setDriveStopColor();

        // Set flags to indicate the cone has been dropped and the game is over
        coneDroppedOff = true;
        gameEnded = true;

        // Ensure we can't accidentally start again
        motionComplete = true;
    }
}