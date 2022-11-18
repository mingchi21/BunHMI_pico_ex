# BunHMI arduino demo
This project demo how to use arduino interact with BunHMI.

# BunHMI
For BunHMI Display detail, please check https://shop.mcuidea.com/

The directory [BunHMI] contain the BunMaker Project. BunMaker is free to [Downlaod](https://shop.mcuidea.com/bunmaker)

# Arduino
This demo is using RPi pico. For using pico Please check https://github.com/earlephilhower/arduino-pico

The directory [arduino_pico] contain the demo code of arduino pico.

# Hardware
This schmatic diagram of this demo is showing as following. The pico commnuicate with BunHMI by UART interface.

Server motor is controlled by PWM and VR is the simulation of Analog input.

![RPI-pico link with BunHMI](./images/BunHMI_link.png)