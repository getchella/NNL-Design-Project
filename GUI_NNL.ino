#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <stdint.h>

#define LengthOfRect          115
#define HeightOfRect          100
#define x_Coordinate_Rect1    30
#define y_Coordinate_Rect1    40
#define x_Coordinate_Rect2    175
#define y_Coordinate_Rect2    40

#define Circle_fill           4
#define Circle_size           5
#define x_Coordinate_Circle1  140
#define y_Coordinate_Circle1  220
#define x_Coordinate_Circle2  160
#define y_Coordinate_Circle2  220
#define x_Coordinate_Circle3  180
#define y_Coordinate_Circle3  220

#define WIDTH     320
#define HEIGHT    240
#define TFT_RST   8
#define TFT_DC    9
#define TFT_CS    10
#define TFT_MOSI  11
#define TFT_MISO  12
#define TFT_CLK   13

// utilizes SPI on Arduino to output data to touchscreen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_MOSI, TFT_CLK, TFT_RST, TFT_MISO);
// utilizes I2C on Arduino to get touch screen points
Adafruit_FT6206 ts = Adafruit_FT6206();

uint8_t screen;

void setup() {
  Serial.begin(9600);
  tft.begin();
  if (! ts.begin(40)) {  // pass in 'sensitivity' coefficient
    Serial.println("Couldn't start FT6206 touchscreen controller");
    while (1);
  }

  tft.setRotation(3);   // rotates the screen to horizontal
  tft.fillScreen(ILI9341_LIGHTGREY);   // sets background to gray

  createGuiMainFrame();
  setScreen1();  
}

void loop() {
  
  if (ts.touched()) {   
    // Retrieve a point  
    TS_Point p = ts.getPoint(); 
    // rotate coordinate system
    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    int x = WIDTH - p.y;
    int y = p.x;

    Serial.print("("); Serial.print(x);
    Serial.print(", "); Serial.print(y);
    Serial.println(")");
   
    switch(screen) {
      case 1:
        if ((x > 245 && x < 305) && (y > 185 && y < 225)) {
          setScreen2();
        }

        else if ((x > 30 && x < 145) && (y > 40 && y < 140)) {
          TempScreen();
        }

        else if ((x > 175 && x < 290) && (y > 40 && y < 140)) {
          HumidityScreen();
        }
        break;

      case 2:
        if ((x > 245 && x < 305) && (y > 185 && y < 225)) {
          setScreen3();
        }
        else if ((x > 15 && x < 75) && (y > 185 && y < 225)) {
          setScreen1();
        }

        else if ((x > 30 && x < 145) && (y > 40 && y < 140)) {
          Sensor3Screen();
        }

        else if ((x > 175 && x < 290) && (y > 40 && y < 140)) {
          Sensor4Screen();
        }
        break; 

      case 3:
        if ((x > 15 && x < 75) && (y > 185 && y < 225)) {
          setScreen2();
        }

        else if ((x > 30 && x < 145) && (y > 40 && y < 140)) {
          Bluetooth();
        }

        else if ((x > 175 && x < 290) && (y > 40 && y < 140)) {
          SD_Card();
        }
        break;

      case 4:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_LIGHTGREY);
          tft.fillRect(15, 15, 60, 40, ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen1();  
        }
        break;

      case 5:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_LIGHTGREY);
          tft.fillRect(15, 15, 60, 40, ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen1();  
        }
        break;

        case 6:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_LIGHTGREY);
          tft.fillRect(15, 15, 60, 40, ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen2();  
        }
        break;

        case 7:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_LIGHTGREY);
          tft.fillRect(15, 15, 60, 40, ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen2();  
        }
        break;

        case 8:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_LIGHTGREY);
          tft.fillRect(15, 15, 60, 40, ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen3();  
        }
        break;


        case 9:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_LIGHTGREY);
          tft.fillRect(15, 15, 60, 40, ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen3();  
        }
        break;
    }
  }

  delay(100);
}

/**
 * Creates 2 main boxes that hold values and 
 **/
void createGuiMainFrame() {
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_NAVY);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_NAVY);
  tft.drawCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_BLACK);
  tft.drawCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_BLACK);
  tft.drawCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_BLACK);
}

/**
 * Creates right arrow box to move to a different screen
 **/
void drawRightArrowBox() {
  tft.drawRect(245, 185, 60, 40, ILI9341_BLACK);
  tft.fillRect(246, 186, 58, 38, ILI9341_RED);
  tft.drawFastHLine(260, 205, 31, ILI9341_WHITE);
  
  int x, y_low, y_high;
  for (x=289, y_low=204, y_high=206; x>=285 && y_low>=200 && y_high<=210; x--, y_low--, y_high++) {
    tft.drawPixel(x, y_low, ILI9341_WHITE);
    tft.drawPixel(x, y_high, ILI9341_WHITE);  
  }
}

/**
 * Creates left arrow box to move to a different screen
 */
void drawLeftArrowBox() {
  tft.drawRect(15, 185, 60, 40, ILI9341_BLACK);
  tft.fillRect(16, 186, 58, 38, ILI9341_RED);
  tft.drawFastHLine(29, 205, 31, ILI9341_WHITE);

  int x, y_low, y_high;
  for (x=30, y_low=204, y_high=206; x<=34 && y_low>=200 && y_high<=210; x++, y_low--, y_high++) {
    tft.drawPixel(x, y_low, ILI9341_WHITE);
    tft.drawPixel(x, y_high, ILI9341_WHITE);
  }
}

void drawBacktArrowBox() {
  tft.drawRect(15, 15, 60, 40, ILI9341_BLACK);
  tft.fillRect(16, 16, 58, 38, ILI9341_RED);
  tft.drawFastHLine(29, 35, 31, ILI9341_WHITE);

  int x, y_low, y_high;
  for (x=30, y_low=34, y_high=36; x<=34 && y_low>=30 && y_high<=40; x++, y_low--, y_high++) {
    tft.drawPixel(x, y_low, ILI9341_WHITE);
    tft.drawPixel(x, y_high, ILI9341_WHITE);
  }
}

/**
 * Displays screen 1 which displays humidity and temperature
 **/
void setScreen1() {
  screen = 1;
  drawRightArrowBox();
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have more left screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_fill, ILI9341_BLACK);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_fill, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_fill, ILI9341_LIGHTGREY);
}

/**
 * Displays screen 2
 **/
void setScreen2() {
  screen = 2;
  drawLeftArrowBox();
  drawRightArrowBox();
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_fill, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_fill, ILI9341_BLACK);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_fill, ILI9341_LIGHTGREY);
}

/**
 * Displays screen 3
 **/
void setScreen3() {
  screen = 3;
  drawLeftArrowBox();
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_fill, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_fill, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_fill, ILI9341_BLACK);
}

void TempScreen() {
  screen = 4;
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  drawBacktArrowBox(); //draw back arrow
  tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
  
}

void HumidityScreen() {
  screen = 5;
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  drawBacktArrowBox(); //draw back arrow
  tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
}

void Sensor3Screen() {
  screen = 6;
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  drawBacktArrowBox(); //draw back arrow
  tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
}

void Sensor4Screen() {
  screen = 7;
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  drawBacktArrowBox(); //draw back arrow
  tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
}

void Bluetooth() {
  screen = 8;
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  drawBacktArrowBox(); //draw back arrow
  //tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
}

void SD_Card() {
  screen = 9;
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect2, y_Coordinate_Rect2, LengthOfRect, HeightOfRect, ILI9341_LIGHTGREY);
  drawBacktArrowBox(); //draw back arrow
  //tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
}

