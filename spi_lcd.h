/**
 * Project         : ILI9341 LCD SPI header file
 * Filename        : spi_lcd.h
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

#ifndef SPI_LCD_H_
#define SPI_LCD_H_


#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

/**Bit-Banded defines*/

#define ONBOARD_RED_LED      (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 1*4))) // PF1
#define ONBOARD_GREEN_LED    (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 3*4))) // PF3
#define ONBOARD_BLUE_LED     (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 2*4))) // PF2
#define ONBOARD_PUSH_BUTTON  (*((volatile uint32_t *)(0x42000000 + (0x400253FC-0x40000000)*32 + 4*4))) // PF4

#define CS_NOT               (*((volatile uint32_t *)(0x42000000 + (0x400073FC-0x40000000)*32 + 1*4))) // PD1
#define BL_LED               (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 3*4))) // PB3
#define D_CNOT               (*((volatile uint32_t *)(0x42000000 + (0x400243FC-0x40000000)*32 + 1*4))) // PE1
#define RESET                (*((volatile uint32_t *)(0x42000000 + (0x400243FC-0x40000000)*32 + 2*4))) // PE2


/**Masking macros*/

#define LO(x)        ((x) & 0x0F) //LSB
#define HI(x)        (((x)>>4) & 0x0F) //MSB

/**Tests**/

#ifndef test_spi
#define test_spi
#endif

void initSPI();
void spi_Transmit(uint8_t *dataIn, size_t size , uint8_t *dataOut);
#endif /* SPI_LCD_H_ */
