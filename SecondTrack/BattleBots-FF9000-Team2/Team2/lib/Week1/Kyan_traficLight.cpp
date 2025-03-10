#include <Arduino.h>

// Define pin numbers for the LEDs and button
const int   RED_LED = 2;
const int   YELLOW_LED = 3;
const int   GREEN_LED = 4;
const int   BUTTON = 5;

// Define state variables for the sequence
bool        _toggleSequence = false;

void setup()
{
    // Initialize the LED and button pins
    pinMode(RED_LED, OUTPUT);
    pinMode(YELLOW_LED, OUTPUT);
    pinMode(GREEN_LED, OUTPUT);
    pinMode(BUTTON, INPUT);

    // Set the initial state of the LEDs
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
    digitalWrite(YELLOW_LED, HIGH);
}

// Define the function prototype
void lightSequence();

void loop()
{
    lightSequence();
}

/*
    lightSequence() is a function that changes the traffic lights. First it sets the toggleSequence to true.
    Then it changes the lights by setting it to HIGH or LOW with a delay between switching lights.
    The first delay of 2 seconds (2000ms) prevents the red light from turning green immediately.
*/
void lightSequence()
{
    // Check if the button is pressed and the sequence is not already toggled
    if(digitalRead(BUTTON) == LOW && _toggleSequence == false)
    {
        _toggleSequence = true; // Set the sequence toggle to true
        delay(2000); // Wait for 2 seconds

        // Change the lights to red
        digitalWrite(RED_LED, HIGH);
        digitalWrite(GREEN_LED, LOW);
        delay(4000); // Wait for 4 seconds

        // Change the lights to yellow
        digitalWrite(GREEN_LED, HIGH);
        digitalWrite(YELLOW_LED, LOW); // Yellow LED 
        delay(2000); // Wait for 2 seconds

        // Change the lights back to the initial state
        digitalWrite(RED_LED, LOW);
        digitalWrite(YELLOW_LED, HIGH);

        _toggleSequence = false; // Reset the sequence toggle
    }
}