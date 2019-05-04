Overview
--------------------
* Name : LR_meter_arduino
* Title : Inductance and Resistance meter, Arduino based.
* Description : Inductance and Resistance meter, Arduino based. 3 tests, 4 resistor test ranges. One Inductance test, ADC measurement display , Push button  input , outputs data to serial monitor and OLED. Resistor range is from 0ohms to 2MOhms , Inductor range is 80uH to 30,000uH.
* Author: Gavin Lyons


Software Used
------------------
Arduino 1.8.5

Eagle 9.0.1


Libraries
------------------------
* <Wire.h> //I2C for OLED comms 1.0.0
* <Adafruit_SSD1306.h> // OLED 1.1.2

Parts List
------------------------------
The schematic is available in Eagle in documents

Parts List:

>
> I2C 0.91" inch 128x32 OLED Display Module Driver IC: SSD1306.
>
> Arduino NANO or UNO
>
> Resistors: 2kohms, 20kohms, 200kohms, 1Mohms, 150ohm, 330ohm, 4.7Kohms X 2 .
>
> Capacitors: 2 uF non-polarized  
>
> Two push buttons.
>
> Diode:  1N4001 or IN4148
>
> LM393 comparator IC [datasheet](https://www.onsemi.com/pub/Collateral/LM393-D.PDF) or LM339
>
> ZIF test socket , Output terminal or socket connections
>


Push Button control
-------------------------------

Push Buttons
1. Mode Button to change mode
2. Test button to start test for selected mode.

Tests Modes:

1. Inductance test.
2. 0 to 2k range resistor test.
3. 2K to 20k range resistor test.
4. 20k to 200k range resistor test.
5. 200k to 1M range resistor test.
6. Display ADC in digital value and Voltage.

Features
-----------------------------------------------

![ScreenShot schematic](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/lr_meter2.png)


**Resistance test**

The resistors used in circuit should be measured on good quality meter 
and there exact values added to code for maximum accuracy.

0-2kohms : 2kohms resistor

2k-20k : 20k resistor

20k-200k : 200k resistor

200k - 1Mohm : 1 Mohm resistor

So we will use a basic voltage divider to calculate the resistance. 

![ScreenShot seq 1](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq3.png)

A voltage divider is made of two Resistances (R1 and R2) in series. The output voltage in the middle point is [R2/(R1+R2)]Vin. Using this formula and knowing the value of R2 and measuring the Vout calculate the resistance of R1. 
Vin will be 5 volts from the Arduino, We can measure Vout with one of the Arduino's analog inputs.
We need to use different ranges of R2 to measure with better  precision. 

**Inductance test**

This tests test range is from 80uH to 30,000uH. Ideally the capacitors in circuit must be in specification and measured on a good quality meter. The unknown test inductor is placed in  parallel with a capacitor is creating an LC circuit. 

As soon as the voltage on the LC circuit becomes positive, the LM393 will be floating, which can be pulled high with a pull up resistor. When the voltage on the LC circuit becomes negative, the LM393 will pull its output to ground. 

A 5V pulse signal from Arduino is applied to the LC circuit. We charge the circuit for some time. Then we change the voltage from 5 volts directly to 0. That pulse will make the circuit resonate creating a  sinusoidal signal oscillating at the resonant frequency. The freq is measured and later using the formulas obtain the inductance value. We will use the Arduino to measure the frequency and calculate the value. The resonant frequency is related with the next equation: 

![ScreenShot seq 1](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq1.png)

So we could obtain the L value because we know the F frequency that we've just measured and we also know the values of the capacitor because it's a component that we've selected. All we need is to obtain L from this equation.


![ScreenShot seq 2](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq2.png)

The comparator will turn the sine wave into a square wave with a duty of 50%(see fig 9 in datasheet),  The microcontroller measures the signal , This measurement can then be doubled to get the period and the inverse of the period is the frequency. Since the circuit is resonating, this frequency is the resonating frequency.

We can now solve the equation for inductance.




**Display ADC in digital value and Voltage.**

This displays the voltage and ADC digital value measured on A0, This can be used for testing potentiometers,
sensors or measuring voltage levels.

Copyright
-------------------------------

Copyright (C) 2018 G Lyons. This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, see license.md for more details

