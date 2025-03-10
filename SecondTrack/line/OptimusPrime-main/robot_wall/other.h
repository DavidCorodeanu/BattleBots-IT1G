#ifndef other_h
#define other_h

#include "motor_control.h"
// rotation sensor
void read_sensors() 
{
  if (millis() - sensorDataInterval >= timerSensor)
  {
    timerSensor = millis();
    int sr1_state = digitalRead(SR1);
    int sr2_state = digitalRead(SR2);
    int sr1_analog_state = analogRead(SR1);
    int sr2_analog_state = analogRead(SR2);
    Serial.print("SR1 analog: ");
    Serial.print(sr1_analog_state);
    Serial.print("||Digital: ");
    Serial.print(sr1_state);

    Serial.print("   SR2 analog: ");
    Serial.print(sr2_analog_state);
    Serial.print("||Digital: ");
    Serial.println(sr2_state);
  }
}
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2; 

  return distance;
}

//  return true If distance more than threshold
bool moreDistance(int threshold) {
  float distance = getDistance();
  if (millis() - sensorDataInterval >= timerGoodDistance)
        {
          timerGoodDistance = millis();
          Serial.print("Distance: ");
          Serial.print(distance);
          Serial.println(" cm");
        }
  if (distance > 0 && distance >= threshold) {
     
    Serial.println("Distance is good.");
    return true;
  } 
  return false;
  
}



// follow line code
void callibrate(){
  delay(1000);
  digitalWrite(13, HIGH);
  delay(1000);
  digitalWrite(13, LOW);
  black = analogRead(A4) - 150;
  if (millis() - sensorDataInterval >= timerColor)
  {
    timerColor = millis();
    Serial.print("Color value is: ");
    Serial.println(black);
  }

  
}
void read_color(){
    for (int i = 0; i < NUM_SENSORS; i++) {
      sensorValues[i] = analogRead(sensorPins[i]);
      Serial.print(sensorValues[i]);
      Serial.print(", ");
  }
  Serial.println(" ");
}

void optimus_followLine(){
  read_color();
  bool whereIsLine[NUM_SENSORS]; 
  Serial.print("SSSensors: ");
  for (int i = 0; i < NUM_SENSORS; i++) {
    whereIsLine[i] =  (sensorValues[i] > black);
    Serial.print(whereIsLine[i]);
    Serial.print(", ");
  }
  Serial.println(" ");

  if (whereIsLine[3] || whereIsLine[4]) {
    setBothMotor(190);
  } else if (whereIsLine[5] || whereIsLine[6] || whereIsLine[7]) {
    leftF(250);
  } else if (whereIsLine[2] || whereIsLine[1] || whereIsLine[0]) {
    rightF(250);
  }  else {
    allS();
    rightF(250);
  }

}
#endif