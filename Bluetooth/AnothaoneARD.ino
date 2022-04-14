/*
 * Bluetooth Test - Arduino Side
 * Arduino to HM10 module to Google Chrome
 * https://www.amazon.com/gp/product/B06WGZB2N4/ref=ppx_yo_dt_b_asin_title_o01_s00?ie=UTF8&psc=1
 * 
 * 
 * kevin darrah
 * 
 * Twitter: https://twitter.com/KDcircuits
 * For inquiries or design services:
 * https://www.kdcircuits.com
 * 
 * License?  Do whatever you want with this code - it's just a sample
 */

#include <SoftwareSerial.h>

//Pins  ************************
//UART TO HM10 Module
const uint8_t bluRX_ardTXpin = 3;
const uint8_t bluTX_ardRXpin = 2;


//Declaring any other pins for variables. These can be deleted. Left in so code works for now and is raw
//RGB LED
const uint8_t redPin = 9;
const uint8_t greenPin = 10;
const uint8_t bluePin = 11;
// Push Button
const uint8_t pushButtonPin = 6;
String temp;
String humi;

//It is now just called bluetooth. Can use bluetooth.print to talk to the module
SoftwareSerial  bluetooth(bluTX_ardRXpin, bluRX_ardTXpin);

//function prototypes
void checkBluetooth();

void setup() {
  bluetooth.begin(9600);
  Serial.begin(9600);
  //bluetooth.print("AT+NAMENNLArduino");//found this here: ftp://imall.iteadstudio.com/Modules/IM130614001_Serial_Port_BLE_Module_Master_Slave_HM-10/DS_IM130614001_Serial_Port_BLE_Module_Master_Slave_HM-10.pdf
  bluetooth.print("NNL Connected");
  pinMode(pushButtonPin, INPUT_PULLUP);
}

void loop() {

Serial.println("Enter Temp Data: ");
while (Serial.available() == 0){}
temp = Serial.readString();
Serial.println("Enter Humidity Data: ");
while (Serial.available() == 0){}
humi = Serial.readString();

bluetooth.print("Temp: " + String(temp));
delay(200);
bluetooth.print("Humi: " + String(humi));
//bluetooth.print("Humi: " + String(humi));// bluetooth.print(temp);


delay(1000);

//checkBluetooth();//if something is coming at us


//Bottom can prob be deleted, this is arduino to computer info
//  if (!digitalRead(pushButtonPin)) {//send out
//    bluetooth.print("Push Button");
//    while (!digitalRead(pushButtonPin)) {}
//    delay(20);
//    bluetooth.print(" ");
//  }


}

//void checkBluetooth() {
//  char charBuffer[20];//most we would ever see
//  if (bluetooth.available() > 0) {
//    int numberOfBytesReceived = bluetooth.readBytesUntil('\n', charBuffer, 19);
//    charBuffer[numberOfBytesReceived] = 0;
//    //    Serial.print("Received: ");
//    //    Serial.println(charBuffer);
//
//    if (strstr(charBuffer, "LED OFF") == &charBuffer[0]) {
//      Serial.println("Turning LED OFF");
//      analogWrite(redPin, 255);
//      analogWrite(greenPin, 255);
//      analogWrite(bluePin, 255);
//
//    } else if (strstr(charBuffer, "LED Color#") == &charBuffer[0]) {
//      char hexColorString[10] = "";
//      strcpy(hexColorString, &charBuffer[strlen("LED Color#")]);
//      unsigned long colorInt = 0;
//      //http://www.asciitable.com
//      for (int i = 0; i < 6; i++) {
//        if (hexColorString[i] < 97) { //number parts
//          colorInt = colorInt + (((long)hexColorString[i] - 48) << ((5 - i) * 4));
//        } else { //letter in hex parts
//          colorInt = colorInt + (((long)hexColorString[i] - 97  + 10) << ((5 - i) * 4));
//        }
//      }
//
//      //Serial.println(colorInt);
//      byte redColor = colorInt >> (16);
//
//      analogWrite(redPin, ~redColor);
//
//      byte greenColor = colorInt >> (8);
//      analogWrite(greenPin, ~greenColor);
//      //Serial.println(greenColor);
//      byte blueColor = colorInt;
//      analogWrite(bluePin, ~blueColor);
//
//      Serial.print(redColor);
//      Serial.print(",");
//      Serial.print(greenColor);
//      Serial.print(",");
//      Serial.println(blueColor);
//    }
//  }
//


//}
