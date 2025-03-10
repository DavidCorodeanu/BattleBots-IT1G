void gripper(int pulse)
{
    static unsigned long timer;
    static int lastPulse;
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

void openGripper()
{
    gripper(GRIPPER_OPEN);
}

void closeGripper()
{
    gripper(GRIPPER_CLOSE);
}

void gripperSetup()
{
    openGripper();
}