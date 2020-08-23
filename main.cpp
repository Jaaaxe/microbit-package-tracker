// UFCFVK-15-2 Internet of Things
// Challenge III
// Description: Demostrating the bluetooth capabilities of the microbit
// by making a package tracker that logs the time in transit and if
// it was dropped during shipment
// Authors: Mariyam Yasmeen (S1800367) & Ismath Ibrahim (S1900094)          
// Version: 1.3.0 
// Modified: 21 August 2020 
// Requirements: Microbit, Smart phone with android OS 
// Setup: Specified in README.md


#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitUARTService *uart;

static bool fallDetect;     // Used to trigger fall detection into a true state
int StartTime;              // Tracks the time when the package is set as shipped
int TransitTime;            // Tracks the time from when package is shipped, till it's received
int h = 0;                  // Tracked time in hours
int m = 0;                  // Tracked time in minutes


void BLEConnected(MicroBitEvent) {
    // Bluetooth connected
    uBit.display.scroll("BLE",100);
    // Prompting users to press A to start
    uBit.display.print("<");
}
void BLEDisconnected(MicroBitEvent) {
    // Bluetooth disconnected
    uBit.display.scroll("!BLE",100);
}

const char* getEvent(int state) {
    // Switch case that takes in the current gesture state (fall or none)
    // and returns FALL if the microbit fall detection event is triggered
    switch (state) {
    
        case  MICROBIT_ACCELEROMETER_EVT_NONE:       
        return " ";
        
        case  MICROBIT_ACCELEROMETER_EVT_FREEFALL:   
        return "FALL";
        
        default:                                     
        return " ";
    }
}

// Function to track the gesture (event) states that the 
// microbit is subjected to
// If a fall is detected the fallDetect bool is set to true
void TrackState(){
    uBit.accelerometer.updateSample();
    
        if(getEvent(uBit.accelerometer.getGesture())=="FALL"){
        fallDetect=true;
        }
        
    uBit.sleep(200);
    }
        
// Start time takes in the system time which is the time the microbit has been 
// on since the last reset
void TrackStart(){
    StartTime = uBit.systemTime();
    uBit.display.scroll("TRACKING",100);
    }
    
// Log report tracks the system time when the package is received and subtracts
// the initial start time so we accurately get the transit time
// This function also prints the log to the microbit display and the phone
void LogReport(){
    
        TransitTime = (uBit.systemTime() - StartTime);
        
        // If the values for hours and minutes is zero, there's no point in 
        // doing any calculations and so it will return zero. Else, the time
        // has to be converted from milliseconds to hours and minutes
        if(TransitTime>3600000){
            h = TransitTime/3600000;
        }
        
        if(TransitTime>60000){
            m = TransitTime/60000;
        } 
        
        // Messages sent to the app as a log
        uart->send("\nYour package was ");
        uart->send("in transit for ");
        uart->send(h);
        uart->send(" hour(s) and ");
        uart->send(m);
        uart->send(" minute(s)");        
        
        // Messages sent to the microbit as a log
        uBit.display.print(h);
        uBit.display.scroll("h",100);
        uBit.display.print(m);
        uBit.display.scroll("m",100);
        
        // Package status shows if the package was
        // dropped during the shipment process
        uart->send("\nPackage Status: ");
        
        // If a fall is detected it sets the message "compromised"
        // Else it's "safe"
        if(fallDetect){
            uart->send("Compromised");
            uBit.display.scroll("COMPROMISED",100);
            } else {
            uart->send("Safe");
            uBit.display.scroll("SAFE",100);
            }    
    }

int main() {
    
    uBit.init();
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    
    // Listeners to check if the bluetooth connects / disconnects
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_CONNECTED, BLEConnected);
    uBit.messageBus.listen(MICROBIT_ID_BLE, MICROBIT_BLE_EVT_DISCONNECTED, BLEDisconnected);
    
    uBit.display.scroll("HELLO",100);
    
    fallDetect = false;

    while(1) {
        
        // Keeps constant track of the microbit accelerometer
        // events so that falls can be identified
        TrackState();

        // Button A starts the tracking process
        if(uBit.buttonA.isPressed()){
            uart->send("Package Shipped\n");
            TrackStart();
            uBit.display.print(">");
            uBit.sleep(100);
            }
            
        // Button B ends the process and prints the log
        if(uBit.buttonB.isPressed()){
            uart->send("Package Arrived");
            LogReport();
            uBit.sleep(100);
        }

    }
   
=======
// UFCFVK-15-2 Internet of Things
// Challenge III
// Description: Demostrating the bluetooth capabilities of the microbit
// by making a package tracker that logs the time in transit and if
// it was dropped during shipment
// Authors: Mariyam Yasmeen (S1800367) & Ismath Ibrahim (S1900094)
// Version: 1.3.0
// Modified: 22 August 2020
// Requirements: Microbit, Smart phone with android OS
// Setup: Specified in README.md

#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitUARTService *uart;

static bool fallDetect; // Used to trigger fall detection into a true state
int StartTime;          // Tracks the time when the package is set as shipped
int TransitTime;        // Tracks the time from when package is shipped, till it's received
int h = 0;              // Tracked time in hours
int m = 0;              // Tracked time in minutes

void onConnected(MicroBitEvent)
{
    // Bluetooth connected
    uBit.display.scroll("CONNECTED", 100);
}
void onDisconnected(MicroBitEvent)
{
    // Bluetooth disconnected
    uBit.display.scroll("DISCONNECTED", 100);
}

const char *getEvent(int state)
{
    // Switch case that takes in the current gesture state (fall or none)
    // and returns FALL if the microbit fall detection event is triggered
    switch (state)
    {

    case MICROBIT_ACCELEROMETER_EVT_NONE:
        return " ";

    case MICROBIT_ACCELEROMETER_EVT_FREEFALL:
        return "FALL";

    default:
        return " ";
    }
}

// Function to track the gesture (event) states that the
// microbit is subjected to
// If a fall is detected the fallDetect bool is set to true
void TrackState()
{
    uBit.accelerometer.updateSample();

    if (getEvent(uBit.accelerometer.getGesture()) == "FALL")
    {
        fallDetect = true;
    }

    uBit.sleep(200);
}

// Start time takes in the system time which is the time the microbit has been
// on since the last reset
void TrackStart()
{
    StartTime = uBit.systemTime();
    uBit.display.scroll("TRACKING", 100);
}

// Log report tracks the system time when the package is received and subtracts
// the initial start time so we accurately get the transit time
// This function also prints the log to the microbit display and the phone
void LogReport()
{

    TransitTime = (uBit.systemTime() - StartTime);

    // If the values for hours and minutes is zero, there's no point in
    // doing any calculations and so it will return zero. Else, the time
    // has to be converted from milliseconds to hours and minutes
    if (TransitTime > 3600000)
    {
        h = TransitTime / 3600000;
    }

    if (TransitTime > 60000)
    {
        m = TransitTime / 60000;
    }

    // Messages sent to the app as a log
    uart->send("\nYour package was ");
    uart->send("in transit for ");
    uart->send(h);
    uart->send(" hour(s) and ");
    uart->send(m);
    uart->send(" minute(s)");

    // Messages sent to the microbit as a log
    uBit.display.print(h);
    uBit.display.scroll("h", 100);
    uBit.display.print(m);
    uBit.display.scroll("m", 100);

    // Package status shows if the package was
    // dropped during the shipment process
    uart->send("\nPackage Status: ");

    // If a fall is detected it sets the message "compromised"
    // Else it's "safe"
    if (fallDetect)
    {
        uart->send("Compromised");
        uBit.display.scroll("COMPROMISED", 100);
    }
    else
    {
        uart->send("Safe");
        uBit.display.scroll("SAFE", 100);
    }
}

int main()
{

    uBit.init();
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);

    uBit.display.scroll("HELLO", 100);
    // Prompting users to press A to start
    uBit.display.print("<");

    fallDetect = false;

    while (1)
    {

        // Keeps constant track of the microbit accelerometer
        // events so that falls can be identified
        TrackState();

        // Button A starts the tracking process
        if (uBit.buttonA.isPressed())
        {
            uart->send("Package Shipped\n");
            TrackStart();
            uBit.display.print(">");
            uBit.sleep(100);
        }

        // Button B ends the process and prints the log
        if (uBit.buttonB.isPressed())
        {
            uart->send("Package Arrived");
            LogReport();
            uBit.sleep(100);
        }
    }
}