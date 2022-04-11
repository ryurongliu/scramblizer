#include <WiFi.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial
#include <ESP32Servo.h>

//**************************************
//wifi connection stuff

const char *ssid_Router     = "Columbia University"; //Enter the router name
const char *password_Router = ""; //Enter the router password

String address= "http://134.122.113.13/ryl2113/running";

//*************************************
//motor connections

// Conncet the port of the stepper motor driver
int outPorts[] = {12, 33, 26, 25};

Servo myservo;  // create servo object to control a servo
Servo myservo2;

int posVal = 0;    // variable to store the servo position
int servoPin = 13; // Servo motor pin
int servo2Pin = 15;

//**************************************
//vars for motors 

int turning = 0; //keep track
int servo1Pos = 60;
int servo2Pos = 0;

//**************************************
//neopixel led stuff

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
int strip1Pin= 17;// On Trinket or Gemma, suggest changing this to 1
int strip2Pin = 27; 
int strip3Pin = 32;
int strip4Pin = 22;

// How many NeoPixels are attached to the Arduino?
int strip1Num = 1;// Popular NeoPixel ring size
int strip2Num = 3;
int strip3Num = 2;
int strip4Num = 3;

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel strip1(strip1Num, strip1Pin, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip2(strip2Num, strip2Pin, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip3(strip3Num, strip3Pin, NEO_RGB + NEO_KHZ800);
Adafruit_NeoPixel strip4(strip4Num, strip4Pin, NEO_RGB + NEO_KHZ800);

//LED colors used
int colors[][3] = {{255, 0, 0}, {255, 0, 255}, {0, 255, 0}, {0, 0, 255}, {255, 255, 255}, {255, 255, 0}}; 

//variables to keep track of current light colors
int s1c = -1;
int s2c[] = {-1, -1, -1};
int s3c[] = {-1, -1};
int s4c[] = {-1, -1, -1};

//*************************************



void setup(){
  
  USE_SERIAL.begin(115200);

  //*********************************
  //wifi connections....
  WiFi.begin(ssid_Router, password_Router);
  USE_SERIAL.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println("\nConnected, IP address: ");
  USE_SERIAL.println(WiFi.localIP());
  USE_SERIAL.println("Setup End");

  //*********************************
  //motors.........
  
  // set pins to output for stepper
  for (int i = 0; i < 4; i++) {
    pinMode(outPorts[i], OUTPUT);
  }

  //initiate servo 1
  myservo.setPeriodHertz(50);           // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500);  // attaches the servo on servoPin to the servo object
  myservo.write(servo1Pos);

  //initiate servo 2
  myservo2.setPeriodHertz(50);           // standard 50 hz servo
  myservo2.attach(servo2Pin, 500, 2500);  // attaches the servo on servoPin to the servo object
  myservo2.write(servo2Pos);
  delay(15);

  //*********************************
  //LEDs.......
  
  #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif
  // END of Trinket-specific code.


  //center LED
  strip1.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip1.show();

  //set to color 5 (yellow)
  for(int i = 0; i < strip1Num; i++){
    int color = 5;
    strip1.setPixelColor(i, strip1.Color(colors[color][0], colors[color][1], colors[color][2]));
    s1c = color; 
    Serial.println(color);
  }
  strip1.show();

  //first moving group of lights
  strip2.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip2.show();
  //set lights to random colors
  for(int i = 0; i < strip2Num; i++){
    int color = random(6);
    strip2.setPixelColor(i, strip2.Color(colors[color][0], colors[color][1], colors[color][2]));
    s2c[i] = color;
    Serial.println(color);
  }
  strip2.show();

  //second moving group of lights
  strip3.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip3.show();
  for(int i = 0; i < strip3Num; i++){
    int color = random(6);
    strip3.setPixelColor(i, strip3.Color(colors[color][0], colors[color][1], colors[color][2]));
    s3c[i] = color;  
    Serial.println(color);
  }
  strip3.show();

  //third moving group of lights
  strip4.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  strip4.show();
  for(int i = 0; i < strip4Num; i++){
    int color = random(6);
    strip4.setPixelColor(i, strip4.Color(colors[color][0], colors[color][1], colors[color][2]));
    s4c[i] = color; 
    Serial.println(color);
  }
  strip4.show();
  
}




//********************************
//motor functions....

//Suggestion: the motor turns precisely when the ms range is between 3 and 20
void moveSteps(bool dir, int steps, byte ms) {
  for (unsigned long i = 0; i < steps; i++) {
    moveOneStep(dir); // Rotate a step
    delay(constrain(ms,3,20));        // Control the speed
  }
  turning = 0;
}

void moveOneStep(bool dir) {
  // Define a variable, use four low bit to indicate the state of port
  static byte out = 0x01;
  // Decide the shift direction according to the rotation direction
  if (dir) {  // ring shift left
    out != 0x08 ? out = out << 1 : out = 0x01;
  }
  else {      // ring shift right
    out != 0x01 ? out = out >> 1 : out = 0x08;
  }
  // Output singal to each port
  for (int i = 0; i < 4; i++) {
    digitalWrite(outPorts[i], (out & (0x01 << i)) ? HIGH : LOW);
  }
}

//********************************




 
void loop() {
  
  if((WiFi.status() == WL_CONNECTED)) {
    HTTPClient http;
    http.begin(address);
 
    int httpCode = http.GET(); // start connection and send HTTP header
    if (httpCode == HTTP_CODE_OK) { 
        String response = http.getString();
        if (response.equals("false")) {
            // Do not run sculpture, perhaps sleep for a couple seconds
        }
        else if(response.equals("true")) {
            // Run sculpture
 
            if (turning == 0) {
              turning = 1;
              // first rotate stepper
              for (int i = 0; i < 340; i++){
                moveSteps(false, 1, 3);
                delay(10);
                
              }
              
              //rotate 60 degrees on servo 1
              for (int i=60; i >=0; i--){
                servo1Pos = i;
                myservo.write(servo1Pos);
                delay(60);
              }

              //rotaate 60 degrees on servo 2
              for (int i=0; i <= 60; i++){
                servo2Pos = i;
                myservo2.write(servo2Pos);
                delay(60);
              }
              
              //wait  a bit
              delay(1000);

              //change colors on moving group 1
              for(int i = 0; i < strip2Num; i++){
                int color = s2c[i];
                while (color == s2c[i]){   // make sure color changes
                  color = random(6);
                }
                strip2.setPixelColor(i, strip2.Color(colors[color][0], colors[color][1], colors[color][2]));
                strip2.show();
                delay(200);
              }
              strip2.show();

              //change colors on moving group 2
              for(int i = 0; i < strip3Num; i++){
                int color = s3c[i];
                while (color == s3c[i]){
                  color = random(6);
                }
                strip3.setPixelColor(i, strip3.Color(colors[color][0], colors[color][1], colors[color][2]));
                strip3.show();
                delay(200);
              }
              strip3.show();

              //change colors on moving group 3
              for(int i = 0; i < strip4Num; i++){
                int color = s4c[i];
                while (color == s4c[i]){
                  color = random(6);
                }
                strip4.setPixelColor(i, strip4.Color(colors[color][0], colors[color][1], colors[color][2]));
                strip4.show();
                delay(200);
              }
              strip4.show();

              //wait a bit
              delay(1000); 

              
              //rotate motors back
              for (int i = 0; i < 340; i++){
                moveSteps(true, 1, 3);
                delay(10);
              }
              
              for (int i=0; i <= 60; i++){
                servo1Pos = i;
                myservo.write(servo1Pos);
                delay(60);
              }
              
              for (int i=60; i >= 0; i--){
                servo2Pos = i;
                myservo2.write(servo2Pos);
                delay(60);
              }

              //wait a bit
              delay(4000);
            }
        }
        USE_SERIAL.println("Response was: " + response);
    } else {
        USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }
    http.end();
    delay(500); // sleep for half of a second
  }
}
