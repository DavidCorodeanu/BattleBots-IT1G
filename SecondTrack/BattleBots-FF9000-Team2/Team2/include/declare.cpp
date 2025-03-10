#include <Adafruit_NeoPixel.h>

// Define pins
const int           BLUETOOTH_TRANSMIT= 1;
const int           NEOPIXEL_PIN = 4;
const int           GRIPPER = 9;
const int           MOTOR_A_BACKWARD = 11;
const int           MOTOR_A_FORWARD = 10;
const int           MOTOR_B_FORWARD = 6;
const int           MOTOR_B_BACKWARD = 5;
const int           SONAR_SENSOR_ECHO = 8;
const int           SONAR_SENSOR_TRIGGER = 7;
const int           LINE_SENSORS[] = {A0, A1, A2, A3, A4, A5, A6, A7};

// PID Constants
float Kp = 0.5; // Determines correction strength
float Kd = 0.01; // Helps reduce shaking
int baseSpeed = 250; // Default speed of the motors
int lastError = 0; // Stores previous error for derivative calculation

// Define NeoPixel
const int           NUM_PIXELS = 4;
const int           PIXEL_BOTTOM_LEFT = 0;
const int           PIXEL_BOTTOM_RIGHT = 1;
const int           PIXEL_TOP_RIGHT = 2;
const int           PIXEL_TOP_LEFT = 3;
Adafruit_NeoPixel   NeoPixel = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);

// Gripper states
const int           GRIPPER_OPEN = 1800;
const int           GRIPPER_CLOSE = 990;

// Calibration offsets
const int           CALIBRATION_OFFSET_A = 10; // Adjust this value as needed
const int           CALIBRATION_OFFSET_B = 5;  // Adjust this value as needed

// Define state variables for the millis
unsigned long       _lastTime = 0;
bool                _avoidObject = false;