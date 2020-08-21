// UFCFVK-15-2 Internet of Things
// Challenge III
// Description: Demostrating the bluetooth capabilities of the microbit
// by making a package tracker that logs the time in transit and if
// it was dropped during shipment
// Authors: Mariyam Yasmeen (S1800367) & Ismath Ibrahim (S1900094)          
// Version: 1.2.0 
// Modified: 21 August 2020 
// Requirements: Microbit, Smart phone with android OS 
// Setup: Specified in README.md

#include "MicroBit.h"
#include "MicroBitUARTService.h"

MicroBit uBit;
MicroBitUARTService *uart;

static bool fallDetect;
int StartTime;
int TransitTime;
int h = 0;
int m = 0;

void onConnected(MicroBitEvent) {
    uBit.display.scroll("CONNECTED",100);
}
void onDisconnected(MicroBitEvent) {
    uBit.display.scroll("DISCONNECTED",100);
}

const char* getEvent(int state) {
    switch (state) {
        case  MICROBIT_ACCELEROMETER_EVT_NONE:       
        return " ";
        
        case  MICROBIT_ACCELEROMETER_EVT_FREEFALL:   
        fallDetect = true;
        return "FALL";
        
        default:                                     
        return " ";
    }
}
    

void TrackState(){
    uBit.accelerometer.updateSample();
    uBit.sleep(200);
    
    if(getEvent(uBit.accelerometer.getGesture())=="FALL"){
        fallDetect=true;
        }
    }
        
        
void TrackStart(){
    StartTime = uBit.systemTime();
    uBit.display.scroll("TRACKING",100);
    }
        
        
void LogReport(){
    
        TransitTime = (uBit.systemTime() - StartTime);
        
        if(TransitTime>3600000){
            h = TransitTime/3600000;
        }
        
        if(TransitTime>60000){
            m = TransitTime/60000;
        } 
        uart->send("\nYour package was ");
        uart->send("in transit for ");
        uart->send(h);
        uart->send(" hours and ");
        uart->send(m);
        uart->send(" minutes");        
        uBit.display.print(h);
        uBit.display.scroll("h",100);
        uBit.display.print(m);
        uBit.display.scroll("m",100);
        uart->send("\nPackage Status: ");
        
        if(fallDetect){
            uBit.display.scroll("COMPROMISED",100);
            uart->send("Compromised");
            } else {
            uBit.display.scroll("SAFE",100);
            uart->send("Safe");
            }    
    }

int main() {
    
    uBit.init();
    uart = new MicroBitUARTService(*uBit.ble, 32, 32);
    uBit.display.scroll("PRESS A TO START",100);
    fallDetect = false;

    while(1) {
        
        TrackState();

        if(uBit.buttonA.isPressed()){
            uart->send("Package Shipped");
            TrackStart();
            uBit.sleep(100);
            }
            
        if(uBit.buttonB.isPressed()){
            uart->send("Package Arrived");
            LogReport();
            uBit.sleep(100);
        }

    }
   
}