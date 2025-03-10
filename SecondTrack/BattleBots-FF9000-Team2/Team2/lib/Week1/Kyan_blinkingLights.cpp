#include <Arduino.h>

// Define pin numbers for the LED and buttons
const int RED_LED = 2;
const int BUTTON_1 = 3;
const int BUTTON_2 = 4;

// Define state variables for button presses and blinking speed
bool      _button_1_Pressed = false;
bool      _button_2_Pressed = false;
bool      _blinkingSlow = true;

void setup()
{
    // Initialize the LED and button pins
    pinMode(RED_LED, OUTPUT);
    pinMode(BUTTON_1, INPUT);
    pinMode(BUTTON_2, INPUT);

    // Set the initial state of the LED to ON
    digitalWrite(RED_LED, HIGH);
}

// Define the function prototypes
void buttonPress();
void blinkingFunction();

void loop()
{
    // Check button states and update blinking speed
    buttonPress();
    // Blink the LED based on the current speed setting
    blinkingFunction();
}

// Function to handle button presses and set the blinking speed accordingly
void buttonPress()
{
    // Check if BUTTON_1 is pressed
    if (digitalRead(BUTTON_1) == LOW)
    {
        _button_1_Pressed = true;
        _blinkingSlow = true; // Set blinking speed to slow
    }
    else
    {
        _button_1_Pressed = false;
    }

    // Check if BUTTON_2 is pressed
    if (digitalRead(BUTTON_2) == LOW)
    {
        _button_2_Pressed = true;
        _blinkingSlow = false; // Set blinking speed to fast
    }
    else
    {
        _button_2_Pressed = false;
    }
}

// Function to handle the LED blinking based on the current speed setting
void blinkingFunction()
{
    if (_blinkingSlow)
    {
        // Blink the LED slowly
        digitalWrite(RED_LED, LOW);
        delay(300);
        digitalWrite(RED_LED, HIGH);
        delay(300);
    }
    else
    {
        // Blink the LED quickly
        digitalWrite(RED_LED, LOW);
        delay(100);
        digitalWrite(RED_LED, HIGH);
        delay(100);
    }
}