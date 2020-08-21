#include "MicroBit.h"

MicroBit uBit;

static bool fallDetect;
int StartTime;
int TransitTime;
int h = 0;
int m = 0;

const char *getEvent(int state)
{
    switch (state)
    {
    case MICROBIT_ACCELEROMETER_EVT_NONE:
        return ".";

    case MICROBIT_ACCELEROMETER_EVT_FREEFALL:
        fallDetect = true;
        return "FALL";

    default:
        return ".";
    }
}

void TrackState()
{
    uBit.accelerometer.updateSample();
    uBit.serial.send(getEvent(uBit.accelerometer.getGesture()));
    uBit.sleep(200);

    if (getEvent(uBit.accelerometer.getGesture()) == "FALL")
    {
        fallDetect = true;
    }
}

void TrackStart()
{
    StartTime = uBit.systemTime();
    uBit.display.scroll("TRACKING", 100);
}

void LogReport()
{

    TransitTime = (uBit.systemTime() - StartTime);

    if (TransitTime > 3600000)
    {
        h = TransitTime / 3600000;
    }

    if (TransitTime > 60000)
    {
        m = TransitTime / 60000;
    }
    uBit.serial.send("Your package was in transit for ");
    uBit.serial.send(h);
    uBit.serial.send(" hours and ");
    uBit.serial.send(m);
    uBit.serial.send(" minutes");
    uBit.serial.send("Package Status: ");

    uBit.display.print(h);
    uBit.display.scroll("h");
    uBit.display.print(m);
    uBit.display.scroll("m");

    if (fallDetect)
    {
        uBit.display.scroll("FELL");
        uBit.serial.send("Compromised");
    }
    else
    {
        uBit.display.scroll("SAFE");
        uBit.serial.send("Safe");
    }
}

int main()
{

    uBit.init();
    uBit.display.scroll("PRESS A TO START", 100);
    uBit.serial.send("Start:\n");
    fallDetect = false;

    while (1)
    {

        TrackState();

        if (uBit.buttonA.isPressed())
        {
            TrackStart();
            uBit.sleep(100);
        }

        if (uBit.buttonB.isPressed())
        {
            LogReport();
            uBit.sleep(100);
        }
    }
}