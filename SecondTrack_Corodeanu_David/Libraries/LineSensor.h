// Reads all line sensor values and stores them in sensorValues[]
void readSensors()
{
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        sensorValues[i] = analogRead(sensorPins[i]);
    }
}

// Calculates the position of the line using a weighted average
// Returns an integer proportional to the detected line position
int calculateLinePosition()
{
    long weightedSum = 0;
    long sum = 0;

    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int value = sensorValues[i];

        // Determine if the sensor is on the line
        weightedSum += (long)value * i * 1000; // More weight for sensors further right
        sum += value;
    }

    return weightedSum / sum;  // Returns the line position; undefined if sum is 0 (no line)
}

// Initialize the endGame() function to get it in scope
void endGame();

void getLinePosition() // Determines the robot's current position relative to the line
{
    readSensors(); // Update sensor readings

    // ========================= LINE PATTERN DETECTION =========================

    // Detect left turn (strong right-side sensors, weak left-side sensors)
    leftTurn =
    sensorValues[4] > sensorThreshold[4] &&
    sensorValues[5] > sensorThreshold[5] &&
    sensorValues[6] > sensorThreshold[6] &&
    sensorValues[7] > sensorThreshold[7] &&
    sensorValues[0] < sensorThreshold[0] &&
    sensorValues[1] < sensorThreshold[1] &&
    sensorValues[2] < sensorThreshold[2];

    // Detect right turn (strong left-side sensors, weak right-side sensors)
    rightTurn =
    sensorValues[5] < sensorThreshold[5] &&
    sensorValues[6] < sensorThreshold[6] &&
    sensorValues[7] < sensorThreshold[7] &&
    sensorValues[0] > sensorThreshold[0] &&
    sensorValues[1] > sensorThreshold[1] &&
    sensorValues[2] > sensorThreshold[2];

    // Detect T-junction or base (all sensors see dark)
    tJunctionOrBase =
    sensorValues[0] > sensorThreshold[0] &&
    sensorValues[1] > sensorThreshold[1] &&
    sensorValues[2] > sensorThreshold[2] &&
    sensorValues[3] > sensorThreshold[3] &&
    sensorValues[4] > sensorThreshold[4] &&
    sensorValues[5] > sensorThreshold[5] &&
    sensorValues[6] > sensorThreshold[6] &&
    sensorValues[7] > sensorThreshold[7];

    // Detect dead end (all sensors see light)
    deadEnd =
    sensorValues[0] < sensorThreshold[0] &&
    sensorValues[1] < sensorThreshold[1] &&
    sensorValues[2] < sensorThreshold[2] &&
    sensorValues[3] < sensorThreshold[3] &&
    sensorValues[4] < sensorThreshold[4] &&
    sensorValues[5] < sensorThreshold[5] &&
    sensorValues[6] < sensorThreshold[6] &&
    sensorValues[7] < sensorThreshold[7];

    // ===================== BLACK SQUARE DETECTION =======================

    static unsigned long blackSquareStartTime = 0;
    static bool potentialBlackSquare = false;
    static int blackSquareOffset = 50; // Offset above threshold to confirm "black"
    
    // Check if all sensors detect values significantly above threshold
    bool allSensorsBlack =
    sensorValues[0] > (sensorThreshold[0] + blackSquareOffset) &&
    sensorValues[1] > (sensorThreshold[1] + blackSquareOffset) &&
    sensorValues[2] > (sensorThreshold[2] + blackSquareOffset) &&
    sensorValues[3] > (sensorThreshold[3] + blackSquareOffset) &&
    sensorValues[4] > (sensorThreshold[4] + blackSquareOffset) &&
    sensorValues[5] > (sensorThreshold[5] + blackSquareOffset) &&
    sensorValues[6] > (sensorThreshold[6] + blackSquareOffset) &&
    sensorValues[7] > (sensorThreshold[7] + blackSquareOffset);

    // Start timing when black square first detected
    if (allSensorsBlack && !potentialBlackSquare)
    {
        blackSquareStartTime = millis();
        potentialBlackSquare = true;
    }

    // Confirms black square if sensors stay black for 150ms
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

    // ===================== LINE POSITION STATE SET ========================
    
    // Only update linePosition when current movement is done
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