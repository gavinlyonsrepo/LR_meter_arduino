/*
  Name : LR_meter_ardunio
  Title : Resistance and Inductance meter. Ardunio ucontroller based.
  Description : Inductance and Resistance meter, Arduino based. 2 tests,
  4 resistor test ranges. One Inductance test, Analog joystick input ,
  outputs to serial monitor and OLED 1602. Resistor range is from 0 ohms to 2 Mohms ,
  Inductor range is 80uH to 30,000uH.
  Author: Gavin Lyons
  URL: https://github.com/gavinlyonsrepo/LR_meter_arduino
*/

//****************** Libraries ********************
#include <Wire.h>
#include <Adafruit_SSD1306.h> // OLED 1.1.2

//***************** GLOBALS ********************

// joystick pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

// resistor test pin numbers
const int analogPin = 2; //analog pin read for resistor test
const int apply_voltage = 7;  //digital pin to resitor unknown apply 5V
// Known Resistors connected to these 4 digital pins
const int Res2K = 11;
const int Res20K = 10;
const int Res200K = 9;
const int Res1M = 8;

// Inductance test pin numbers
const int OutLtestPin = 13; //digital pin input to circuit to "ring" LC circuit
const int PulseInPin = 12; //digital pin to read in pulse , is the comparator/op-amp output.

// OLED data
#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

//*************** SETUP *************
void setup()
{
  Serialinit();
  GPIOinit();  
  Display_init(); 
}

//******************* MAIN LOOP *****************
void loop()
{
  JoyStickRead();
  delay(50);
}

// ********************* Functions *************************

//Function scale_one : Right pressed on joystick carry out 0 to 2k range test.
void scale_one()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, OUTPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, INPUT);
  digitalWrite(Res2K, LOW);
  float R2 = 0;
  float R1 = 2.009; // Set this value to the value of the used resistor in K ohms
  R2 = calc_Res(R1, 1000);
  if (R2 > (R1 * 1000))
  {
    printScaleMsg(5);
  }
  if (R2 < (R1 * 1000))
  {
    print_Res("ohms", R2);
  }
}

//Function scale_two: left pressed on joystick carry out 2K to 20k range test.
void scale_two()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, INPUT);
  pinMode(Res20K, OUTPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, INPUT);
  digitalWrite(Res20K, LOW);
  float R2 = 0;
  float R1 = 21.3; // Set this value to the value of the used resistor in K ohms
  R2 = calc_Res(21.3, 1);
  if (R2 > R1)
  {
    printScaleMsg(5);
  }
  if (R2 < R1)
  {
    print_Res("k ohms", R2);
  }
}

//Function scale_three : Down pressed on joystick carry out 20k to 200k range test.
void scale_three()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, INPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, OUTPUT);
  pinMode(Res1M, INPUT);
  digitalWrite(Res200K, LOW);
  float R2 = 0;
  float R1 = 182; // Set this value to the value of the used resistor in K ohms
  R2 = calc_Res(R1, 1);
  if (R2 > R1)
  {
    printScaleMsg(5);
  }
  if (R2 < R1)
  {
    print_Res("k ohms",  R2);
  }
}
//Function scale_four :UP pressed on joystick carry out 200k to 1M range test.
void scale_four()
{
  digitalWrite(apply_voltage, HIGH);
  pinMode(Res2K, INPUT);
  pinMode(Res20K, INPUT);
  pinMode(Res200K, INPUT);
  pinMode(Res1M, OUTPUT);
  digitalWrite(Res200K, LOW);
  float R2 = 0;
  float R1 = 1.006;// Set first value to the value of the used resistor in K ohms
  R2 = calc_Res(R1, 1);
  if (R2 > 2)
  {
    printScaleMsg(5);
  }
  if (R2 < 2)
  {
    print_Res("M ohms", R2);
  }
}

// Function: calc_Res to calculate unknown resistor value
// Inputs: (2) 1, float R1 known resistor value for given scale
// 2, Integer mulitple factor either 1 or 1000 depending on given scale.
// Outputs: returns 1, float R2 unknown resitor value
float calc_Res(float R1, int multi_factor)
{
  float R2 = 0;
  float buffer = 0;
  int V_measured = 0;
  int Vin = 5;
  float Vout = 0;
  V_measured = analogRead(analogPin); //in 8bits
  buffer = V_measured * Vin;
  Vout = (buffer) / 1024.0; //in volts
  buffer = (Vin / Vout) - 1;
  R2 = R1 * buffer * multi_factor;
  return R2;
}


// Function to print various messages input integer pick is a selector 
// of which message to print.
void printScaleMsg(int pick)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  switch (pick)
  {
        case (1): 
               display.print("R 0k - 2k range");
               Serial.println("0k to 2k rang");
        break;
        case (2):
               display.print("R 2k - 20k range");
               Serial.println("2k to 20k range");
        break;
        case (3): 
                display.print("R 20k - 0.2M range");
                Serial.println("20k to 200k range");
        break;
        case (4): 
               display.print("R  0.2M - 1M range");
               Serial.println("200k to 1M range");
        break;             
        case (5):
               display.print("Increase Scale");
               display.display();
               Serial.println("Increase scale");
               delay(1000);
               OLEDready();              
        break;
       case (6):
               display.print("Inductance test");
               Serial.println("Inductance test");            
        break;
  }
  delay(1000);
}

//Function print_Res : Print calculated resistor value and unit to serial monitor
//Inputs (2) : 1 , String unit unit of resistance Kohms Mohms ,
// 2, float R2 , value of resistance calculated
void print_Res(String unit, float R2)
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

//Function L_test: Calculates Inductance
void L_test()
{
  double pulse, frequency, capacitance, inductance;
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
    Serial.print( pulse );
    Serial.print("\tfrequency Hz:");
    Serial.print( frequency );
    Serial.print("\tinductance uH:");
    Serial.println( inductance );
 
    display.setCursor(0, 15);
    display.print(inductance);
    display.print("uH"); 
    display.display();
    delay(2000);
    OLEDready(); 
}

//Function to init OLED and display OLED  welcome message
// Function to display init screen in setup
void Display_init()
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
  delay(1500); 
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
  display.print("LR Meter");
  display.setCursor(0, 15);
  display.print("Ready");
  display.display();
  delay(1500); 
  Serial.println("LR Meter Ready");
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
    // Joystick input
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

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

void JoyStickRead()
{
    // Read Joystick input on SW_pin
  if (digitalRead(SW_pin) == 0)
  {
    // Switch pressed Inductance test
    printScaleMsg(6);
    L_test();
  }

  //Read Joystick input position
  if (analogRead(X_pin) > 900)
  {
    // up pressed, 200k to 1Meg
    printScaleMsg(4);
    scale_four();
  }
  if (analogRead(X_pin) < 100)
  {
    //down pressed, 20k to 200k
    printScaleMsg(3);
    scale_three();
  }
  if (analogRead(Y_pin) < 100)
  {
    //left pressed, 2k to 20k
    printScaleMsg(2);
    scale_two();
  }
  if (analogRead(Y_pin) > 900)
  {
    //right pressed, 0 to 2k
    printScaleMsg(1);
    scale_one();
  }
  
}
//******************* EOF *****************
