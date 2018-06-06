/*
   Name : LR_meter_ardunio
  Title : Resistance and Inductance meter. Ardunio ucontroller based.
  Description : Resistance and Inductance meter,  Ardunio based
  Author: Gavin Lyons
  URL: 
*/

//*************************** GLOBALS ********************

// joystick pin numbers
const int SW_pin = 2; // digital pin connected to switch output
const int X_pin = 0; // analog pin connected to X output
const int Y_pin = 1; // analog pin connected to Y output

// resistor test pin numbers
const int analogPin= 2; //analog pin read for resistor test
const int apply_voltage = 7;  //digital pin to resitor unknown apply 5V
// Known Resistors connected to these 4 digital pins
const int Res2K = 8;
const int Res20K = 9;
const int Res200K = 10;
const int Res1M = 11;

// Inductance test pin numbers
const int OutLtestPin = 13; //digital pin input to circuit to "ring" LC circuit
const int PulseInPin = 12; //digital pin to read in pulse , is the comparator/op-amp output.

//*************************** SETUP ************************
void setup() 
{
  // Joystick input
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);

  // Set resistor measurement pins 
  pinMode(analogPin,INPUT);
  pinMode(apply_voltage,OUTPUT);
  // Set known resistor pins as inputs 
  pinMode(Res2K,INPUT);
  pinMode(Res20K,INPUT);
  pinMode(Res200K,INPUT);
  pinMode(Res1M,INPUT);
  
  Serial.begin(9600);
  delay(100);
  Serial.println("-------------LR Meter Comms UP------------");
}

//******************* MAIN LOOP *****************
void loop() 
{      
  // Read Joystick input on SW_pin
  if (digitalRead(SW_pin) == 1)
  {
    // Switch pressed Inductance test
    Serial.println("Inductance test:");
    Serial.println(digitalRead(SW_pin));
    L_test();
  }
  if (analogRead(X_pin) > 900)
  {
    // up pressed, 200k to 1Meg 
    Serial.println("200k to 1M range");
    scale_four();
  }  
  if (analogRead(X_pin) < 100)
  {
    //down pressed, 20k to 200k 
    Serial.println("20k to 200k range");
    scale_three();
  }
  if (analogRead(Y_pin) < 100)
  {
    //left pressed, 2k to 20k 
    Serial.println("2k to 20k range");
    scale_two();
  }
  if (analogRead(Y_pin) >900)
  {
    //right pressed, 0 to 2k
    Serial.println("0k to 2k range");
    scale_one();
  } 
  delay(500);
}


// ********************* Functions *************************

//Fucntion scale_one : Right pressed on joystick carry out 0 to 2k range test.
void scale_one()
{
  digitalWrite(apply_voltage,HIGH);
  pinMode(Res2K,OUTPUT);
  pinMode(Res20K,INPUT);
  pinMode(Res200K,INPUT);
  pinMode(Res1M,INPUT);
  digitalWrite(Res2K,LOW); 
  float R2 = 0;
  float R1 = 2.009; // Set this value to the value of the used resistor in K ohms
  R2 = calc_Res(R1, 1000);  
      if (R2 > (R1*1000))
      {
        Serial.println("Increase scale");       
      }
      if (R2 < (R1*1000))
      {
        print_Res("ohms", R2);
      }
}

//Function scale_two: left pressed on joystick carry out 2K to 20k range test.
void scale_two()
{
  digitalWrite(apply_voltage,HIGH);
  pinMode(Res2K,INPUT);
  pinMode(Res20K,OUTPUT);
  pinMode(Res200K,INPUT);
  pinMode(Res1M,INPUT);
  digitalWrite(Res20K,LOW);
  float R2 = 0;
  float R1 = 21.3; // Set this value to the value of the used resistor in K ohms
  R2 = calc_Res(21.3, 1); 
      if (R2 > R1)
      {
        Serial.println("Increase scale");       
      } 
      if (R2 < R1)
      {
       print_Res("k ohms", R2);
      }
}

//Function scale_three : Down pressed on joystick carry out 20k to 200k range test.
void scale_three()
{
  digitalWrite(apply_voltage,HIGH);
  pinMode(Res2K,INPUT);
  pinMode(Res20K,INPUT);
  pinMode(Res200K,OUTPUT);
  pinMode(Res1M,INPUT);
  digitalWrite(Res200K,LOW);
  float R2 = 0;
  float R1 = 182; // Set this value to the value of the used resistor in K ohms
  R2 = calc_Res(R1, 1); 
      if (R2 > R1)
      {
        Serial.println("Increase scale");           
      } 
      if (R2 < R1)
      {
       print_Res("k ohms",  R2);
      }
}
//Function scale_four :UP pressed on joystick carry out 200k to 1M range test.
void scale_four()
{
  digitalWrite(apply_voltage,HIGH);
  pinMode(Res2K,INPUT);
  pinMode(Res20K,INPUT);
  pinMode(Res200K,INPUT);
  pinMode(Res1M,OUTPUT);
  digitalWrite(Res200K,LOW);
  float R2 = 0;
  float R1 = 1.006;// Set first value to the value of the used resistor in K ohms
  R2 = calc_Res(R1, 1);  
      if (R2 > 2)
      {
        Serial.print("Increase scale");
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
      float R2= 0;
      float buffer= 0;
      int V_measured= 0;
      int Vin= 5;
      float Vout= 0;
      V_measured= analogRead(analogPin); //in 8bits
      buffer= V_measured * Vin;
      Vout= (buffer)/1024.0;  //in volts
      buffer= (Vin/Vout) -1;
      R2= R1 * buffer*multi_factor; 
      return R2;
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
    delay(1000);
}

//Function L_test: Calculates Inductance
void L_test()
{ 

  double pulse, frequency, capacitance, inductance;
  digitalWrite(OutLtestPin, HIGH);
  delay(5);//give some time to charge inductor.
  digitalWrite(OutLtestPin,LOW);
  delayMicroseconds(100); //make sure resination is measured
  pulse = pulseIn(PulseInPin,HIGH,5000);//returns 0 if timeout
  if(pulse > 0.1){ //if a timeout did not occur and it took a reading:
      
  //error insert your used capacitance value here. Currently using 2uF. Delete this line after that
  capacitance = 2.E-6; // - insert value here
   
  frequency = 1.E6/(2*pulse);
  inductance = 1./(capacitance*frequency*frequency*4.*3.14159*3.14159);
  inductance *= 1E6; //note that this is the same as saying inductance = inductance*1E6

  //Serial print
  Serial.print("High for uS:");
  Serial.print( pulse );
  Serial.print("\tfrequency Hz:");
  Serial.print( frequency );
  Serial.print("\tinductance uH:");
  Serial.println( inductance );
  delay(10);
  }
  delay(1500);
}
//******************* EOF *****************
