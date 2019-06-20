#include "spi_lcd.h"
#include "ILI9341.h"
#include "wait.h"
//#include "fontsans.h"
//#include "fontcalibri.h"
#include "fontcalibri14.h"
#include "tm4c123gh6pm.h"

//TODO : fontconfig fns
//TODO : image displays
//TODO : Add touch lib
//uint16_t cursorX,cursorY;
struct LCD_status_t
{
    uint16_t CURSORX;
    uint16_t CURSORY;
    uint16_t LCD_HEIGHT;
    uint16_t LCD_WIDTH;
    uint16_t CHARCOLOR;
    uint16_t CHARBGCOLOR;
    uint8_t ROTATION;
    uint8_t LINESPACING;
//    uint8_t *fontArray;
//    uint8_t (*fontDesp)[][3];
//    uint8_t fontSize;
//    uint8_t fontStyle;
    uint8_t CHARSPACING;
    uint8_t CHARSIZE;
}LCD;

struct LCD_font
{
    uint8_t FONTWIDTH;
    uint8_t FONTHEIGHT;
}font;
void  writedata (uint8_t d)
{
      uint8_t data = d;
      uint8_t dataOut;
      CS_NOT = 0;
      D_CNOT = 1;
      spi_Transmit(&data, 1,&dataOut);
      CS_NOT = 1;
}


void writecommand(uint8_t d)
{
    uint8_t data = d;
    uint8_t dataOut;
    CS_NOT = 0;
    D_CNOT = 0;
    spi_Transmit(&data, 1,&dataOut);
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
     LCD.ROTATION = 0;
     LCD.LCD_HEIGHT = 320;
     LCD.LCD_WIDTH = 240;
     LCD.CURSORX = 10;
     LCD.CURSORY = 220;
     break;
   case 1:
     writedata(MADCTL_MV | MADCTL_BGR);
     LCD.ROTATION = 1;
     LCD.LCD_HEIGHT = 240;
     LCD.LCD_WIDTH = 320;
     LCD.CURSORX = 10;
     LCD.CURSORY = 250;
     break;
   case 2:
     writedata(MADCTL_MY | MADCTL_BGR);
     LCD.ROTATION = 2;
     LCD.LCD_HEIGHT = 320;
     LCD.LCD_WIDTH = 240;
     LCD.CURSORX = 10;
     LCD.CURSORY = 220;
    break;
   case 3:
     writedata(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
     LCD.ROTATION = 3;
     LCD.LCD_HEIGHT = 240;
     LCD.LCD_WIDTH = 320;
     LCD.CURSORX = 10;
     LCD.CURSORY = 250;
     break;
  }
}


void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{

  writecommand(ILI9341_CASET); // Column addr set 0x2A
  writedata(x0 >> 8);
  writedata(x0 & 0xFF);     // XSTART
  writedata(x1 >> 8);
  writedata(x1 & 0xFF);     // XEND

  writecommand(ILI9341_PASET); // Row addr set 0x2B
  writedata(y0>>8);
  writedata(y0);     // YSTART
  writedata(y1>>8);
  writedata(y1);     // YEND

  writecommand(ILI9341_RAMWR); // write to RAM 0x2C
}

// fill a rectangle
void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    uint8_t data[2];
    uint8_t dataOut[2];
    uint8_t hi;
    uint8_t lo;

  if((x + w - 1) >= LCD.LCD_WIDTH)  w =  LCD.LCD_WIDTH - x;
  if((y + h - 1) >= LCD.LCD_HEIGHT) h = LCD.LCD_HEIGHT - y;

  setAddrWindow(x, y, x+w-1, y+h-1);

    hi = color >> 8;
    lo = color;

    D_CNOT = 1;

    CS_NOT = 0;

  for(y=h; y>0; y--) {
    for(x=w; x>0; x--) {
            data[0] = hi;
            data[1] = lo;

            spi_Transmit(&data[0], 2 ,dataOut );
    }
  }

    CS_NOT = 1;
}

void fillScreen(uint16_t color)
{
  fillRect(0, 0, LCD.LCD_WIDTH, LCD.LCD_HEIGHT, color);
}

void drawPixel(int16_t x, int16_t y, uint16_t color)
{

    uint8_t data[2];
    uint8_t dataOut[2];

  setAddrWindow(y,x,y+1,x+1);

    D_CNOT = 1;
    CS_NOT = 0;

    data[0] = color >> 8;
    data[1] = color;

    spi_Transmit(&data[0], 2 , dataOut);

    CS_NOT = 1;
}

void drawRect(int16_t x,int16_t y,int16_t height,int16_t width,uint16_t color)
{
    //From bottom left
    uint8_t i,j;
    int16_t startX = x;
    int16_t startY = y;

   for(i = startY ; i < startY + height - 1 ; i++ )
   {
       drawPixel(startX, i, color);
       drawPixel(startX + width -1 , i , color);
   }
   for(j = startX ; j < startX + width - 1 ; j++ )
   {
       drawPixel(j , startY , color );
       drawPixel(j , startY + height - 1 , color);
   }

}


void drawChar(char Character) //TODO : Generic Font
{

    uint8_t k,i,j,l;
    uint8_t charNo;
    uint8_t charWidth;
    uint8_t charHeight;
    uint16_t charOffset;
    uint8_t tempX,tempY;
    uint8_t mask = 0x80;
    uint16_t mask16 = 0x8000;
    bool spl;
    tempX = LCD.CURSORX;
    tempY = LCD.CURSORY;
    k = 0;
    l = 0;

    //Find Character Description

    if((uint8_t)Character >= 33)
    {
        charNo = Character - 33;
    }
    else
        charNo  = (uint8_t)Character;

    //TODO : Add a set font function here

    charWidth = calibri_14ptDescriptors[charNo][0];
    font.FONTWIDTH = charWidth;
    charHeight = calibri_14ptDescriptors[charNo][1];
    font.FONTHEIGHT = charHeight;
    charOffset = calibri_14ptDescriptors[charNo][2];

    uint8_t temp[charHeight];
    uint16_t temp16[charHeight];

    if(charWidth > 8)
        spl = true;
    else
        spl = false;

    // Queue character bit map

  if(!spl)
  {
    while(k < charHeight)
    {
        temp[k] = calibri_14ptBitmaps[(charOffset+k)];
        k++;
    }
  }
  else
  {
      while(k < charHeight)
      {

          temp16[k] = ((calibri_14ptBitmaps[(charOffset+l)] << 8) & 0xFF00) + calibri_14ptBitmaps[(charOffset+l+1)];
          k+=1;
          l+=2;
      }
  }
  //Set Bg

  fillRect((LCD.CURSORY-charHeight),LCD.CURSORX,charHeight,(charWidth+1)*LCD.CHARSIZE,LCD.CHARBGCOLOR);

  if(!spl)
  {
    for(i = 0 ; i < charHeight ; i++)
    {

           for(j = 0 ; j < (charWidth + 1) ; j++)
           {
              if(temp[i] & (mask >> j))
              {
                drawPixel(tempX , tempY , LCD.CHARCOLOR);
              }

              tempX += LCD.CHARSIZE ;
           }

           tempX = LCD.CURSORX;
           tempY-=LCD.LINESPACING;
    }
  }
  else
  {
      for(i = 0 ; i < charHeight ; i++)
      {

             for(j = 0 ; j < (charWidth + 1) ; j++)
             {
                if(temp16[i] & (mask16 >> j))
                {
                  drawPixel(tempX , tempY , LCD.CHARCOLOR);
                }

                tempX += LCD.CHARSIZE;
             }

             tempX = LCD.CURSORX;
             tempY-=LCD.LINESPACING;
//             tempY--;
      }

  }

  LCD.CURSORX += ((charWidth*LCD.CHARSIZE) + LCD.CHARSPACING);
}

void drawString(char arr[],size_t size)
{
    uint8_t i;
    for(i = 0; i < size ; i++)
    {
        if(arr[i] == 0x20)
        {
          fillRect((LCD.CURSORY-12),LCD.CURSORX,12,8,LCD.CHARBGCOLOR);
          LCD.CURSORX +=font.FONTWIDTH; //Depends on current font used
        }
        else if(arr[i] == 0x0D)
        {
            LCD.CURSORY-=(font.FONTHEIGHT * LCD.LINESPACING);   //Depends on current font used
        }
        else if(arr[i] == 0x0A)
        {
            LCD.CURSORX = 10; //Original x coordinate
        }
        else
        {
          drawChar(arr[i]);
        }
    }
}


void clearScreen()
{
    fillScreen(ILI9341_BLACK);
}


void moveCursor(uint16_t x , uint16_t y)
{
    if(x < LCD.LCD_WIDTH && y < LCD.LCD_HEIGHT )
    {
       LCD.CURSORX = x;
       LCD.CURSORY = y;
    }
}


void setCharConfig(uint16_t color , uint8_t csize , uint8_t cspace , uint16_t bcolor , uint8_t lspace)
{
    LCD.CHARCOLOR = color;
    LCD.CHARSIZE = csize;
    LCD.CHARSPACING = cspace;
    LCD.CHARBGCOLOR = bcolor;
    LCD.LINESPACING = lspace;
}


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
    drawString("Press Enter to Begin",20);
    drawString("\r\nWelcome\r\n@ILI9341",19);
//    drawString("hariharan@ILI9341:~$ sudo su\r\n",30);
//    drawString("hariharan@ILI9341:~$ Enter Password:\r\n",38);
//    moveCursor(10,50);
//    drawString(">_ ",3);
//    drawString("Hello!",8);
//    drawString("\r\n",2);
//    drawString("Press Enter to Begin!",21);
////    setCharConfig(ILI9341_ORANGE,1,1,ILI9341_DARKGREEN);
//    drawString("\r\n",2);
//    drawString("Commands are ",13);
    while(1);
}
