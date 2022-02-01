//Base logic to translate voltage from input to outpt

int channel1 = A0; // declare pin for input channel 1
int channel2 = A1; // declare pin for input channel 2
int channel3 = A2; // declare pin for input channel 3
int channel4 = A3; // declare pin for input channel 4
//*Declare output pins*

int val1 = 0; //channel 1 value variable
int val2 = 0; //channel 1 value variable
int val3 = 0; //channel 1 value variable
int val4 = 0; //channel 1 value variable

bool on1 = 0; //bool to flag if channel should be read
bool on2 = 0; //bool to flag if channel should be read
bool on3 = 0; //bool to flag if channel should be read
bool on4 = 0; //bool to flag if channel should be read

float scale1 = 1.0; //scale factors for different inputs
float scale2 = 1.0; //initialized to 1.0 -> Voltage Reading
float scale3 = 1.0; 
float scale4 = 1.0;

void setup() {
  // put your setup code here, to run once:

  //declare output pins to touch screen and peripherals
}

void loop() {
  // put your main code here, to run repeatedly:
  if(on1)
  {
    val1 = analogRead(channel1); //store voltage reading from channel 1
    /*
     * Check that val1 is a valid input
     * Scale val1 using scale1
     * Send value to peripherals
     */
  }
  if(on2)
  {
    val2 = analogRead(channel2); //store voltage reading from channel 1
    /*
     * Check that val2 is a valid input
     * Scale val2 using scale2
     * Send value to peripherals
     */
    
  }
  if(on3)
  {
    val3 = analogRead(channel3);//store voltage reading from channel 1
    /*
     * Check that val3 is a valid input
     * Scale val3 using scale3
     * Send value to peripherals
     */
    
  }
  if(on4)
  {
    val4 = analogRead(channel4);//store voltage reading from channel 1
    /*
     * Check that val4 is a valid input
     * Scale val4 using scale4
     * Send value to peripherals
     */
    
  }

  // read four channel pins 
}
