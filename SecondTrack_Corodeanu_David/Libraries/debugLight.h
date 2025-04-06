void setStandByColor() // Sets the NeoPixels to a blinking red pattern for standby mode
{
    static unsigned long previousMillis = 0;
    static bool isOn = false;
    const unsigned long interval = 500;

    unsigned long currentMillis = millis();

    if (currentMillis - previousMillis >= interval)
    {
        previousMillis = currentMillis;
        isOn = !isOn;

        if (isOn)
        {
            NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
            NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
            NeoPixel.show();
        }
        else
        {
            NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
            NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
            NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
            NeoPixel.show();
        }
        NeoPixel.show();
    }
}

void setDriveForwardColor() // Indicates driving forward with green top LEDs
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setDriveBackwardColor() // Indicates driving backward with yellow bottom LEDs
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 255, 0);
    NeoPixel.show();
}

void setTurnRightColor() // Indicates turning right with orange on the right side
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 150, 0);
    NeoPixel.show();
}

void setTurnLeftColor() // Indicates turning left with orange on the left side
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setTurnAroundColor() // Indicates turning around with all orange LEDs
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 150, 0);
    NeoPixel.show();
}

void setDriveStopColor() // Indicates stop with all red LEDs
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setCalibrateColor() // Indicates calibration mode with cyan color on all LEDs
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 255);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 255, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 255, 255);
    NeoPixel.show();
}