void gripper(int pulse)
 {
     static unsigned long timer; // Time tracker for servo pulse interval
     static int lastPulse; // Stores the last valid pulse value
     if (millis() > timer) // Only update the servo every 20ms (standard servo refresh rate)
     {
         if (pulse > 0)
         {
             lastPulse = pulse;
         }
         else
         {
             pulse = lastPulse;
         }
 
         digitalWrite(SERVO, HIGH);
         delayMicroseconds(pulse);
         digitalWrite(SERVO, LOW);
         timer = millis() + 20;
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