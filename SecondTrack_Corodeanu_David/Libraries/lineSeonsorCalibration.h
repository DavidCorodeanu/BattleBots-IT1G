void calibrateSensors() // Function to calibrate the line sensors by recording their min and max values
{
    static bool firstRun = true; // Flag to check if it's the first time running calibration
    readSensors(); // Read initial sensor values
    setCalibrateColor();

    // Initialize sensor min/max values on the first run
    if (firstRun)
    {
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorMin[i] = 1023;  // Set min to max ADC value
            sensorMax[i] = 0;     // Set max to min ADC value
        }
        firstRun = false; // Set flag to false after initialization
    }

    // Check if robot has moved far enough to complete calibration
    if (_leftTicks > TARGET || _rightTicks > TARGET)
    {
        stopMotors();
        sensorsCalibrated = true;

        // Calculate threshold values for each sensor and print them
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorThreshold[i] = (sensorMin[i] + sensorMax[i]) / 2; // Average of min and max

            // Print calibration data to Serial Monitor
            Serial.print("Threshold [");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(sensorThreshold[i]);
            Serial.println(sensorMin[i]);
            Serial.println(sensorMax[i]);
        }
        return;
    }

    // Update min and max values by reading current sensor values
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int sensorValue = analogRead(sensorPins[i]);

        if (sensorValue < sensorMin[i]) // Update the minimum value seen so far
        {
            sensorMin[i] = sensorValue;
        }
        if (sensorValue > sensorMax[i]) // Update the maximum value seen so far
        {
            sensorMax[i] = sensorValue;
        }
    }

    moveForwardPID(200, 200, true, false); // Continue moving forward while collecting sensor calibration data
}