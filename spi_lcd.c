/**
 * Project         : ILI9341 LCD SPI source file
 * Filename        : spi_lcd.c
 * Version         : 1.3
 * Author          : Hariharan Gopalarishnan
 * Date            : June 20th, 2019
 * Target Platform : TM4C123GH6PM
 * IDE             : Code Composer Studio v7.4.0
 * System Clock    : 40 MHz
 * SSI Clock Rate  : 20 MHz
 * SSI Mode        : Master mode - 3
 * SSI Format      : Freescale SPI Frame Format
 * SSI Datasize    : 8-bit
 */

/**
 * Copyright (c) 2019, Hariharan Gopalakrishnan
 * All rights reserved.
 *
 */

/**
 *MIT License
 *
 *Copyright (c) 2019 Hariharan Gopalakrishnan
 *
 *Permission is hereby granted, free of charge, to any person obtaining a copy
 *of this software and associated documentation files (the "Software"), to deal
 *in the Software without restriction, including without limitation the rights
 *to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the Software is
 *furnished to do so, subject to the following conditions:
 *
 *The above copyright notice and this permission notice shall be included in all
 *copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *SOFTWARE.
 */

/*!<
 * Pin Map
 *
 * MISO - PD2
 * LED - PB3
 * SCK - PD0
 * MOSI - PD3
 * DC  - PE1
 * RESET - PE2
 * CS - PD1
 */

#include "spi_lcd.h"
#include "wait.h"
#include "tm4c123gh6pm.h"


void initSPI() //Currently includes GPIO + SYSCTL + LED + SSI Init
{

       // Configure System clock as 40Mhz
       SYSCTL_RCC_R = SYSCTL_RCC_XTAL_16MHZ | SYSCTL_RCC_OSCSRC_MAIN | SYSCTL_RCC_USESYSDIV | (4 << SYSCTL_RCC_SYSDIV_S);


       // Enable GPIO port F and B peripherals
       SYSCTL_RCGC2_R |= SYSCTL_RCGC2_GPIOB|SYSCTL_RCGC2_GPIOD|SYSCTL_RCGC2_GPIOE|SYSCTL_RCGC2_GPIOF;

       // Configure LED and pushbutton pins
       GPIO_PORTF_DIR_R |= (1 << 1) | (1 << 2) | (1 << 3);  // bits 1-3 are outputs, other pins are inputs
       GPIO_PORTF_DEN_R |= (1 << 1) | (1 << 2) | (1 << 3) | (1 << 4);  // enable LEDs and pushbuttons
       GPIO_PORTF_PUR_R |= (1 << 4);  // enable internal pull-up for push button


       // Configure BL_LED
       GPIO_PORTB_DIR_R |= (1 << 3);
       GPIO_PORTB_DEN_R |= (1 << 3);

       // Configure ~CS
       GPIO_PORTD_DIR_R |= (1 << 1);
       GPIO_PORTD_DEN_R |= (1 << 1);

       // Configure RESET , DC_NOT
       GPIO_PORTE_DIR_R |= (1 << 1) | (1 << 2);
       GPIO_PORTE_DEN_R |= (1 << 1) | (1 << 2);

       // Configure SSI1 pins for SPI configuration
       SYSCTL_RCGCSSI_R |= SYSCTL_RCGCSSI_R1;           // turn-on SSI1 clocking

       // wait until SSI is ready
           while ((SYSCTL_PRSSI_R & SYSCTL_PRSSI_R1) == 0){}

       GPIO_PORTD_DIR_R |= (1 << 0) | (1 << 3);         // make bits 0 and 3 outputs
       GPIO_PORTD_AFSEL_R |= (1 << 0) | (1 << 2) | (1 << 3);                      // select alternative functions for MOSI, MISO, SCLK pins
       GPIO_PORTD_PCTL_R = GPIO_PCTL_PD3_SSI1TX | GPIO_PCTL_PD2_SSI1RX | GPIO_PCTL_PD0_SSI1CLK; // map alt fns to SSI1
       GPIO_PORTD_DEN_R |= (1 << 0) | (1 << 2) | (1 << 3);                       // enable digital operation on TX, RX, CLK pins
       GPIO_PORTD_PUR_R |= (1 << 0); // Clk idle high for mode 3

       // Configure the SSI1 as a SPI master, mode 3, 8bit operation, 1 MHz bit rate
       SSI1_CR1_R &= ~SSI_CR1_SSE;                      // turn off SSI1 to allow re-configuration
       SSI1_CR1_R = 0;                                  // select master mode
       SSI1_CC_R = 0;                                   // select system clock as the clock source
       SSI1_CPSR_R = 40;                                // set bit rate to 1 MHz (if SR=0 in CR0)
       SSI1_CR0_R = SSI_CR0_SPH | SSI_CR0_SPO | SSI_CR0_FRF_MOTO | SSI_CR0_DSS_8;   // set SR=0, mode 3 (SPH=1, SPO=1), 8-bit
       SSI1_CR1_R |= SSI_CR1_SSE;                       // turn on SSI1

}


//*****************************************************************************
// Function for transmitting data to LCD screen.
//*****************************************************************************
void spi_Transmit(uint8_t *dataIn, size_t size , uint8_t *dataOut)
{
  uint8_t count = 0;

  // Wait until any SPI device is inactive
  while((SSI1_SR_R & SSI_SR_TFE)!= 1){};

  // Disable the SSI interface
  SSI1_CR1_R &= ~SSI_CR1_SSE;

  // Fill the Transmit FIFO
  while((SSI1_SR_R & SSI_SR_TNF)!= 0 && (count < size) )
  {
    // Send out the first byte
    SSI1_DR_R = *dataIn;
    dataIn++;
    count++;
  }

  //Enable SSI
  SSI1_CR1_R |= SSI_CR1_SSE;

  for( count = 0; count < size; count++)
  {
    // Wait until the recieve has finished
    while((SSI1_SR_R & SSI_SR_RNE)==0){};// wait until response

    // Store the results
    *dataOut =  SSI1_DR_R;
    dataOut++;

  }
}
