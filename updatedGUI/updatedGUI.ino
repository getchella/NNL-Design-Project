#include <SPI.h>
#include <Wire.h>
#include <SD.h>
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

int lowTmp, highTmp;
uint8_t screen;
float tempHigh = 120;
float tempLow = -40;        // Celsius by default
float humidityHigh = 100;
float humidityLow = 0;    // %
float pressureHigh;
float pressureLow;    // PSI by default

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
        // Right arrow pressed to move to screen 2
        if ((x > 245 && x < 305) && (y > 185 && y < 225)) {
          setScreen2();
        }        
        break;

      case 2:
        if ((x > 245 && x < 305) && (y > 185 && y < 225)) {
          setScreen3();
        }
        else if ((x > 15 && x < 75) && (y > 185 && y < 225)) {
          setScreen1();
        }

        else if ((x > 30 && x < 145) && (y > 20 && y < 90)) {
          TempScreen();
        }

        else if ((x > 175 && x < 290) && (y > 20 && y < 90)) {
          HumidityScreen();
        }

        else if ((x > 30 && x < 145) && (y > 105 && y < 175)) {
          PressureScreen();
        }

        else if ((x > 175 && x < 290) && (y > 105 && y < 175)) {
          Sensor4Screen();
        }
        break; 

      case 3:
        if ((x > 15 && x < 75) && (y > 185 && y < 225)) {
          setScreen2();
        }

        else if ((x > 30 && x < 145) && (y > 20 && y < 90)) {
          Bluetooth();
        }

        else if ((x > 175 && x < 290) && (y > 20 && y < 90)) {
          SD_Card();
        }
        break;

      case 4:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillScreen(ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen2();  
        }
        else if ((x > 135 && x < 175) && (y > 65 && y < 90)) {
          lowTmp--;
          printLowValue();
        }
        else if ((x > 176 && x < 215) && (y > 65 && y < 90)) {
          lowTmp++;
          printLowValue();
        }
        else if ((x > 219 && x < 258) && (y > 65 && y < 90)) {
          highTmp--;
          printHighValue();
        }
        else if ((x > 259 && x < 269) && (y > 65 && y < 90)) {
          highTmp++;
          printHighValue();
        }
        else if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          tempLow = lowTmp;
          tempHigh = highTmp;
        }
        break;

      case 5:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillScreen(ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen2();  
        }
        else if ((x > 135 && x < 175) && (y > 65 && y < 90)) {
          lowTmp--;
          printLowValue();
        }
        else if ((x > 176 && x < 215) && (y > 65 && y < 90)) {
          lowTmp++;
          printLowValue();
        }
        else if ((x > 219 && x < 258) && (y > 65 && y < 90)) {
          highTmp--;
          printHighValue();
        }
        else if ((x > 259 && x < 269) && (y > 65 && y < 90)) {
          highTmp++;
          printHighValue();
        }
        else if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          tempLow = lowTmp;
          tempHigh = highTmp;
        }
        break;

        case 6:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillScreen(ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen2();  
        }
        else if ((x > 135 && x < 175) && (y > 65 && y < 90)) {
          lowTmp--;
          printLowValue();
        }
        else if ((x > 176 && x < 215) && (y > 65 && y < 90)) {
          lowTmp++;
          printLowValue();
        }
        else if ((x > 219 && x < 258) && (y > 65 && y < 90)) {
          highTmp--;
          printHighValue();
        }
        else if ((x > 259 && x < 269) && (y > 65 && y < 90)) {
          highTmp++;
          printHighValue();
        }
        else if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          tempLow = lowTmp;
          tempHigh = highTmp;
        }
        break;

        case 7:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillScreen(ILI9341_LIGHTGREY);
          createGuiMainFrame();
          setScreen2();  
        }
        else if ((x > 135 && x < 175) && (y > 65 && y < 90)) {
          lowTmp--;
          printLowValue();
        }
        else if ((x > 176 && x < 215) && (y > 65 && y < 90)) {
          lowTmp++;
          printLowValue();
        }
        else if ((x > 219 && x < 258) && (y > 65 && y < 90)) {
          highTmp--;
          printHighValue();
        }
        else if ((x > 259 && x < 269) && (y > 65 && y < 90)) {
          highTmp++;
          printHighValue();
        }
        else if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          tempLow = lowTmp;
          tempHigh = highTmp;
        }
        break;

        case 8:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillScreen(ILI9341_LIGHTGREY);
          createBluetoothSDFrame();
          setScreen3();  
        }
        break;


        case 9:
        if ((x > 15 && x < 75) && (y > 15 && y < 55)){
          tft.fillScreen(ILI9341_LIGHTGREY);
          createBluetoothSDFrame();
          setScreen3();  
        }
        break;
    }
  }

  else {
    switch(screen) {
      case 1: // display value
        break;
      case 4: // Temperature
      case 5: // Humidity
      case 6: // Pressure
      case 7: // Sensor 4
      case 8: // Bluetooth
      case 9: // SD card
        break;
    }
  }

  delay(100);
}

/**
 * Creates 2 main boxes that hold values and 
 **/
void createGuiMainFrame() {
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1-20, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2-20, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1+HeightOfRect-35, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2+HeightOfRect-35, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_BLACK);
  tft.drawCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_BLACK);
  tft.drawCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_BLACK);
}

void createBluetoothSDFrame() {
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1-20, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2-20, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
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

void drawBackArrowBox() {
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
  drawRightArrowBox();
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have more left screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_fill, ILI9341_BLACK);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_fill, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_fill, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1+HeightOfRect-34, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2+HeightOfRect-34, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  /*
  tft.setCursor(65, 80);
  tft.setTextSize(2);
  tft.print("Temp");
  tft.setCursor(185, 80);
  tft.setTextSize(2);
  tft.print("Humidity"); */
  screen = 1;
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
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1+HeightOfRect-35, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2+HeightOfRect-35, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);

  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1+HeightOfRect-34, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2+HeightOfRect-34, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  /*
  tft.setCursor(40, 80);
  tft.setTextSize(2.5);
  tft.print("Pressure");
  tft.setCursor(195, 80);
  tft.setTextSize(2.5);
  tft.print("Sensor 4"); */
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
  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1+HeightOfRect-35, LengthOfRect*2+30, HeightOfRect-30, ILI9341_LIGHTGREY);
  /*
  tft.setCursor(35, 80);
  tft.setTextSize(2.5);
  tft.println("Bluetooth");
  tft.setCursor(190, 80);
  tft.setTextSize(2.5);
  tft.println("SD_Card"); */
}

void TempScreen() {
  screen = 4;
  lowTmp = (int) tempLow;
  highTmp = (int) tempHigh;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("TEMPERATURE"); 
  printLowValue();
  printHighValue();
}

void HumidityScreen() {
  screen = 5;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("HUMIDITY");
}

void PressureScreen() {
  screen = 6;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("PRESSURE");
}

void Sensor4Screen() {
  screen = 7;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("SENSOR 4");
}

void setValueScreen() {
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillCircle(x_Coordinate_Circle1, y_Coordinate_Circle1, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle2, y_Coordinate_Circle2, Circle_size, ILI9341_LIGHTGREY);
  tft.fillCircle(x_Coordinate_Circle3, y_Coordinate_Circle3, Circle_size, ILI9341_LIGHTGREY);  //get rid of the circles
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1-20, LengthOfRect*2+30, 170, ILI9341_LIGHTGREY);                           // *****************************************
  drawBackArrowBox(); //draw back arrow
  tft.drawRect(15, 65, 60, 40, ILI9341_BLACK);
  tft.fillRect(16, 66, 58, 38, ILI9341_BLUE);
  tft.setCursor(28, 79);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.print("SET");
  tft.setCursor(135, 15);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("ADJUST SCALING");
  tft.drawRect(135, 35, 165, 30, ILI9341_BLACK);
  tft.drawFastVLine(217, 36, 28, ILI9341_BLACK);
  tft.drawRect(135, 65, 165, 30, ILI9341_BLACK);
  tft.drawFastVLine(216, 66, 28, ILI9341_BLACK);
  tft.drawFastVLine(217, 66, 28, ILI9341_BLACK);
  tft.drawFastVLine(218, 66, 28, ILI9341_BLACK);
  tft.drawFastVLine(176, 66, 28, ILI9341_BLACK);
  tft.drawFastVLine(258, 66, 28, ILI9341_BLACK);
  tft.fillRect(136, 66, 40, 28, ILI9341_RED);
  tft.fillRect(177, 66, 39, 28, ILI9341_DARKGREEN);
  tft.fillRect(219, 66, 39, 28, ILI9341_RED);
  tft.fillRect(259, 66, 40, 28, ILI9341_DARKGREEN);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(152, 72);
  tft.print('-');
  tft.setCursor(193, 72);
  tft.print('+');
  tft.setCursor(235, 72);
  tft.print('-');
  tft.setCursor(275, 72);
  tft.print('+');
}

void printLowValue() {
  tft.fillRect(152, 40, 46, 15, ILI9341_LIGHTGREY);
  tft.setCursor(152, 40);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.print(lowTmp);
}

void printHighValue() {
  tft.fillRect(240, 40, 46, 15, ILI9341_LIGHTGREY);
  tft.setCursor(240, 40);
  tft.setTextColor(ILI9341_BLACK);
  tft.setTextSize(2);
  tft.print(highTmp);
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
  drawBackArrowBox(); //draw back arrow
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
  drawBackArrowBox(); //draw back arrow
  //tft.drawRect(x_Coordinate_Rect1 + 50, y_Coordinate_Rect1 + 30, LengthOfRect + 50, HeightOfRect + 50, ILI9341_NAVY);
}

float scale(float high, float low, float value)
{
   float b = -(high-5*low)/4; //intercept calculation
   float m = (high-b)/1023; //slope calculation

   float scaled = (m*value) + b; //linear calculation of scaled input

   return scaled; //output scaled value
}

bool errorcheck(float value)
{
   if(value*5/(1023) < 0.875 )
    {
      return 0;
    }
    else 
    {
      return 1;
    }
}
