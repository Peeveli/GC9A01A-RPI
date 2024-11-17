#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include "GC9A01A_rpi.h"
#include "GPIOConfig.h"

int main() {

    GPIOConfig gpio1(8, 27, 25, 19, 60000000, 0); // CS, RST, DC, BL, speed, bus
    GPIOConfig gpio2(7, 22, 24, 14, 60000000, 1); // CS, RST, DC, BL, speed, bus
    
    LCDDisplay display1(gpio1);
    LCDDisplay display2(gpio2);

    // Initialize each display
    display1.initDisplay();
    display2.initDisplay();
    std::cout << "Initialized!\n";

    display1.setBacklight(255);
    display2.setBacklight(255);
    display1.clear(GREEN);
    display2.clear(CYAN);

    return 0;
}