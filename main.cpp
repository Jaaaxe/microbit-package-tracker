#include "MicroBit.h"

MicroBit uBit;

static bool fallDetect;

const char* getEvent(int state) {
    switch (state) {
        case  MICROBIT_ACCELEROMETER_EVT_NONE:       return ".";
        case  MICROBIT_ACCELEROMETER_EVT_FREEFALL:   
        fallDetect = true;
        return "\nFALL";
        default:                                     return ".";
    }
}

int main() {
    
    uBit.init();
    uBit.display.scroll("PRESS A TO START",50);
    uBit.serial.send("Start:\n");
    fallDetect = false;

    while(1) {
        int StartTime;
        int TransitTime;
        int h = 0;
        int m = 0;
        if(uBit.buttonA.isPressed()){
            StartTime = uBit.systemTime();
            uBit.display.scroll("TRACKING",50);
            }

        
        uBit.accelerometer.updateSample();
        uBit.serial.send(getEvent(uBit.accelerometer.getGesture()));
        uBit.sleep(300);

        if(getEvent(uBit.accelerometer.getGesture())=="FALL"){
            fallDetect=true;
            }
            
            
        if(uBit.buttonB.isPressed()){
        
        TransitTime = (uBit.systemTime() - StartTime);
        
        if(TransitTime>3600000){
            h = TransitTime/3600000;
        }
        
        if(TransitTime>60000){
            m = TransitTime/60000;
        } 
        uBit.serial.send("Your package was in transit for");
        uBit.serial.send(h);
        uBit.serial.send("hours");
        uBit.serial.send(m);
        uBit.serial.send("minutes");
        uBit.serial.send("Status: ");        
        
        uBit.display.print(h);
        uBit.display.scroll("h");
        uBit.display.print(m);
        uBit.display.scroll("m");
        
        if(fallDetect){
            uBit.display.scroll("FELL");
            uBit.serial.send("FELL");
            } else {
            uBit.display.scroll("SAFE");
            uBit.serial.send("SAFE");
            }
        }
        
        if(uBit.buttonAB.isPressed()){
             uBit.display.scroll("both"); 
            }
            
        uBit.sleep(200);
    }
}