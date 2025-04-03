void calibrateSensors()
{
    static bool firstRun = true;
    readSensors();
    setCalibrateColor();

    // Initialize sensor min/max values on the first run
    if (firstRun)
    {
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorMin[i] = 1023;  // Set min to max ADC value
            sensorMax[i] = 0;     // Set max to min ADC value
        }
        firstRun = false;
    }

    // Check if target distance is reached
    if (_leftTicks > TARGET || _rightTicks > TARGET)
    {
        stopMotors();
        sensorsCalibrated = true;

        // Calculate and store threshold values
        for (int i = 0; i < NUM_SENSORS; i++)
        {
            sensorThreshold[i] = (sensorMin[i] + sensorMax[i]) / 2;
            Serial.print("Threshold [");
            Serial.print(i);
            Serial.print("]: ");
            Serial.println(sensorThreshold[i]);
            Serial.println(sensorMin[i]);
            Serial.println(sensorMax[i]);
        }
        return;
    }

    // Read sensor values and update min/max
    for (int i = 0; i < NUM_SENSORS; i++)
    {
        int sensorValue = analogRead(sensorPins[i]);

        if (sensorValue < sensorMin[i])
        {
            sensorMin[i] = sensorValue;
        }
        if (sensorValue > sensorMax[i])
        {
            sensorMax[i] = sensorValue;
        }
    }

    moveForwardPID(200, 200, true, false);
}