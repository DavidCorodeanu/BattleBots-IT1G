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