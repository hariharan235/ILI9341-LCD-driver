/*
 * LCDtestAPI.c
 *
 *  Created on: Jul 2, 2019
 *      Author: harig
 */

#include "ILI9341.h"


int main()
{
    //Init Pins
    //init LCD
    //Set rotation
    initSPI();
    init_lcd2();
    setRotation(0);
    waitMicrosecond(10000);
    clearScreen();
    setCharConfig(ILI9341_DARKGREEN,1,1,ILI9341_BLACK,1);
    drawString(" Press Enter to Begin",21);
    drawString("\r\nWelcome\r\n@ILI9341",19);
    while(1);
}
