// Define pins
const int           BLUETOOTH_TRANSMIT= 1; // Sets up the bluetooth SerialBegin(9600) connection
const int           NEOPIXEL_PIN = 4; // NeoPixel connection
const int           GRIPPER = 9; // Gripper 
const int           MOTOR_A1_LEFT_FORWARD = 11;  //Left Forward Motor
const int           MOTOR_A2_LEFT_BACKWARDS = 10; //Left Backwards Motor
const int           MOTOR_B1_RIGHT_BACKWARDS = 6; //Right Backwards Motor
const int           MOTOR_B2_RIGHT_FORWARD = 5; //Right Forward Motor
const int           SONAR_SENSOR_ECHO = 12; //Echo pin for the object detection sonar sensor
const int           SONAR_SENSOR_TRIGGER = 13; //Trigger pin for the object detection sonar sensor
const int           LINE_SENSORS[] = {A0, A1, A2, A3, A4, A5, A6, A7};  // Array of the Line sensors

// PID Constants
float Kp = 0.5; // Determines correction strength
float Kd = 0.01; // Helps reduce shaking
int baseSpeed = 250; // Default speed of the motors
int lastError = 0; // Stores previous error for derivative calculation

/* Define NeoPixel
const int           NUM_PIXELS = 4;
const int           PIXEL_BOTTOM_LEFT = 0;
const int           PIXEL_BOTTOM_RIGHT = 1;
const int           PIXEL_TOP_RIGHT = 2;
const int           PIXEL_TOP_LEFT = 3;
*/

// Gripper states
const int           GRIPPER_OPEN = 1300; // PWM value to open the gripper
const int           GRIPPER_CLOSE = 990; // PWM value to close the gripper

// Calibration offsets
const int           CALIBRATION_OFFSET_A = 0; // Adjust this value as needed
const int           CALIBRATION_OFFSET_B = 0;  // Adjust this value as needed

// Define state variables for the millis
unsigned long       _lastTime = 0; // Stores the last recorded time in milliseconds
bool                _avoidObject = false; // Tracks whether an object avoidance maneuver is active or not