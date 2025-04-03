// Motor Pins
const int                   MOTOR_A1_FORWARD = 11;   // Left Forward
const int                   MOTOR_A2_BACKWARD = 10;   // Left Reverse
const int                   MOTOR_B2_FORWARD = 5;  // Right Forward
const int                   MOTOR_B1_BACKWARD = 6;   // Right Reverse
int                         baseSpeed = 255; // Base speed

// Rotation Sensors
const int                   MOTOR_R1 = 3;
const int                   MOTOR_R2 = 2;

const int                   ISR_INTERVAL = 20;  // interval of 20 milli seconds to update counter by interupt


// Servo Control
const int                   GRIPPER_OPEN = 1800;
const int                   GRIPPER_CLOSE = 990;
const int                   SERVO = 9;
const int                   PULSE = 2000;
int                         previousTime = 0;
const int                   GRIPPER_INTERVAL = 20;

// Line Sensors
const int                   NUM_SENSORS = 8;                                               // Number of sensors
int                         sensorPins[NUM_SENSORS] = { A0, A1, A2, A3, A4, A5, A6, A7 };  // Sensor pin mapping
int                         sensorValues[NUM_SENSORS];                                     // Array to store sensor readings
int                         sensorMin[NUM_SENSORS];
int                         sensorMax[NUM_SENSORS];
int                         sensorThreshold[NUM_SENSORS];                                  // Array to store thresholds for each sensor

// Line Positions
bool                        leftTurn;
bool                        rightTurn;
bool                        tJunctionOrBase;
bool                        deadEnd;

// Measurements
const float                 WHEEL_CIRCUMFERENCE = 20.4;
const int                   PULSE_PER_REVOLUTION = 20;
const float                 DISTANCE_BETWEEN_WHEELS = 22.75;
static const int            DISTANCE_FROM_BASE_TO_CONE = 55;
const int                   TARGET = DISTANCE_FROM_BASE_TO_CONE;

const int                   TRIG = 13;
const int                   ECHO = 12;

// Define NeoPixel
const int                   NEOPIXEL_PIN = 4;
const int                   NUM_PIXELS = 4;
const int                   PIXEL_BOTTOM_LEFT = 0;
const int                   PIXEL_BOTTOM_RIGHT = 1;
const int                   PIXEL_TOP_RIGHT = 2;
const int                   PIXEL_TOP_LEFT = 3;
Adafruit_NeoPixel           NeoPixel = Adafruit_NeoPixel(NUM_PIXELS, NEOPIXEL_PIN, NEO_RGB + NEO_KHZ800);

enum RobotState             {FOLLOW_LINE, TURNING, TURNING_LEFT, TURNING_RIGHT, TURNING_AROUND, CHECKING_FOR_PATH_AHEAD};
enum LinePosition           {T_JUNCTION, LEFT_LINE, RIGHT_LINE, NO_LINE, CENTER_LINE};

RobotState                  robotState = FOLLOW_LINE;
LinePosition                linePosition = CENTER_LINE;

// Pulse Counters
volatile signed int         _leftTicks = 5;
volatile signed int         _rightTicks = 0;

// Conditions
bool                        coneInSquare = true;
bool                        sensorsCalibrated = false;
bool                        conePickedUp = false;
bool                        gameStarted = false;
bool                        coneDroppedOff = false;
bool                        gameEnded = false;
bool                        motionComplete = true;
bool                        robotDetected = false;
bool                        blackSquareDetected = false;
bool                        robotCalibrated = false;

// Setup the minimum time to confirm a black square
const int                   MIN_SQUARE_TIME = 2000;  // Minimum time in ms to confirm we're on a black square

// PID Variables
int                         error = 0, lastError = 0;
float                       integral = 0;
float                       derivative = 0;
float                       Kp;  // Proportional Gain
float                       Ki;  // Integral Gain
float                       Kd;  // Derivative Gain
int                         correction;

// Robot variables
int                         pulses;
int                         angle;
int                         radius = DISTANCE_BETWEEN_WHEELS;
int                         turn_Circumference = 2 * 3.14 * radius;
float                       turnDistances = 0;  // ARC of a circle

// function to reset the encoder ticks
void resetTicks()
{
    _leftTicks = 0;
    _rightTicks = 0;
}