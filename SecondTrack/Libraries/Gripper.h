// Function to control the gripper servo using PWM
void gripper(int pulse)
{
    static unsigned long timer; // Stores the last update time to control timing
    static int lastPulse;  // Stores the last valid pulse width
    if (millis() > timer)
    {
        if(pulse > 0)
        {
            lastPulse = pulse;
        }
        else
        {
            pulse = lastPulse;
        }
        digitalWrite(GRIPPER, HIGH);
        delayMicroseconds(pulse);
        digitalWrite(GRIPPER, LOW);
        timer = millis() + 20; // 20 ms update interval
    }
}

// Function to open the gripper
void openGripper()
{
    gripper(GRIPPER_OPEN);
}

// Function to close the gripper
void closeGripper()
{
    gripper(GRIPPER_CLOSE);
}

// Function to initialize the gripper by opening it
void gripperSetup()
{
    openGripper();
}
