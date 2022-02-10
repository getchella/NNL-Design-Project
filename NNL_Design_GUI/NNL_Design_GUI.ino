#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"

#define WIDTH     320
#define HEIGHT    240
#define TFT_RST   8
#define TFT_DC    9
#define TFT_CS    10
#define TFT_MOSI  11
#define TFT_MISO  12
#define TFT_CLK   13

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);

void setup() {
  tft.begin();

  tft.setRotation(3);   // rotates the screen to horizontal

  tft.fillScreen(ILI9341_LIGHTGREY);   // sets background to gray

  tft.drawRect(50, 50, 100, 100, ILI9341_NAVY);
  
}

void loop() {
  // put your main code here, to run repeatedly:

}
