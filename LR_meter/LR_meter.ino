
/*
  Name : LR_meter_ardunio
  Title : Resistance and Inductance meter. Ardunio ucontroller based.
  Description : Inductance and Resistance meter, Arduino based. 3 tests,
  4 resistor test ranges. One Inductance test, push button input ,
  outputs to serial monitor and OLED 1602. Resistor range is from 0 ohms to 2 Mohms ,
  Inductor range is 80uH to 30,000uH. Also includes Analog ADc display
  Author: Gavin Lyons
  URL: https://github.com/gavinlyonsrepo/LR_meter_arduino
*/

//****************** Libraries ********************
#include <Adafruit_SSD1306.h> // OLED 1.1.2

//***************** GLOBALS ********************

// Push Button pin
#define buttonTest 4
#define buttonMode 5

// Variables to debounce switches
int buttonTestState;             // the current reading from the input pin
int lastButtonTestState = HIGH;   // the previous reading from the input pin
// the following variable are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTestTime = 0;  // the last time the output pin was toggled

int buttonModeState;             // the current reading from the input pin
int lastButtonModeState = HIGH;   // the previous reading from the input pin
// the following variable are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceModeTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; 

// Var to hold menu mode
uint8_t mode = 0;

//ADC display.
const int analogPinADC = 0; //analog pin read for ADC display

// resistor test pin numbers
const int analogPin = 2; //analog pin read for resistor test
const int apply_voltage = 7;  //digital pin to resitor unknown apply 5V
// Known Resistors connected to these 4 digital pins
const int Res2K = 11;
const int Res20K = 10;
const int Res200K = 9;
const int Res1M = 8;

// Inductance test pin numbers
const int OutLtestPin = 6; //digital pin input to circuit to "ring" LC circuit
const int PulseInPin = 12; //digital pin to read in pulse

// OLED data
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//**************** FUNCTION PROTOTYPES ***********
void DisplayInit();
void Serialinit();
void GPIOinit();
void OLEDready();
void DisplayHelpMsg();
void ResScaleOne();
void ResScaleTwo();
void ResScaleThree();
void ResScaleFour();
void Ltest();
void AnalogDisplay();
float calcResult(float R1, int multi_factor);
void PrintResult(String unit, float R2);
void printMenuMsg();
void TestRun();
void ReadPushButtonMode();
void ReadPushButtonTest();

//*************** SETUP *************
void setup()
{
  Serialinit();
  GPIOinit();
  DisplayInit();
}

//******************* MAIN LOOP *****************
void loop()
{
  ReadPushButtonMode();
  ReadPushButtonTest();
  delay(50);
}

// ********************* Functions *************************

// Function to display help message related to PCB socket layout
void DisplayHelpMsg()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.print("Connector pinout");
  display.setCursor(0, 12);
  display.print("Row1 : R1  RRRR");
  display.setCursor(0, 22);
  display.print("Row2 : A G V LL");
  display.display();
  delay(5000);
  OLEDready();
}

//Function to init OLED and display OLED  welcome message
void DisplayInit()
{
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("LR Meter");
  display.setCursor(0, 15);
  display.print("G. Lyons");
  display.display();
  delay(1000);
  OLEDready();
}

//Function to display Ready message on OLED and serial monitor
void OLEDready()
{
  display.clearDisplay();
  display.display();
  display.setCursor(0, 0);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.print("      MODE");
  display.setCursor(0, 15);
  display.print("Ready TEST");
  display.display();
  Serial.println("LR Meter Ready");
  digitalWrite(LED_BUILTIN, HIGH);
  mode = 0;
  delay(50);
}

//Function to setup serial called from setup
void Serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("-------------LR Meter Comms UP------------");
}

//Function to init GPIO pins on ATmega328p setup called from setup
void GPIOinit()
{
   // Status LED
   pinMode(LED_BUILTIN, OUTPUT);
   digitalWrite(LED_BUILTIN, HIGH);  
   delay(100);                    
   digitalWrite(LED_BUILTIN, LOW);
  
  // Setup pins for button enable internal pull-up resistors
  digitalWrite(buttonMode, HIGH);
  digitalWrite(buttonTest, HIGH);

  // Set resistor measurement pins
  pinMode(analogPin, INPUT);
  pinMode(apply_voltage, OUTPUT);
  // Set known resistor pins as inputs
  pinMode(Res2K, INPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, INPUT);

  //Inductance test pins
  pinMode(PulseInPin, INPUT);
  pinMode(OutLtestPin, OUTPUT);
}

//Function ResScaleOne :resistor 0 to 2k range test.
void ResScaleOne()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, OUTPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, INPUT);
  digitalWrite(Res2K, LOW);
  float R2 = 0;
  float R1 = 2.005; // Set this value to the value of the used resistor in K ohms
  R2 = calcResult(R1, 1000);
  if (R2 > (R1 * 1000))
  {
    Serial.println("Increasing Scale");
    mode = 2; //increase scale
    printMenuMsg();
    //ResetResRange();
    ResScaleTwo();
  }
  if (R2 < (R1 * 1000))
  {
    PrintResult("ohms", R2);
  }
}

//Function ResScaleTwo: resistor 2K to 20k range test.
void ResScaleTwo()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, INPUT);
  pinMode(Res20K, OUTPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, INPUT);
  digitalWrite(Res20K, LOW);
  float R2 = 0;
  float R1 = 18.3; // Set this value to the value of the used resistor in K ohms
  R2 = calcResult(20.03, 1);
  if (R2 > R1)
  {
    Serial.println("Increasing Scale");
    mode = 3; //increase scale
    printMenuMsg();
    //ResetResRange();
    ResScaleThree();
  }
  if (R2 < R1)
  {
    PrintResult("k ohms", R2);
  }
}

//Function ResScaleThree : resistor test 20k to 200k range test.
void ResScaleThree()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, INPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, OUTPUT);
  pinMode(Res1M, INPUT);
  digitalWrite(Res200K, LOW);
  float R2 = 0;
  float R1 = 218; // Set this value to the value of the used resistor in K ohms
  R2 = calcResult(R1, 1);
  if (R2 > R1)
  {
    Serial.println("Increasing Scale");
    mode = 4; //increase scale
    printMenuMsg();
    //ResetResRange();
    ResScaleFour();
  }
  if (R2 < R1)
  {
    PrintResult("k ohms",  R2);
  }
}
//Function ResScaleFour : resistence test 200k to 1M range test.
void ResScaleFour()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, INPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, OUTPUT);
  digitalWrite(Res1M, LOW);
  float R2 = 0;
  float R1 = 1006;// Set first value to the value of the used resistor in Kohms
  R2 = calcResult(R1, 1);
  if (R2 > 2000)
  {
    mode = 8; //Beyond Scale Too high
    printMenuMsg();
  }
  if (R2 < 2000)
  {
    if (R2 <= 10)
    {
       Serial.println("Decreasing Scale");
       mode = 1; //decrease scale
       printMenuMsg();
       //ResetResRange();
       ResScaleOne(); 
       return;  
    }
    PrintResult("M ohms", (R2/1000));
  }
}

// Function: calcResult to calculate unknown resistor value
// Inputs: (2) 1, float R1 known resistor value for given scale
// 2, Integer multiple factor either 1 or 1000 depending on given scale.
// Outputs: returns 1, float R2 unknown resitor value
float calcResult(float R1, int multi_factor)
{
  float R2 = 0;
  float tmpbuffer = 0;
  int V_measured = 0;
  uint8_t Vin = 5;
  float Vout = 0;
  const uint8_t numReadings = 11; // number of analog samples
  int readings[numReadings];      // the readings from the analog input

  // Get 11(numreadings) values from ADC 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
  {
    readings[thisReading] = analogRead(analogPin); // ADC
    if (thisReading != 0) // ignore first reading as it is bad during auto-range.
    {
      V_measured  = V_measured + readings[thisReading]; //running total
    }
  }
  V_measured = (V_measured /(numReadings-1)); // average
  
  Vout = (V_measured * Vin) / 1024.0; //Convert ADC to voltage
  tmpbuffer = (Vin / Vout) - 1; //voltage divider (VIN/VOUT -1)
  R2 = R1 * tmpbuffer * multi_factor;  // R2 = R1(Vin/Vout -1) 
  return R2;
}

// Function to print various messages input based on Menu push button press
void printMenuMsg()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  switch (mode)
  {
    case (0):
      __asm__("nop\n\t");
      break;
    case (1):
      display.print("R 0k - 2k range");
      display.display();
      Serial.println("0k to 2k range");
      break;
    case (2):
      display.print("R 2k - 20k range");
      display.display();
      Serial.println("2k to 20k range");
      break;
    case (3):
      display.print("R 20k - 200k range");
      display.display();
      Serial.println("20k to 200k range");
      break;
    case (4):
      display.print("R  0.2M - 1M range");
      display.display();
      Serial.println("200k to 1M range");
      break;
    case (5):
      display.print("Inductance test");
      display.display();
      Serial.println("Inductance test");
      break;
    case (6):
      display.print("Analog TEST");
      display.display();
      Serial.println("Analog display test");
      break;
    case(7):
         OLEDready();
    break;
    case (8):
      display.print("Out of Scale");
      display.display();
      Serial.println("Out of scale");
      delay(1500);
      OLEDready();
      break;
  }
}

//Function to run tests when test button pressed
void TestRun()
{
    digitalWrite(LED_BUILTIN, LOW);
    switch (mode)
    {  
    case (0):
        DisplayHelpMsg();
        __asm__("nop\n\t");
      break;
    case (1):
        ResScaleOne();
      break;
    case (2):
        ResScaleTwo();
      break;
    case (3):
        ResScaleThree();
      break;
    case (4):
        ResScaleFour();
      break;
    case (5):
        Ltest();
      break;
    case (6):
       AnalogDisplay();
      break;
    }
  digitalWrite(LED_BUILTIN, HIGH);
}

//Function PrintResult : Print calculated resistor value and unit to serial monitor
//Inputs (2) : 1 , String unit unit of resistance Kohms Mohms ,
// 2, float R2 , value of resistance calculated
void PrintResult(String unit, float R2)
{
  Serial.println("Resistance: ");
  Serial.print(R2);
  Serial.print(" ");
  Serial.println(unit);

  display.setCursor(0, 15);
  display.print(R2);
  display.print(unit);
  display.display();
  delay(2000);

  OLEDready();
}

//Function to display ADC and voltage value at analog input
void AnalogDisplay()
{
  float ADCValue = 0;
  float VoltValue = 0;
  while(1)
  {
  float ADCValue = analogRead(analogPinADC);
  VoltValue = (ADCValue*0.0048828125);  // ADC*(5/1024)
  Serial.print("ADC: ");
  Serial.println(ADCValue);
  Serial.print("Volt : ");
  Serial.println(VoltValue);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("ADC : ");
  display.print( ADCValue);
  display.setCursor(0, 15);
  display.print("Volt : ");
  display.print(VoltValue);
  
  display.display();
  delay(2000);
  }
}
//Function Ltest: Calculates Inductance
void Ltest()
{
  double pulse, frequency, capacitance, inductance = 0;
  digitalWrite(OutLtestPin, HIGH);
  delay(5);//give some time to charge inductor.
  digitalWrite(OutLtestPin, LOW);
  delayMicroseconds(100); //make sure resonation is measured
  pulse = pulseIn(PulseInPin, HIGH, 5000); //returns 0 if timeout
  if (pulse > 0.1) { //pulse returns 0 if no complete pulse was received within the timeout
     
    capacitance = 2.E-6; // - insert Cap value here

    frequency = 1.E6 / (2 * pulse);
    inductance = 1. / (capacitance * frequency * frequency * 4.*3.14159 * 3.14159);
    inductance *= 1E6;
  }

  //Serial print
  Serial.print("High for uS:");
  Serial.println( pulse );
  Serial.print("\tfrequency Hz:");
  Serial.println( frequency );
  Serial.print("\tinductance uH:");
  Serial.println( inductance );

  display.setCursor(0, 12);
  display.print(inductance);
  display.print("uH ");
  display.setCursor(0, 22);
  display.print(frequency);
  display.print("Hz ");
  display.print(pulse);
  display.print("uS ");
  display.display();
  delay(2000);
  OLEDready();
}

// Function to handle debounce of mode menu button
// If debounced and succesful read alter mode var and menu display
void ReadPushButtonMode()
{
   // read and debounce push button.
  int reading = digitalRead(buttonMode);
  // If the switch changed?
  if (reading != lastButtonModeState) {
    // reset the debouncing timer
    lastDebounceModeTime = millis();
  }
  if ((millis() - lastDebounceModeTime) > debounceDelay) {
    // if the button state has changed:
    if (reading != buttonModeState) {
      buttonModeState = reading;
      if (buttonModeState == LOW) {
        mode++;
        printMenuMsg();
      }
    }
  }

  // save the reading.
  lastButtonModeState = reading;
}

// Function to handle debounce of start test button
// If debounced and succesful read start the test.
void ReadPushButtonTest()
{
   // read and debounce push button.
  int reading = digitalRead(buttonTest);
  // If the switch changed?
  if (reading != lastButtonTestState) {
    // reset the debouncing timer
    lastDebounceTestTime = millis();
  }
  if ((millis() - lastDebounceTestTime) > debounceDelay) {
    // if the button state has changed:
    if (reading != buttonTestState) {
      buttonTestState = reading;
      // start test if the new button state is low
      if (buttonTestState == LOW) {
        TestRun();
      }
    }
  }

  // save the reading.
  lastButtonTestState = reading;
}

//******************* EOF *****************
