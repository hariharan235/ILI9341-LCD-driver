/*
 * spi_lcd.h
 *
 *  Created on: Jun 2, 2019
 *      Author: harig
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

#define CS_NOT               (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 5*4))) // PD1
#define BL_LED               (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 2*4))) // PB3
#define D_CNOT               (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 1*4))) // PE1
#define RESET                (*((volatile uint32_t *)(0x42000000 + (0x400053FC-0x40000000)*32 + 0*4))) // PE2

#define MISO PB6
#define MOSI PB7
#define SCLK PB4


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
