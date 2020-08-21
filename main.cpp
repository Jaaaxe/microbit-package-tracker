#include "MicroBit.h"
#include <math.h>

MicroBit uBit;

static bool fallDetect;

const char* getAccGestureName(int eventValue) {
    switch (eventValue) {
        case  MICROBIT_ACCELEROMETER_EVT_NONE:       return ".";
        case  MICROBIT_ACCELEROMETER_EVT_TILT_UP:    return "\nUP";
        case  MICROBIT_ACCELEROMETER_EVT_TILT_DOWN:  return "\nDOWN";
        case  MICROBIT_ACCELEROMETER_EVT_TILT_LEFT:  return "\nLEFT";
        case  MICROBIT_ACCELEROMETER_EVT_TILT_RIGHT: return "\nRIGHT";
        case  MICROBIT_ACCELEROMETER_EVT_FACE_UP:    return "\nUP";
        case  MICROBIT_ACCELEROMETER_EVT_FACE_DOWN:  return "\nDOWN";
        case  MICROBIT_ACCELEROMETER_EVT_FREEFALL:   
        fallDetect = true;
        return "\nFALL";
        case  MICROBIT_ACCELEROMETER_EVT_3G:         return "\n3G";
        case  MICROBIT_ACCELEROMETER_EVT_6G:         return "\n6G";
        case  MICROBIT_ACCELEROMETER_EVT_8G:         return "\n8G";
        case  MICROBIT_ACCELEROMETER_EVT_SHAKE:      return "\nSHAKE";
        default:                                     return "\n?";
    }
}

int main() {
    
    uBit.init();
    uBit.display.print('!');
    uBit.serial.send("Start:\n");
    
    

    for (;;) {
        int StartTime;
        int TransitTime;
        int h = 0;
        int m = 0;
        if(uBit.buttonA.isPressed()){
            StartTime = uBit.systemTime();
            uBit.display.scroll("Tracking");
            }

        
        uBit.accelerometer.updateSample();
        uBit.serial.send(getAccGestureName(uBit.accelerometer.getGesture()));
        uBit.sleep(300);

        if(getAccGestureName(uBit.accelerometer.getGesture())=="FALL"){
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
            
        uBit.sleep(200);
    }
}