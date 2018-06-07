Overview
--------------------
* Name : LR_meter_arduino
* Title : Inductance and Resistance meter, Arduino based.
* Description : Inductance and Resistance meter, Arduino based. 2 tests, 4 resistor test ranges. One Inductance test, Analog joystick input , outputs to serial monitor.  Resistor range is form 0ohms to 2MOhms , Inductor range is 80uH to 30,000uH.
* Author: Gavin Lyons


Software Used
------------------
Arduino 1.8.5

Fritzing 0.9.3 

Eagle 9.0.1


Parts List
------------------------------
The schematic is available in Eagle or fritzing format in documents. 

You will need following parts.

>
> Arduino based board with 8 digital pins and 3 analog pins + serial output. (eg nano)
>
> Resistors: 2kohms, 20kohms, 200kohms, 1Mohms, 150ohm, 330ohm, 
>
> Capacitors: 2* non-polarized 1uF 
>
> Analog joystick five pin slide with integral push button 
>
> Diode:  1N4001
>
> LM393 comparator IC 
>
> Output terminal connections
>


Features
-----------------------------------------------

![ScreenShot schematic](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/lr_meter2.png)


**Resistance test**


This test is activated by Joystick control, You could also use a DIP switch or rotary encoder or set of push buttons in an alternative design. The resistors used in circuit should be measured on good quality meter and there exact values added to code for maximum accuracy.

Right: 0-2kohms : 2K resistor

Left: 2k-20k : 21.3k resistor

Down: 20k-200k : 200K resistor

Up: 200k - 1Mohm : 1 Mohm resistor

So we will use a basic voltage divider to calculate the resistance. 

![ScreenShot seq 1](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq3.png)

A voltage divider is made of two Resistances (R1 and R2) in series. The output voltage in the middle point is [R2/(R1+R2)]Vin. Using this formula and knowing the value of R2 and measuring the Vout calculate the resistance of R1. 
Vin will be 5 volts from the Arduino, We can measure Vout with one of the Arduino's analog inputs.
We need to use different ranges of R2 to measure with better  precision. 

**Inductance test**

This test is activated by push button on joystick.

This test test range is from 80uH to 30,000uH. Ideally the capacitors in circuit must be in specification and measured on a good quality meter. The unknown test inductor is placed in  parallel with a capacitor is creating an LC circuit. 

As soon as the voltage on the LC circuit becomes positive, the LM393 will be floating, which can be pulled high with a pull up resistor. When the voltage on the LC circuit becomes negative, the LM393 will pull its output to ground. 

A 5V pulse signal from Arduino is applied to the LC circuit. We charge the circuit for some time. Then we change the voltage from 5 volts directly to 0. That pulse will make the circuit resonate creating a  sinusoidal signal oscillating at the resonant frequency. The freq is measured and later using the formulas obtain the inductance value. We will use the Arduino to measure the frequency and calculate the value. The resonant frequency is related with the next equation: 

![ScreenShot seq 1](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq1.png)

So we could obtain the L value because we know the F frequency that we've just measured and we also know the values f the C capacitor because it's a component that we've selected. All we need is to obtain L from this equation.


![ScreenShot seq 2](https://github.com/gavinlyonsrepo/LR_meter_arduino/blob/master/documentation/images/eq2.png)

The comparator will turn the sine wave into a square wave with a duty of 50%,  The microcontroller measures the signal , This measurement can then be doubled to get the period and the inverse of the period is the frequency. Since the circuit is resonating, this frequency is the resonating frequency.

We can now solve for the equation for inductance.


Copyright
-------------------------------

Copyright (C) 2018 G Lyons. This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, see license.md for more details

