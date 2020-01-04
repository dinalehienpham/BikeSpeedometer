# Arduino Color-Changing Bike Wheel Speedometer
## Description
This was a group project for an Arduino microcontrollers class. We created a color-changing, wheel light bike speedometer that would show current MPH and light up green, yellow, or red if the user is biking fast, slowing down, or has stopped. Arduino code and Fritzing files for PCB included.

<img src="https://github.com/dinalehienpham/BikeSpeedometer/blob/master/images/device.jpg" width="352" height="422">

## Materials List
* Arduino Uno
* reed switch and magnet
* NeoPixels
* SSD1306 OLED
* LM7805 voltage regulator
* 3-slide switch
* 9V battery
* resistors (470 Ω, 10 kΩ)
* non-eletcrolytic capacitors (0.22 μF, 0.1 μF) (note: 0.22 μF can be replaced by 0.33 μF)
* 1000 μF eletcrolytic capacitor
* protoboard (Arduino Uno) or PCB
* 0.5" inner diameter clear vinyl tubing
* 3D printing for case

## Circuit
![](https://github.com/dinalehienpham/BikeSpeedometer/blob/master/images/breadboardcircuit.jpg)

<img src="https://github.com/dinalehienpham/BikeSpeedometer/blob/master/images/electronics.jpg" width="375" height="442">
Circuit assembled and soldered onto protoboard.

## Usage
* Requires Arduino IDE for uploading code to the Arduino Uno. 
* Install [u8glib](https://github.com/olikraus/u8glib/wiki) ([more info](http://wiki.sunfounder.cc/index.php?title=OLED-SSD1306_Module)) and [Adafruit NeoPixel](https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-installation) libraries.
* Main body (housing electronics) and NeoPixels LED strip were attached to bike spokes (via zipties). The reed switch should be attached externally on the main body and magnet to the bike frame. Reed switch and magnet should be in close proximity.

![Demo](https://github.com/dinalehienpham/BikeSpeedometer/blob/master/images/demo.gif)

### Additional notes
* Keep in mind that within the Fritzing file, parts for SSD1306 OLED and NeoPixels did not exist so placeholder parts were used instead. 
* Inspiration taken from [Arduino Bike Speedometer](https://www.instructables.com/id/Arduino-Bike-Speedometer/). Includes a more detailed tutorial on using reed switches.

## Authors
* Dina Pham (dlpham@ucdavis.edu)
* Nhi Trinh (nntrinh@ucdavis.edu)

2019 @ UC Davis
