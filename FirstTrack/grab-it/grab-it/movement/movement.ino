#define SERVO 12 
#define GRIPPER_OPEN 1850 
#define GRIPPER_CLOSE 1150 
 
// Motor A (Left) 
const int MOTOR_A_1 = 10;  // Motor A control pin 1 (left forward) 
const int MOTOR_A_2 = 11;  // Motor A control pin 2 (reverse for the left wheel) 

// Motor B (Right) 
const int MOTOR_B_1 = 5;  // Motor B control pin 1 (backwards for the right wheel) 
const int MOTOR_B_2 = 6;  // Motor B control pin 2 (forward for the right wheel) 
 
void setup() { 
  pinMode(SERVO, OUTPUT); 
  pinMode(MOTOR_A_1, OUTPUT); 
  pinMode(MOTOR_A_2, OUTPUT); 
  pinMode(MOTOR_B_1, OUTPUT); 
  pinMode(MOTOR_B_2, OUTPUT); 
  digitalWrite(SERVO, LOW); 
  digitalWrite(MOTOR_A_1, LOW); 
  digitalWrite(MOTOR_A_2, LOW); 
  digitalWrite(MOTOR_B_1, LOW); 
  digitalWrite(MOTOR_B_2, LOW); 
  Serial.begin(9600); 
} 

// Move forward 
void moveForward() { 
  digitalWrite(MOTOR_A_1, HIGH); 
  digitalWrite(MOTOR_A_2, LOW); 
  digitalWrite(MOTOR_B_1, LOW); 
  digitalWrite(MOTOR_B_2, HIGH); 
} 

// Move backward 
void moveBackward() { 
  digitalWrite(MOTOR_A_1, LOW); 
  digitalWrite(MOTOR_A_2, HIGH); 
  digitalWrite(MOTOR_B_1, HIGH); 
  digitalWrite(MOTOR_B_2, LOW); 
} 

// Stop motors 
void stopMotors() { 
  digitalWrite(MOTOR_A_1, LOW); 
  digitalWrite(MOTOR_A_2, LOW); 
  digitalWrite(MOTOR_B_1, LOW); 
  digitalWrite(MOTOR_B_2, LOW); 
} 

void moveServo(int pulse) { 
  static unsigned long timer; 
  static int lastPulse; 
  if (millis() > timer) { 
    if (pulse > 0) { 
      lastPulse = pulse; 
    } else { 
      pulse = lastPulse; 
    } 
    digitalWrite(SERVO, HIGH); 
    delayMicroseconds(pulse); 
    digitalWrite(SERVO, LOW); 
    timer = millis() + 20; // 20ms interval for servo 
  } 
} 

void loop() { 
  moveServo(GRIPPER_OPEN); // Open gripper 
  delay(500); 
  moveForward(); // Move forward to grab cone 
  delay(1000); 
  stopMotors(); 
  delay(500); 

  moveServo(GRIPPER_CLOSE); // Close gripper to grab cone 
  delay(100); 
  moveForward(); // Move forward with cone 
  delay(2000); 
  stopMotors(); 
  delay(500); 

  moveServo(GRIPPER_OPEN); // Release cone 
  delay(1000); 
  moveBackward(); // Move backward after dropping cone 
  delay(1000); 
  stopMotors(); 
}