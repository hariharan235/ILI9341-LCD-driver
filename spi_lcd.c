/*
 * spi_lcd.c
 *
 *  Created on: Jun 2, 2019
 *      Author: harig
 */

#include "spi_lcd.h"
#include "wait.h"
#include "tm4c123gh6pm.h"


void initHw()
{

       // Configure System clock as 40Mhz
       SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);


       // Enable GPIO port F and B peripherals
       SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOF|SYSCTL_RCGC2_GPIOB;

       // Configure LED and pushbutton pins
       GPIO_PORTF_DIR_R |= (1 << 1) | (1 << 2) | (1 << 3);  // bits 1-3 are outputs, other pins are inputs
       GPIO_PORTF_DEN_R |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);  // enable LEDs and pushbuttons
       GPIO_PORTF_PUR_R |= (1 << 4);  // enable internal pull-up for push button


       // Configure ~CS,BL,D/~C,RESET pins
       GPIO_PORTB_DIR_R |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5);
       GPIO_PORTB_DEN_R |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 5);

       // Configure SSI2 pins for SPI configuration
       SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R2;           // turn-on SSI2 clocking
       GPIO_PORTB_DIR_R |= (1 << 4) | (1 << 7);         // make bits 4 and 7 outputs
       GPIO_PORTB_AFSEL_R |= (1 << 4) | (1 << 6) | (1 << 7);                      // select alternative functions for MOSI, MISO, SCLK pins
       GPIO_PORTB_PCTL_R = GPIO_PCTL_PB7_SSI2TX | GPIO_PCTL_PB6_SSI2RX | GPIO_PCTL_PB4_SSI2CLK; // map alt fns to SSI2
       GPIO_PORTB_DEN_R |= (1 << 4) | (1 << 6) | (1 << 7);                       // enable digital operation on TX, RX, CLK pins
       GPIO_PORTB_PUR_R |= (1 << 4); // Clk idle high for mode 3

       // Configure the SSI2 as a SPI master, mode 3, 8bit operation, 1 MHz bit rate
       SSI2_CR1_R &= ~SSI_CR1_SSE;                      // turn off SSI2 to allow re-configuration
       SSI2_CR1_R = 0;                                  // select master mode
       SSI2_CC_R = 0;                                   // select system clock as the clock source
       SSI2_CPSR_R = 40;                                // set bit rate to 1 MHz (if SR=0 in CR0)
       SSI2_CR0_R = SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;   // set SR=0, mode 3 (SPH=1, SPO=1), 8-bit
       SSI2_CR1_R |= SSI_CR1_SSE;                       // turn on SSI2

}


void putSSI2(uint8_t val)
{
    SSI2_DR_R = val;
    while (SSI2_SR_R & SSI_SR_BSY); //**< Wait till no operation on FIFO*/<

}

uint8_t getSSI2()
{
    return SSI2_DR_R;
}

int main()
{

    initHw();
    ONBOARD_RED_LED = 1;
    waitMicrosecond(10000);
    ONBOARD_RED_LED = 0;
    waitMicrosecond(10000);

    while(1);
}
