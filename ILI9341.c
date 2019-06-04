#include "spi_lcd.h"
#include "ILI9341.h"
#include "wait.h"

#include "tm4c123gh6pm.h"



void  writedata (uint8_t d)
{
      uint8_t data = d;
      uint8_t dataOut;

      CS_NOT = 0;
      D_CNOT = 1;


      spiTx(&data, 1, &dataOut);

      CS_NOT = 1;
}


void writecommand(uint8_t d)
{
    uint8_t data = d;
    uint8_t dataOut;

    CS_NOT = 0;
    D_CNOT = 0;


    spiTx(&data, 1, &dataOut);

    CS_NOT = 1;
}

void init_lcd2(void)
{
    // Reset the LCD by toggling the pin low
    waitMicrosecond(5);
    RESET = 0;
    waitMicrosecond(10);
    RESET = 1;
    waitMicrosecond(120);


    writecommand(0xEF);
  writedata(0x03);
  writedata(0x80);
  writedata(0x02);

  writecommand(0xCF);
  writedata(0x00);
  writedata(0XC1);
  writedata(0X30);

  writecommand(0xED);
  writedata(0x64);
  writedata(0x03);
  writedata(0X12);
  writedata(0X81);

  writecommand(0xE8);
  writedata(0x85);
  writedata(0x00);
  writedata(0x78);

  writecommand(0xCB);
  writedata(0x39);
  writedata(0x2C);
  writedata(0x00);
  writedata(0x34);
  writedata(0x02);

  writecommand(0xF7);
  writedata(0x20);

  writecommand(0xEA);
  writedata(0x00);
  writedata(0x00);

  writecommand(ILI9341_PWCTR1);    //Power control
  writedata(0x23);   //VRH[5:0]

  writecommand(ILI9341_PWCTR2);    //Power control
  writedata(0x10);   //SAP[2:0];BT[3:0]

  writecommand(ILI9341_VMCTR1);    //VCM control
  writedata(0x3e); //?????
  writedata(0x28);

  writecommand(ILI9341_VMCTR2);    //VCM control2
  writedata(0x86);  //--

  writecommand(ILI9341_MADCTL);    // Memory Access Control
  writedata(0x48);

  writecommand(ILI9341_PIXFMT);
  writedata(0x55);

  writecommand(ILI9341_FRMCTR1);
  writedata(0x00);
  writedata(0x18);

  writecommand(ILI9341_DFUNCTR);    // Display Function Control
  writedata(0x08);
  writedata(0x82);
  writedata(0x27);

  writecommand(0xF2);    // 3Gamma Function Disable
  writedata(0x00);

  writecommand(ILI9341_GAMMASET);    //Gamma curve selected
  writedata(0x01);

  writecommand(ILI9341_GMCTRP1);    //Set Gamma
  writedata(0x0F);
  writedata(0x31);
  writedata(0x2B);
  writedata(0x0C);
  writedata(0x0E);
  writedata(0x08);
  writedata(0x4E);
  writedata(0xF1);
  writedata(0x37);
  writedata(0x07);
  writedata(0x10);
  writedata(0x03);
  writedata(0x0E);
  writedata(0x09);
  writedata(0x00);

  writecommand(ILI9341_GMCTRN1);    //Set Gamma
  writedata(0x00);
  writedata(0x0E);
  writedata(0x14);
  writedata(0x03);
  writedata(0x11);
  writedata(0x07);
  writedata(0x31);
  writedata(0xC1);
  writedata(0x48);
  writedata(0x08);
  writedata(0x0F);
  writedata(0x0C);
  writedata(0x31);
  writedata(0x36);
  writedata(0x0F);


    writecommand(ILI9341_SLPOUT);    //Exit Sleep

    waitMicrosecond(1200);

   writecommand(ILI9341_DISPON);    //Display on

   waitMicrosecond(15000); // wait a bit
        // turn on the back light
   BL_LED = 1;
}

void setRotation(uint8_t m)
{

    uint8_t rotation;
  writecommand(ILI9341_MADCTL);
  rotation = m % 4; // can't be higher than 3
  switch (rotation)
  {
   case 0:
     writedata(MADCTL_MX | MADCTL_BGR);
     break;
   case 1:
     writedata(MADCTL_MV | MADCTL_BGR);
     break;
   case 2:
     writedata(MADCTL_MY | MADCTL_BGR);
    break;
   case 3:
     writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     break;
  }
}


void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

  writecommand(ILI9341_CASET); // Column addr set
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND

  writecommand(ILI9341_PASET); // Row addr set
  writedata(y0>>8);
  writedata(y0);     // YSTART
  writedata(y1>>8);
  writedata(y1);     // YEND

  writecommand(ILI9341_RAMWR); // write to RAM
}

// fill a rectangle
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    uint8_t data[2];
    uint8_t dataOut[2];
    uint8_t hi;
    uint8_t lo;

  // rudimentary clipping (drawChar w/big text requires this)
  if((x >= ILI9341_TFTWIDTH) || (y >= ILI9341_TFTHEIGHT)) return;
  if((x + w - 1) >= ILI9341_TFTWIDTH)  w = ILI9341_TFTWIDTH  - x;
  if((y + h - 1) >= ILI9341_TFTHEIGHT) h = ILI9341_TFTHEIGHT - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

    hi = color >> 8;
    lo = color;

    D_CNOT = 1;

    CS_NOT = 0;

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
            data[0] = hi;
            data[1] = lo;

            spiTx(&data[0], 2, dataOut);
    }
  }

    CS_NOT = 1;
}

void fillScreen(uint16_t color)
{
  fillRect(0, 0,  ILI9341_TFTWIDTH, ILI9341_TFTHEIGHT, color);
}

int main()
{
    //Init Pins
    //init LCD
    //Set rotation
    initSPI();
    init_lcd2();
    setRotation(0);
    fillScreen(ILI9341_BLUE);


    while(1);
}
