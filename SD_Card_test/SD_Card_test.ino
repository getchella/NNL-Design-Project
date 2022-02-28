#include <SPI.h>
#include <SD.h>

#define SD_CS 4

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(SD_CS)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    while (1);
  }
  Serial.println("card initialized.");

  String dataString = "This is a test file";
  float num = 123.45;

  File dataFile = SD.open("testfile.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(dataString);
    dataFile.println(num);
    dataFile.close();
    // print to the serial port too:
    Serial.println(dataString);
    Serial.println(num);
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening testfile.txt");
  }
}

void loop() {
  // make a string for assembling the data to log:

}
