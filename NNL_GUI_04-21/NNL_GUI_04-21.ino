#include <SPI.h>
#include <Wire.h>
#include <SD.h>
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>

#define    F_CPU   16000000UL

// Screen numbers
#define SCREEN1       1
#define SCREEN2       2
#define TEMP          3
#define HUMIDITY      4
#define PRESSURE      5
#define VOLT_CURRENT  6

// dimensions of objects on screen
#define LengthOfRect          115
#define HeightOfRect          100
#define x_Coordinate_Rect1    30
#define y_Coordinate_Rect1    40
#define x_Coordinate_Rect2    175
#define y_Coordinate_Rect2    40
#define WIDTH     320
#define HEIGHT    240

// Pin numbers
#define TFT_RST   PB2
#define TFT_DC    PB3
#define TFT_CS    PB4
#define TFT_MOSI  PB5
#define TFT_MISO  PB6
#define TFT_CLK   PB7
#define SD_CS     PD2

const uint16_t t1_load = 0;
const uint16_t t1_comp = 62500;

// utilizes SPI on Arduino to output data to touchscreen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// utilizes I2C on Arduino to get touch screen points
Adafruit_FT6206 ts = Adafruit_FT6206();

int lowTmp, highTmp;
char tempUnit = 'C';
char humidityUnit = '%';
String pressureUnit = String("PSI");
String voltCurrentUnit = String('V');
uint8_t screen;
float tempValue, humidityValue, pressureValue, voltCurrentValue;
float tempHigh = 80;
float tempLow = -40;        // Celsius by default
bool tempFlag = false;        //enable and disable channel streaming
float humidityHigh = 100;
float humidityLow = 0;      // %
bool humidityFlag = false;    //enable and disable channel streaming
float pressureHigh = 100;
float pressureLow = 0;      // PSI by default
bool pressureFlag = false;    //enable and disable channel streaming
float voltCurrentHigh = 5;
float voltCurrentLow = 1;
bool voltCurrentFlag = false; //enable and disable channel streaming
bool sdFlag = false;
File dataFile;

void setup() {
  SD.begin(SD_CS);
  SD_Init();
  tft.begin(4000000);   // Begin GUI, 4 MHz SPI clk frequency
  ts.begin(40);         // Begin touchscreen, pass in sensitivity coefficient 
  
  tft.setRotation(3);   // rotates the screen to horizontal position
  tft.fillScreen(ILI9341_LIGHTGREY);   // sets background to gray

  setScreen1();   // sets screen to main screen

  TCCR1A = 0;   // reset timer1 control reg A
  // set to prescaler of 256
  TCCR1B |= (1<<CS12);
  TCCR1B &= ~(1<<CS11);
  TCCR1B &= ~(1<<CS10);
  // reset timer1 and set compare value
  TCNT1 = t1_load;
  OCR1A = t1_comp;
  TIMSK1 = (1<<OCIE1A);   // enable timer1 compare interrupt
  sei();    // enables global interrupts
  
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = t1_load;
  if (screen == SCREEN1) {
    if(tempFlag) {
      tempValue = analogRead(0);
      tft.fillRect(40, 45, 90, 22, ILI9341_WHITE);
      Display_Value(tempHigh, tempLow, tempValue, 2, 40, 45);                 // display temperature value
    }
    if(humidityFlag) {
      humidityValue = analogRead(1);
      tft.fillRect(185, 45, 90, 22, ILI9341_WHITE);
      Display_Value(humidityHigh, humidityLow, humidityValue, 2, 185, 45);    // display humidity value
    }
    if(pressureFlag) {
      pressureValue = analogRead(2);
      tft.fillRect(40, 130, 90, 22, ILI9341_WHITE);
      Display_Value(pressureHigh, pressureLow, pressureValue, 2, 40, 130);    // display pressure value
    }
    if(voltCurrentFlag) {
      voltCurrentValue = analogRead(3);
      tft.fillRect(185, 130, 90, 22, ILI9341_WHITE);
      Display_Value(voltCurrentHigh, voltCurrentLow, voltCurrentValue, 2, 185, 130);   // display volt/current value
    }
  }
}

void loop() {
  /*
  delay(1);
  if (sdFlag) {
    int dataSent = 0;
    if (tempFlag) {
      tempValue = analogRead(0);
      if(errorcheck(tempValue)) {
        cli();
        tft.setCursor(0,0);
        tft.setTextSize(1);
        tft.print("SD");
        SD.open("temp.txt", FILE_WRITE);
        dataFile.println("ERROR");
        dataFile.close();
        sei();
      }
      else {
        tempValue = scale(tempHigh, tempLow, tempValue);
        cli();
        dataFile = SD.open("temp.txt", FILE_WRITE);
        for (int i = 0; i < 50; i++) {
          dataSent = dataFile.println(tempValue);
          if (dataSent > 0) break;
        }
        while (dataFile) {
          dataFile.close();
        }
        sei();
      }
    }
    dataSent = 0;
    if (humidityFlag) {
      humidityValue = analogRead(1);
      if(errorcheck(humidityValue)) {
        cli();
        SD.open("humid.txt", FILE_WRITE);
        dataFile.println("ERROR");
        dataFile.close();
        sei();
      }
      else {
        humidityValue = scale(humidityHigh, humidityLow, humidityValue);
        cli();
        dataFile = SD.open("humid.txt", FILE_WRITE);
        dataFile.println(humidityValue);
        dataFile.close();
        sei();
      }
    }
    dataSent = 0;
    if (pressureFlag) {
      pressureValue = analogRead(2);
      if(errorcheck(pressureValue)) {
        cli();
        SD.open("press.txt", FILE_WRITE);
        dataFile.println("ERROR");
        dataFile.close();
        sei();
      }
      else {
        pressureValue = scale(pressureHigh, pressureLow, pressureValue);
        cli();
        dataFile = SD.open("press.txt", FILE_WRITE);
        dataFile.println(pressureValue);
        dataFile.close();
        sei();
      }
    }
    dataSent = 0;
    if (voltCurrentFlag) {
      voltCurrentValue = analogRead(3);
      if(errorcheck(voltCurrentValue)) {
        cli();
        SD.open("volcur.txt", FILE_WRITE);
        dataFile.println("ERROR");
        dataFile.close();
        sei();
      }
      else {
        voltCurrentValue = scale(tempHigh, tempLow, tempValue);
        cli();
        dataFile = SD.open("volcur.txt", FILE_WRITE);
        dataFile.println(humidityValue);
        dataFile.close();
        sei();
      }
    }
  }
  */
  if (ts.touched()) {
    // Retrieve a point  
    TS_Point p = ts.getPoint(); 
    // rotate coordinate system
    // flip it around to match the screen.
    p.x = map(p.x, 0, 240, 240, 0);
    p.y = map(p.y, 0, 320, 320, 0);
    int x = WIDTH - p.y;
    int y = p.x;
  
    switch(screen) {
      case SCREEN1:
        // Right arrow pressed to move to screen 2
        if ((x > 245 && x < 305) && (y > 185 && y < 225)) {
          cli();
          setScreen2();
          sei();
        }
        else if ((x > 30 && x < 145) && (y > 20 && y < 90)) {
          cli();
          TempScreen();
          sei();
        }
        else if ((x > 175 && x < 290) && (y > 20 && y < 90)) {
          cli();
          HumidityScreen();
          sei();
        }
        else if ((x > 30 && x < 145) && (y > 105 && y < 175)) {
          cli();
          PressureScreen();
          sei();
        }
        else if ((x > 175 && x < 290) && (y > 105 && y < 175)) {
          cli();
          VoltCurrentScreen();
          sei();
        }   
        break; 

      case SCREEN2:
        if ((x > 15 && x < 75) && (y > 185 && y < 225)) {
          cli();
          tft.fillRect(10, 29, 350, 100, ILI9341_LIGHTGREY);
          setScreen1();
          sei();
        }
        else if((x > 135 && x < 215) && (y > 30 && y < 60)){
          tft.fillRect(136, 31, 81, 28, ILI9341_DARKGREEN);
          tft.fillRect(218, 31, 81, 28, ILI9341_RED);
          tft.setTextColor(ILI9341_WHITE);
          tft.setCursor(167, 38);
          tft.print("ON");
          tft.setCursor(245, 38);
          tft.print("OFF");
          //enable sd card
          sdFlag = true;
        }
        else if ((x > 215 && x < 295) && (y > 30 && y < 60)){
          tft.fillRect(136, 31, 81, 28, ILI9341_RED);
          tft.fillRect(218, 31, 81, 28, ILI9341_DARKGREEN);
          tft.setTextColor(ILI9341_WHITE);
          tft.setCursor(167, 38);
          tft.print("ON");
          tft.setCursor(245, 38);
          tft.print("OFF");
          //disbale sd card
          sdFlag = false;
        }
        break;

      case TEMP:
        cli();
        checkScaleButtonPress(x, y);
        sei();
        if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          tempLow = lowTmp;
          tempHigh = highTmp;
          cli();
          tft.fillScreen(ILI9341_LIGHTGREY);
          setScreen1();
          sei();
        }
        else if((x > 135 && x < 215) && (y > 150 && y < 180)){
          tft.fillRect(136, 151, 81, 28, ILI9341_DARKGREEN);
          tft.fillRect(218, 151, 81, 28, ILI9341_RED);
          tft.setTextColor(ILI9341_WHITE);
          tft.setCursor(172, 158);
          tft.print('C');
          tft.setCursor(255, 158);
          tft.print('F');
          tempUnit = 'C';
        }
        else if ((x > 215 && x < 295) && (y > 150 && y < 180)){
          tft.fillRect(136, 151, 81, 28, ILI9341_RED);
          tft.fillRect(218, 151, 81, 28, ILI9341_DARKGREEN);
          tft.setTextColor(ILI9341_WHITE);
          tft.setCursor(172, 158);
          tft.print('C');
          tft.setCursor(255, 158);
          tft.print('F');
          tempUnit = 'F';
        }
        else if((x > 15 && x < 75) && (y > 115 && y < 155) && tempFlag == 0){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2.5);
          tft.print("ON");
          tempFlag = 1;
        }
        else if((x > 15 && x < 75) && (y > 115 && y < 155) && tempFlag == 1){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_RED);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2.5);
          tft.print("OFF");
          tempFlag = 0;
        }
        break;

      case HUMIDITY:
        cli();
        checkScaleButtonPress(x, y);
        sei();
        if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          humidityLow = lowTmp;
          humidityHigh = highTmp;
          cli();
          tft.fillScreen(ILI9341_LIGHTGREY);
          setScreen1();
          sei();
        }
        else if((x > 15 && x < 75) && (y > 115 && y < 155) && humidityFlag == 0){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2);
          tft.print("ON");
          humidityFlag = 1;
        }
        else if((x > 15 && x < 75) && (y > 115 && y < 155) && humidityFlag == 1){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_RED);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2);
          tft.print("OFF");
          humidityFlag = 0;
        }
        break;

      case PRESSURE:
        cli();
        checkScaleButtonPress(x, y);
        sei();
        if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          pressureLow = lowTmp;
          pressureHigh = highTmp;
          cli();
          tft.fillScreen(ILI9341_LIGHTGREY);
          setScreen1();
          sei();
        }
        else if((x > 135 && x < 215) && (y > 150 && y < 180)){
          tft.fillRect(136, 151, 81, 28, ILI9341_DARKGREEN);
          tft.fillRect(218, 151, 81, 28, ILI9341_RED);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2);
          tft.setCursor(160, 158);
          tft.print("PSI");
          tft.setCursor(250, 158);
          tft.print("Pa");
          pressureUnit = String("PSI"); 
        }
        else if ((x > 215 && x < 295) && (y > 150 && y < 180)){
          tft.fillRect(136, 151, 81, 28, ILI9341_RED);
          tft.fillRect(218, 151, 81, 28, ILI9341_DARKGREEN);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2);
          tft.setCursor(160, 158);
          tft.print("PSI");
          tft.setCursor(250, 158);
          tft.print("Pa");
          pressureUnit = String("Pa"); 
        }
        else if((x > 15 && x < 75) && (y > 115 && y < 155) && pressureFlag == 0){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2.5);
          tft.print("ON");
          pressureFlag = 1;
        }
        else if((x > 15 && x < 75) && (y > 115 && y < 155) && pressureFlag == 1){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_RED);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2.5);
          tft.print("OFF");
          pressureFlag = 0;
        }
        break;

      case VOLT_CURRENT:
        cli();
        checkScaleButtonPress(x, y);
        sei();
        if ((x > 15 && x < 75) && (y > 65 && y < 105)) {
          voltCurrentLow = lowTmp;
          voltCurrentHigh = highTmp;
          cli();
          tft.fillScreen(ILI9341_LIGHTGREY);
          setScreen1();
          sei();
        }
        else if((x > 135 && x < 215) && (y > 150 && y < 180)){
          tft.fillRect(136, 151, 81, 28, ILI9341_DARKGREEN);
          tft.fillRect(218, 151, 81, 28, ILI9341_RED);
          tft.setTextColor(ILI9341_WHITE);
          tft.setCursor(172, 158);
          tft.print('V');
          tft.setCursor(255, 158);
          tft.print("mA");
          voltCurrentUnit = 'V';
        }
        else if ((x > 215 && x < 295) && (y > 150 && y < 180)){
          tft.fillRect(136, 151, 81, 28, ILI9341_RED);
          tft.fillRect(218, 151, 81, 28, ILI9341_DARKGREEN);
          tft.setTextColor(ILI9341_WHITE);
          tft.setCursor(172, 158);
          tft.print('V');
          tft.setCursor(250, 158);
          tft.print("mA");
          voltCurrentUnit = "mA";
        }

        else if((x > 15 && x < 75) && (y > 115 && y < 155) && voltCurrentFlag == 0){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2.5);
          tft.print("ON");
          voltCurrentFlag = 1;
        }

        else if((x > 15 && x < 75) && (y > 115 && y < 155) && voltCurrentFlag == 1){
          tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
          tft.fillRect(16, 116, 58, 38, ILI9341_RED);
          tft.setCursor(33, 130);
          tft.setTextColor(ILI9341_WHITE);
          tft.setTextSize(2.5);
          tft.print("OFF");
          voltCurrentFlag = 0;
        }
        break;
    }
    delay(100);
  }
}

/**
 * Creates right arrow box to move to a different screen
 **/
void drawRightArrowBox() {
  tft.drawRect(245, 185, 60, 40, ILI9341_BLACK);
  tft.fillRect(246, 186, 58, 38, ILI9341_RED);
  //tft.drawFastHLine(260, 205, 31, ILI9341_WHITE);
  
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
  //tft.drawFastHLine(29, 205, 31, ILI9341_WHITE);

  int x, y_low, y_high;
  for (x=30, y_low=204, y_high=206; x<=34 && y_low>=200 && y_high<=210; x++, y_low--, y_high++) {
    tft.drawPixel(x, y_low, ILI9341_WHITE);
    tft.drawPixel(x, y_high, ILI9341_WHITE);
  }
}

void drawBackArrowBox() {
  tft.drawRect(15, 15, 60, 40, ILI9341_BLACK);
  tft.fillRect(16, 16, 58, 38, ILI9341_RED);
  //tft.drawFastHLine(29, 35, 31, ILI9341_WHITE);

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
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1-20, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2-20, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect1, y_Coordinate_Rect1+HeightOfRect-35, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.drawRect(x_Coordinate_Rect2, y_Coordinate_Rect2+HeightOfRect-35, LengthOfRect, HeightOfRect-30, ILI9341_BLACK);
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have more left screens
  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1+HeightOfRect-34, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2+HeightOfRect-34, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(55, 25);
  tft.print("TEMPERATURE");
  tft.setCursor(207, 25);
  tft.print("HUMIDITY");
  tft.setCursor(65, 110);
  tft.print("PRESSURE");
  tft.setCursor(189, 110);
  tft.print("VOLTAGE/CURRENT");

  tft.setTextSize(1);
  tft.setCursor(122, 67);
  tft.print("o");
  tft.setTextSize(2);
  tft.setCursor(129, 70);
  tft.print(tempUnit);

  if (pressureUnit == "PSI"){
    tft.setCursor(107, 155);
    tft.print(pressureUnit);
  }
  else if (pressureUnit == "Pa"){
    tft.setCursor(120, 155);
    tft.print(pressureUnit);
  }

  tft.setCursor(272, 70);
  tft.print(humidityUnit);

  tft.setCursor(272, 155);
  tft.print(voltCurrentUnit);
  
  screen = SCREEN1;
}

/**
 * Displays screen 3
 **/
void setScreen2() {
  screen = SCREEN2;
  //tft.fillScreen(ILI9341_LIGHTGREY);   // sets background to gray
  drawLeftArrowBox();
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);
  //tft.fillRect(x_Coordinate_Rect1+1, y_Coordinate_Rect1-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  //tft.fillRect(x_Coordinate_Rect2+1, y_Coordinate_Rect2-19, LengthOfRect-2, HeightOfRect-32, ILI9341_WHITE);
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1-20, LengthOfRect*2+30, HeightOfRect-30, ILI9341_LIGHTGREY);
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1+HeightOfRect-35, LengthOfRect*2+30, HeightOfRect-30, ILI9341_LIGHTGREY);

  tft.setTextColor(ILI9341_BLACK);
  tft.setCursor(34, 38);
  tft.drawRect(15, 30, 120, 30, ILI9341_BLACK);
  tft.fillRect(16, 31, 119, 28, ILI9341_WHITE);
  tft.print("SD Card");
  tft.drawRect(135, 30, 165, 30, ILI9341_BLACK);
  tft.drawFastVLine(217, 31, 28, ILI9341_BLACK);
  if (sdFlag) {
    tft.fillRect(136, 31, 81, 28, ILI9341_DARKGREEN);
    tft.fillRect(218, 31, 81, 28, ILI9341_RED);
  }
  else {
    tft.fillRect(136, 31, 81, 28, ILI9341_RED);
    tft.fillRect(218, 31, 81, 28, ILI9341_DARKGREEN);
  }
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(167, 38);
  tft.print("ON");
  tft.setCursor(245, 38);
  tft.print("OFF");
}

void TempScreen() {
  screen = TEMP;
  lowTmp = (int) tempLow;
  highTmp = (int) tempHigh;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("TEMPERATURE"); 
  printLowValue();
  printHighValue();

  if(tempFlag == 0){
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_RED);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("OFF");
  }

  else if (tempFlag == 1)
  {
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("ON");
  }

}

void HumidityScreen() {
  screen = HUMIDITY;
  lowTmp = (int) humidityLow;
  highTmp = (int) humidityHigh;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("HUMIDITY");
  printLowValue();
  printHighValue();

  if(humidityFlag == 0){
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_RED);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("OFF");
  }

  else if (humidityFlag == 1)
  {
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("ON");
  }
}

void PressureScreen() {
  screen = PRESSURE;
  lowTmp = (int) pressureLow;
  highTmp = (int) pressureHigh;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("PRESSURE");
  printLowValue();
  printHighValue();

  if(pressureFlag == 0){
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_RED);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("OFF");
  }

  else if (pressureFlag == 1)
  {
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("ON");
  }
}

void VoltCurrentScreen() {
  screen = VOLT_CURRENT;
  lowTmp = (int) voltCurrentLow;
  highTmp = (int) voltCurrentHigh;
  setValueScreen();
  tft.setCursor(5, 230);
  tft.setTextSize(1);
  tft.setTextColor(ILI9341_BLACK);
  tft.print("VOLTAGE/CURRENT");
  printLowValue();
  printHighValue();

  if(voltCurrentFlag == 0){
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_RED);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("OFF");
  }

  else if (voltCurrentFlag == 1)
  {
    tft.drawRect(15, 115, 60, 40, ILI9341_BLACK);
    tft.fillRect(16, 116, 58, 38, ILI9341_DARKGREEN);
    tft.setCursor(33, 128);
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2.5);
    tft.print("ON");
  }
}

void setValueScreen() {
  tft.fillRect(15, 185, 60, 40, ILI9341_LIGHTGREY);   // get rid of left arrow box since screen does not have left screens
  tft.fillRect(245, 185, 60, 40, ILI9341_LIGHTGREY);  // get rid of right arrow box since screen does not have right screens
  tft.fillRect(x_Coordinate_Rect1, y_Coordinate_Rect1-20, LengthOfRect*2+30, 170, ILI9341_LIGHTGREY);
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

  if (screen == TEMP)
  {
    if(tempUnit == 'C'){
      tft.setCursor(188, 130);
      tft.setTextColor(ILI9341_BLACK);
      tft.print("UNITS");
      tft.drawRect(135, 150, 165, 30, ILI9341_BLACK);
      tft.drawFastVLine(217, 151, 28, ILI9341_BLACK);
      tft.fillRect(136, 151, 81, 28, ILI9341_DARKGREEN);
      tft.fillRect(218, 151, 81, 28, ILI9341_RED);
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(172, 158);
      tft.print('C');
      tft.setCursor(255, 158);
      tft.print('F');
    }

    else if(tempUnit == 'F'){
      tft.setCursor(188, 130);
      tft.setTextColor(ILI9341_BLACK);
      tft.print("UNITS");
      tft.drawRect(135, 150, 165, 30, ILI9341_BLACK);
      tft.drawFastVLine(217, 151, 28, ILI9341_BLACK);
      tft.fillRect(136, 151, 81, 28, ILI9341_RED);
      tft.fillRect(218, 151, 81, 28, ILI9341_DARKGREEN);
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(172, 158);
      tft.print('C');
      tft.setCursor(255, 158);
      tft.print('F');
    }
  }
  else if (screen == PRESSURE) {
    if (pressureUnit == "PSI") {
      tft.setCursor(188, 130);
      tft.setTextColor(ILI9341_BLACK);
      tft.print("UNITS");
      tft.drawRect(135, 150, 165, 30, ILI9341_BLACK);
      tft.drawFastVLine(217, 151, 28, ILI9341_BLACK);
      tft.fillRect(136, 151, 81, 28, ILI9341_DARKGREEN);
      tft.fillRect(218, 151, 81, 28, ILI9341_RED);
      tft.setTextColor(ILI9341_WHITE);
      tft.setCursor(160, 158);
      tft.print("PSI");
      tft.setCursor(250, 158);
      tft.print("Pa");
    }
    else if (pressureUnit == "Pa") {
      tft.setCursor(188, 130);
      tft.setTextColor(ILI9341_BLACK);
      tft.print("UNITS");
      tft.drawRect(135, 150, 165, 30, ILI9341_BLACK);
      tft.drawFastVLine(217, 151, 28, ILI9341_BLACK);
      tft.fillRect(136, 151, 81, 28, ILI9341_RED);
      tft.fillRect(218, 151, 81, 28, ILI9341_DARKGREEN);
      tft.setTextColor(ILI9341_WHITE);
      tft.setTextSize(2);
      tft.setCursor(160, 158);
      tft.print("PSI");
      tft.setCursor(250, 158);
      tft.print("Pa");
    }
  }
  else if (screen == VOLT_CURRENT) {
    tft.setCursor(188, 130);
    tft.setTextColor(ILI9341_BLACK);
    tft.print("UNITS");
    tft.drawRect(135, 150, 165, 30, ILI9341_BLACK);
    tft.drawFastVLine(217, 151, 28, ILI9341_BLACK);
    tft.fillRect(136, 151, 81, 28, ILI9341_RED);
    tft.fillRect(218, 151, 81, 28, ILI9341_DARKGREEN);
    tft.setTextColor(ILI9341_WHITE);
    tft.setCursor(172, 158);
    tft.print('V');
    tft.setCursor(250, 158);
    tft.print("mA");
  }
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

//read, convert then display the temperature to the screen
void Display_Value(float high, float low, float value, int textSize, int x, int y) {
  float val;     //store the temperature
  if (errorcheck(value))
  {
    val  = scale(high, low, value); //run the math to convert the voltage to temperature value
    tft.setCursor(x, y);
    tft.setTextSize(textSize);
    tft.setTextColor(ILI9341_BLACK);
    tft.print(val);       //display the temperature to the screen
  }
  else {
    tft.setCursor(x, y);
    tft.setTextSize(3);
    tft.setTextColor(ILI9341_RED);
    tft.print("ERROR");
  }
}

void SD_Init() {
  dataFile = SD.open("temp.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Temperature");
    dataFile.close();
  }
  for (int i = 0; i < 10; i++) {
    tempValue = analogRead(0);
    if(!errorcheck(tempValue)) {
      dataFile = SD.open("temp.txt", FILE_WRITE);
      dataFile.println("ERROR");
      dataFile.close();
    }
    else {
      tempValue = scale(tempHigh, tempLow, tempValue);
      dataFile = SD.open("temp.txt", FILE_WRITE);
      dataFile.println(tempValue);
      dataFile.close();
    }
  }

  dataFile = SD.open("humid.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Humidity");
    dataFile.close();
  }
  for (int i = 0; i < 10; i++) {
    humidityValue = analogRead(1);
    if(!errorcheck(humidityValue)) {
      dataFile = SD.open("humid.txt", FILE_WRITE);
      dataFile.println("ERROR");
      dataFile.close();
    }
    else {
      humidityValue = scale(humidityHigh, humidityLow, humidityValue);
      dataFile = SD.open("humid.txt", FILE_WRITE);
      dataFile.println(humidityValue);
      dataFile.close();
    }
  }
  
  dataFile = SD.open("press.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Pressure");
    dataFile.close();
  }
  for (int i = 0; i < 10; i++) {
    pressureValue = analogRead(2);
    if(!errorcheck(pressureValue)) {
      dataFile = SD.open("press.txt", FILE_WRITE);
      dataFile.println("ERROR");
      dataFile.close();
    }
    else {
      pressureValue = scale(pressureHigh, pressureLow, pressureValue);
      dataFile = SD.open("press.txt", FILE_WRITE);
      dataFile.println(pressureValue);
      dataFile.close();
    }
  }
  
  dataFile = SD.open("volcur.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Volt/Current");
    dataFile.close();
  }
  for (int i = 0; i < 10; i++) {
    tempValue = analogRead(3);
    if(!errorcheck(voltCurrentValue)) {
      dataFile = SD.open("volcur.txt", FILE_WRITE);
      dataFile.println("ERROR");
      dataFile.close();
    }
    else {
      voltCurrentValue = scale(voltCurrentHigh, voltCurrentLow, voltCurrentValue);
      dataFile = SD.open("volcur.txt", FILE_WRITE);
      dataFile.println(voltCurrentValue);
      dataFile.close();
    }
  }
}

void checkScaleButtonPress(int x, int y) {
  if ((x > 15 && x < 75) && (y > 15 && y < 55)){
    tft.fillScreen(ILI9341_LIGHTGREY);
    setScreen1();  
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
      return 0;   // error
    }
    else 
    {
      return 1;   // no error
    }
}
