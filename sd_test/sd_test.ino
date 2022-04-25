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
#define btTX      PD6
#define btRX      PD7

const uint16_t t1_load = 0;
const uint16_t t1_comp = 62500;

// utilizes SPI on Arduino to output data to touchscreen
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
// utilizes I2C on Arduino to get touch screen points
Adafruit_FT6206 ts = Adafruit_FT6206();

//SoftwareSerial bt(btRX, btTX);

int lowTmp, highTmp;
char tempUnit = 'C';
char humidityUnit = '%';
String pressureUnit = String("PSI");
String voltCurrentUnit = String('V');
uint8_t screen;
float tempValue=0, humidityValue, pressureValue, voltCurrentValue;
float tempHigh = 80;
float tempLow = -40;        // Celsius by default
float humidityHigh = 100;
float humidityLow = 0;    // %
float pressureHigh = 100;
float pressureLow = 0;    // PSI by default
float voltCurrentHigh = 5;
float voltCurrentLow = 1;

File dataFile;
char sdArr[50];
bool sdFlag = true;
bool btFlag = true;

void setup() {
  //bt.begin(9600);
  //SPI.setClockDivider(SPI_CLOCK_DIV64);
  tft.begin(4000000);
  ts.begin(40);   // pass in sensitivity coefficient

  SD.begin(SD_CS);
  SD_Init();
  
  tft.setRotation(3);   // rotates the screen to horizontal
  tft.fillScreen(ILI9341_LIGHTGREY);   // sets background to gray

  setScreen1();

  pinMode(PD7, OUTPUT);
/*
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
  */
}

ISR(TIMER1_COMPA_vect) {
  TCNT1 = t1_load;

  //String tempValueStr = valueToString(tempHigh, tempLow, tempValue);
  //String humidityValueStr = valueToString(humidityHigh, humidityLow, humidityValue);
  //String pressureValueStr = valueToString(pressureHigh, pressureLow, pressureValue);
  //String voltCurrentValueStr = valueToString(voltCurrentHigh, voltCurrentLow, voltCurrentValue);
  
  //if (true) {
    //tempValue = analogRead(0);
    //logValue(tempHigh, tempLow, tempValue);
    
    //String dataString = String(tempValue) + ',' + String(humidityValue) + ',' + String(pressureValue) + ',' + String(voltCurrentValue);
    //while (!(dataFile = SD.open("file.csv", FILE_WRITE)));
    //if (dataFile) {
      //String data = tempValueStr + ',' + humidityValueStr + ',' + pressureValueStr + ',' + voltCurrentValueStr;
      //dataFile.println("Hello World");
      //dataFile.close();
    //}
    //else {
    //  tft.setCursor(0,0);
    //  tft.print("fail");
    //}
  //}
/* 
  if (screen == SCREEN1) {
    tempValue = analogRead(0);
    humidityValue = analogRead(1);
    pressureValue = analogRead(2);
    voltCurrentValue = analogRead(3);
    tft.fillRect(40, 45, 90, 22, ILI9341_WHITE);
    tft.fillRect(185, 45, 90, 22, ILI9341_WHITE);
    tft.fillRect(40, 130, 90, 22, ILI9341_WHITE);
    tft.fillRect(185, 130, 90, 22, ILI9341_WHITE);
        
    Display_Value(tempHigh, tempLow, tempValue, 2, 40, 45);                 // display temperature value
    Display_Value(humidityHigh, humidityLow, humidityValue, 2, 185, 45);    // display humidity value
    Display_Value(pressureHigh, pressureLow, pressureValue, 2, 40, 130);    // display pressure value
    Display_Value(voltCurrentHigh, voltCurrentLow, voltCurrentValue, 2, 185, 130);   // display volt/current value
  }
*/  
}

void loop() {
  dataFile = SD.open("file.csv", FILE_WRITE);
  if (dataFile) {
    dataFile.println("Hello World");
    dataFile.close();
    digitalWrite(PD7, LOW);
  }
  else {
    digitalWrite(PD7, HIGH);
  }
  delay(1000);
  // make a string for assembling the data to log:
  /*
  String dataString = "";

  // read three sensors and append to the string:
  for (int analogPin = 0; analogPin < 3; analogPin++) {
    int sensor = analogRead(analogPin);
    dataString += String(sensor);
    if (analogPin < 2) {
      dataString += ",";
    }
  }
  */
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  /*
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
    // print to the serial port too:
  }
  */
  // if the file isn't open, pop up an error:
}

void SD_Init() {
  dataFile = SD.open("file.csv", FILE_WRITE);
  dataFile.println("Temperature,Humidity,Pressure,Volt/Current");
  dataFile.close();
}

void setScreen1() {
  //drawRightArrowBox();
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

String valueToString(float high, float low, float value) {
  float val;
  if (errorcheck(value)) {
    val = scale(high, low, value);
   // return String(val);
   return "Value";
  }
  else {
    return "ERROR";
  }
}

void logValue(float high, float low, float value) {
  float val;
  if (errorcheck(value)) {
    val = scale(high, low, value);
    dataFile = SD.open("file.csv", FILE_WRITE);
    dataFile.println(val);
    dataFile.close();
  }
  else {
    dataFile = SD.open("file.csv", FILE_WRITE);
    dataFile.println("ERROR");
    dataFile.close();
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
      return 0;
    }
    else 
    {
      return 1;
    }
}
