void setStandByColor()
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

void setDriveForwardColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setDriveBackwardColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 255, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 255, 0);
    NeoPixel.show();
}

void setTurnRightColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 150, 0);
    NeoPixel.show();
}

void setTurnLeftColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 0, 0);
    NeoPixel.show();
}

void setTurnAroundColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 150, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 150, 0);
    NeoPixel.show();
}

void setDriveStopColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 255, 0, 0);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 255, 0, 0);
    NeoPixel.show();
}

void setCalibrateColor()
{
    NeoPixel.setPixelColor(PIXEL_TOP_LEFT, 0, 255, 255);
    NeoPixel.setPixelColor(PIXEL_TOP_RIGHT, 0, 255, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_LEFT, 0, 255, 255);
    NeoPixel.setPixelColor(PIXEL_BOTTOM_RIGHT, 0, 255, 255);
    NeoPixel.show();
}