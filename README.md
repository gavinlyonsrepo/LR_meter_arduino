[![Website](https://img.shields.io/badge/Website-Link-blue.svg)](https://gavinlyonsrepo.github.io/)  [![Rss](https://img.shields.io/badge/Subscribe-RSS-yellow.svg)](https://gavinlyonsrepo.github.io//feed.xml)  [![Donate](https://img.shields.io/badge/Donate-PayPal-green.svg)](https://www.paypal.com/paypalme/whitelight976)

Table of contents
---------------------------

  * [Overview](#overview)
  * [Software used](#software-used)
  * [Libraries](#libraries)
  * [Parts list](#parts-list)
  * [Push button control](#push-button-control)
  * [Features](#features)

Overview
--------------------
* Name : LR_meter_arduino
* Title : Inductance and Resistance meter, Arduino based.
* Description : 
	* Inductance and Resistance meter.
	* Arduino based. 
	* 4 resistor test ranges,  Resistor range is from 0 ohms to 2 Mohms. Auto-range.
	* Inductance test, range is 80uH to 30,000uH.
	* ADC measurement display test.
    * Push button input.
    * Outputs data to serial monitor and OLED.

* Author: Gavin Lyons


Software used
------------------
Arduino 1.8.5 & Eagle 9.0.1

Libraries
------------------------
* <Adafruit_SSD1306.h> // OLED 1.1.2

Parts List
------------------------------
The schematic and Parts list are available in documentation folder.

Push button control
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
and there exact values added to code for maximum accuracy. As of Version 2.2 This Resistance test can now auto-range in both directions. 

0-2kohms : 2kohms resistor

2k-20k : 20k resistor

20k-200k : 200k resistor

200k - 1Mohm : 1 Mohm resistor

So we will use a basic voltage divider to calculate the resistance. 

![ScreenShot seq 1](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq3.png)

A voltage divider is made of two Resistances (R1 and R2) in series. The output voltage in the middle point is [R2/(R1+R2)]Vin. Using this formula and knowing the value of R2 and measuring the Vout calculate the resistance of R1. 
Vin will be 5 volts from the Arduino, We can measure Vout with one of the Arduino's analog inputs.
We need to use different ranges of R2 to measure with better precision. 

**Inductance test**

This tests range is from 80uH to 30,000uH. Ideally the capacitors in circuit must be in specification and measured on a good quality meter. The unknown test inductor is placed in parallel with a capacitor thus creating an LC circuit. 

As soon as the voltage on the LC circuit becomes positive, the LM393 will be floating, which can be pulled high with a pull up resistor. When the voltage on the LC circuit becomes negative, the LM393 will pull its output to ground. 

A 5V pulse signal from Arduino is applied to the LC circuit. We charge the circuit for some time. Then we change the voltage from 5 volts directly to 0. That pulse will make the circuit resonate creating a  sinusoidal signal oscillating at the resonant frequency. The frequency is measured and later using the formulas we can obtain the inductance value. We will use the Arduino to measure the frequency and calculate the value. The resonant frequency is measured with the next equation: 

![ScreenShot seq 1](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq1.png)

So we could obtain the L value because we know the F frequency that we've just measured and we also know the value of the capacitor because it's a component that we've selected. All we need is to obtain L from this equation.


![ScreenShot seq 2](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq2.png)

The comparator will turn the sine wave into a square wave with a duty of 50%(see fig 9 in 
[datasheet](https://www.onsemi.com/pub/Collateral/LM393-D.PDF)),  The microcontroller measures the signal , This measurement can then be doubled to get the period and the inverse of the period is the frequency. Since the circuit is resonating, this frequency is the resonating frequency.

We can now solve the equation for inductance.


**Display ADC in digital value and Voltage.**

This displays the voltage and ADC digital value measured on A0, This can be used for testing potentiometers,
sensors or measuring voltage levels. 
