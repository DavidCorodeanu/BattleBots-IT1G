long readSonarSensor()
{
    // Send a pulse to trigger the sonar sensor
    digitalWrite(SONAR_SENSOR_TRIGGER, LOW);
    delayMicroseconds(2);
    digitalWrite(SONAR_SENSOR_TRIGGER, HIGH);
    delayMicroseconds(10);
    digitalWrite(SONAR_SENSOR_TRIGGER, LOW);

    // Read the echo pin and calculate the distance
    long duration = pulseIn(SONAR_SENSOR_ECHO, HIGH);
    return duration * 0.034 / 2; // Convert duration to distance in cm by using the speed of sound (0.034 cm per microsecond)
    // This does not convert to actual centimeters because of hardware compatibility issues or some other factors
}