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
bool on2 = 1; //bool to flag if channel should be read
bool on3 = 1; //bool to flag if channel should be read
bool on4 = 0; //bool to flag if channel should be read



float High1 = 100; //high reading channel 1
float Low1 = 0; //low reading channel 1
float High2 = 5; //high reading channel 2
float Low2 = 1; //low reading channel 2
float High3 = 20; //high reading channel 3
float Low3 = 4; //low reading channel 3
float High4 = 5; //high reading channel 4
float Low4 = 1; //low reading channel 5

float b1 = -(High1-5*Low1)/4; //intercept for each channel
float b2 = -(High2-5*Low2)/4;
float b3 = -(High3-5*Low3)/4;
float b4 = -(High4-5*Low4)/4;

float m1 = (High1-b1)/1023; //slopes for each channel 
float m2 = (High2-b2)/1023;
float m3 = (High3-b4)/1023;
float m4 = (High4-b4)/1023;

char unit1 = '%'; //units for each channel's input
char unit2 = 'V';
String unit3 = "mA"; 
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
      Serial.print("Error\n");
    }
    else
    {
     /* Check that val1 is a valid input*/
     val1 = (val1*m1) + b1;
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
      Serial.print("Error\n");
    }
    else
    {
     /* Check that val2 is a valid input*/
     val2 = m2*val2 + b2;
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
      Serial.print("Error");
    }
    else
    {
     /* Check that val3 is a valid input*/
     val3 = m3*val3 + b3;
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
      Serial.print("Error\n");
    }
    else
    {
     /* Check that val4 is a valid input*/
     val4 = m4*val4 + b4;
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
