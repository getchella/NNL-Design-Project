//Base logic to translate voltage from input to outpt

const int channel1 = A0; // declare pin for input channel 1
const int channel2 = A1; // declare pin for input channel 2
const int channel3 = A2; // declare pin for input channel 3
const int channel4 = A3; // declare pin for input channel 4
//*Declare output pins*

float val1 = 0; //channel 1 value variable
float val2 = 0; //channel 1 value variable
float val3 = 0; //channel 1 value variable
float val4 = 0; //channel 1 value variable

bool on1 = 1; //bool to flag if channel should be read
bool on2 = 0; //bool to flag if channel should be read
bool on3 = 0; //bool to flag if channel should be read
bool on4 = 0; //bool to flag if channel should be read

float scale1 = 5.0/(1023); //scale factors for different inputs
float scale2 = 5.0/(1023); //initialized to 5/1023 -> Voltage Reading
float scale3 = 5.0/(1023); 
float scale4 = 5.0/(1023);

char unit1 = 'V'; //units for each channel's input
char unit2 = 'V';
char unit3 = 'V'; 
char unit4 = 'V';

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //declare output pins to touch screen and peripherals
}

void loop() {
  // put your main code here, to run repeatedly:
  if(on1)
  {
    val1 = analogRead(channel1); //store voltage reading from channel 1
   if(val1*5/(1023) < 0.875 )
    {
      Serial.print("Error: Low Reading\n");
    }
    else if(val1*5/1023 > 5.125)
    {
      Serial.print("Error: High Reading\n");
    }
    else
    {
     /* Check that val1 is a valid input*/
     val1 = val1*scale1;
     /* Scale val1 using scale1*/
     Serial.print("Channel 1 Value = ");
     Serial.print(val1);
     Serial.print(" ");
     Serial.print(unit1);
     Serial.print("\n");
    }
     delay(1); //delay a millisecond
     /* Send value to peripherals
     */
  }
  if(on2)
  {
   
    val2 = analogRead(channel2); //store voltage reading from channel 2
    if(val2*5/(1023) < 0.875 )
    {
      Serial.print("Error: Low Reading\n");
    }
    else if(val2*5/1023 > 5.125)
    {
      Serial.print("Error: High Reading\n");
    }
    else
    {
     /* Check that val2 is a valid input*/
     val2 = val2*scale2;
     /* Scale val1 using scale1*/
     Serial.print("Channel 2 Value = ");
     Serial.print(val2);
     Serial.print(" ");
     Serial.print(unit2);
     Serial.print("\n");
    }
     delay(1); //delay a millisecond
     /* Send value to peripherals
     */
    
  }
  if(on3)
  {
   val3 = analogRead(channel3); //store voltage reading from channel 1
   if(val3*5/(1023) < 0.875 )
    {
      Serial.print("Error: Low Reading\n");
    }
    else if(val3*5/1023 > 5.125)
    {
      Serial.print("Error: High Reading\n");
    }
    else
    {
     /* Check that val3 is a valid input*/
     val3 = val3*scale3;
     /* Scale val3 using scale3*/
     Serial.print("Channel 3 Value = ");
     Serial.print(val3);
     Serial.print(" ");
     Serial.print(unit3);
     Serial.print("\n");
    }
     delay(1); //delay a millisecond
     /* Send value to peripherals
     */
  }
  if(on4)
  {
    val4 = analogRead(channel4); //store voltage reading from channel 1
   if(val4*5/(1023) < 0.875 )
    {
      Serial.print("Error: Low Reading\n");
    }
    else if(val4*5/1023 > 5.125)
    {
      Serial.print("Error: High Reading\n");
    }
    else
    {
     /* Check that val4 is a valid input*/
     val4 = val4*scale4;
     /* Scale val4 using scale4*/
     Serial.print("Channel 4 Value = ");
     Serial.print(val4);
     Serial.print(" ");
     Serial.print(unit4);
     Serial.print("\n");
    }
     delay(1); //delay a millisecond
     /* Send value to peripherals
     */
    
  }

  // read four channel pins 
}
