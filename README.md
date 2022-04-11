# Scramblizer
A Rubik's cube-inspired kinetic sculpture made with ESP32 and motors, controlled through wifi connection to an online server.

## [Video Demo](https://www.youtube.com/watch?v=LzTKsYzcYKQ)
[![Alien Radio Video Demo](https://img.youtube.com/vi/LzTKsYzcYKQ/maxresdefault.jpg)](https://www.youtube.com/watch?v=LzTKsYzcYKQ)
See detailed explanation [here](https://ryurongliu.github.io/scramblizer/).

## Concept
Scramblizer's three arms rotate outwards, change LED colors randomly, and then rotate back inwards. This evolution mimics the scrambling of a Rubik's cube, where you rotate faces in order to change the colors on each face.

## Tech Specs
- [TTGO T-Display ESP32 WiFi BLE](https://www.amazon.com/LILYGO-T-Display-Arduino-Development-CH9102F/dp/B099MPFJ9M)
- 28BYJ-48 stepper motor 
- SG90 Tower Pro MicroServo (x2)
- Adafruit Neopixel LED strips

## Reproduction
Components are connected to the ESP32 as follows:
- Stepper motor: 
    - Input 1 to pin 12
    - Input 2 to pin 26
    - Input 3 to pin 33
    - Input 4 to pin 25
- Servo 1 to pin 13
- Servo 2 to pin 15
- LED Strip 1 (center) to pin 17
- LED Strip 2 (straight 3) to pin 27
- LED Strip 3 (stright 2) to pin 32
- LED Strip 4 (L-shape 3) to pin 22

Pinouts can be adjusted as needed within [scramblizer.ino](code/scramblizer.ino).<br><br>
This sculpture interacts with an API set up specifically for the class COMS3930 at Barnard (details [here](https://github.com/mbennett12/kinetic-sculpture-webapi)). The API allows the sculpture to be triggered by an online server. This part of the code can be adjusted for different triggering mechanisms, as needed. 
