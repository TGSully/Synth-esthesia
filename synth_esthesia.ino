/******************************************************************************
ISL29125_basics.ino
Simple example for using the ISL29125 RGB sensor library.
Jordan McConnell @ SparkFun Electronics
11 Apr 2014
https://github.com/sparkfun/ISL29125_Breakout

This example declares an SFE_ISL29125 object called RGB_sensor. The 
object/sensor is initialized with a basic configuration so that it continuously
samples the light intensity of red, green and blue spectrums. These values are
read from the sensor every 2 seconds and printed to the Serial monitor.

Developed/Tested with:
Arduino Uno
Arduino IDE 1.0.5

Requires:
SFE_ISL29125_Library

This code is beerware.
Distributed as-is; no warranty is given. 
******************************************************************************/

#include <Wire.h>
#include "SFE_ISL29125.h"

// Declare sensor object
SFE_ISL29125 RGB_sensor;
const int buttonPin = PUSH2; 
const int octUp = P1_4;//octave up
const int octDwn = P1_5; //octave down
//int buttonState; 

double octave=1;
 
void setup()
{
  // Initialize serial communication
  Serial.begin(9600);
  pinMode(PUSH2, INPUT_PULLUP);  
  pinMode(P1_4, INPUT_PULLUP);
  pinMode(P1_5, INPUT_PULLUP);
  // Initialize the ISL29125 with simple configuration so it starts sampling
  if (RGB_sensor.init())
  {
    Serial.println("Sensor Initialization Successful\n\r");
  }
  
  //int octave=1;
}

// Read sensor values for each color and print them to serial monitor
void loop()
{
  //int octave=1;
  int octUpState;
  int octDwnState;
  int buttonState; 
  buttonState = digitalRead(buttonPin);
  octUpState = digitalRead(octUp);
  octDwnState = digitalRead(octDwn);
  // Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();
  
  // Print out readings, change HEX to DEC if you prefer decimal output
  Serial.print("R: "); Serial.println(red,DEC);
  Serial.print("G: "); Serial.println(green,DEC);
  Serial.print("B: "); Serial.println(blue,DEC);
  Serial.println();
  
  //display color in terminal
  /*int last;
  if(red > green && red > blue && last != 1) {            //between red-orange
      Serial.print("Red");
      tone(2, 220);
      last = 1;
   } else if(green >= red && green > blue && last != 2) {  //between yellow-teal
      Serial.print("Green");
      tone(2, 247);
      last = 2;
   } else if(blue > green && blue > red && last != 3) {
       Serial.print("Blue");
       tone(2, 262);
       last = 3;
   }*/ 
   
   //convert to tone
  uint32_t sum = red;
  sum += green;
  sum += blue;
  //sum = clear;
  float r, g, b;
  r = red; 
  r /= sum;
  g = green; 
  g /= sum;
  b = blue; 
  b /= sum;
  r *= 256; 
  g *= 256; 
  b *= 256;
  if (r > 255) r = 255;
  if (g > 255) g = 255;
  if (b > 255) b = 255;
 ////////////////
  
    float remove, normalize;
  if ((b < g) && (b < r)) {
    remove = b;
    normalize = max(r-b, g-b);
  } 
  else if ((g < b) && (g < r)) {
    remove = g;
    normalize = max(r-g, b-g);
  } 
  else {
    remove = r;
    normalize = max(b-r, g-r);
  }
  // get rid of minority report
  float rednorm = r - remove;
  float greennorm = g - remove;
  float bluenorm = b - remove;
  // now normalize for the highest number
  rednorm /= normalize;
  greennorm /= normalize;
  bluenorm /= normalize;
  
Serial.println(rednorm);
Serial.println(greennorm);
Serial.println(bluenorm);   
 
////////// 
 float rainbowtone = 0;
 double pitch;

  if (bluenorm <= 0.1) {
    // between red and green
    if (rednorm >= 0.99 && greennorm <= 0.55) {
      // between red and yellow
      rainbowtone = 0 + 0.2 * greennorm;
      Serial.println("RED");
      pitch = 220 * octave;
   //   tone(2, 220);
    }else if(rednorm >= 0.99 && greennorm < 0.99) {
      Serial.print("ORANGE");
      pitch = 247 * octave;
     // tone(2, 247);
    }
    else if(rednorm >= 0.5 && greennorm >=0.99){
      // between yellow and green
      rainbowtone = 0.2 + 0.2 * (1.0 - rednorm);
      Serial.println("YELLOW");
      pitch = 277 * octave;
    //  tone(2, 277);
    }
    else{
    //else if(rednorm >= 0.5 && greennorm >=0.99){
      rainbowtone = 0.4 + 0.2 * bluenorm;
      Serial.println("GREEN");
      pitch = 294 * octave;
    }
  } 
  else if (rednorm <= 0.1) {
    // between green and blue
//    if (greennorm >= 0.99 && bluenorm < 0.4) {
//      // between green and teal
//      rainbowtone = 0.4 + 0.2 * bluenorm;
//      Serial.println("GREEN");
//      pitch = 294 * octave;
//    //  tone(2, 294);
//    } 
     if (bluenorm >= 0.99 && greennorm <= 0.6){
      Serial.println("BLUE");
      pitch = 370 * octave;
  //  tone(2, 370);     
     } 
     else if (bluenorm >= 0.99 && greennorm <= 0.99){
      // between teal and blue
      rainbowtone = 0.6 + 0.2 * (1.0 - greennorm);
      Serial.println("TEAL");
      pitch = 330 * octave;
   //   tone(2, 330);
    }
  } else{
      // between teal and blue
      rainbowtone = 0; 
      Serial.println("Violet");
      pitch = 415 * octave;
    //  tone(2, 415);
    }
    Serial.println();
    
  /*else if (greennorm <= 0.4){
    // between blue and violet
    if (bluenorm >=0.99) {
      // between blue and violet
      rainbowtone = 0.8 + 0.2 * rednorm;
      Serial.print("Rubber Ducky");
      Serial.println("BLUE");
      tone(2, 370);
    } 
    else{
      // between teal and blue
      rainbowtone = 0; 
      Serial.println("Violet");
      tone(2, 415);
    }
  }*/
  
  ///////////////octave select//////////////
  
  if(octUpState == LOW) {
    octave *=2;
    delay(250);
    Serial.println("octave up");
  }else if(octDwnState == LOW) {
    octave /=2;
    Serial.println("octave down");
    delay(250);
  }
   
   Serial.print("octave =  ");
   Serial.println(octave);
  
 
  //pitch = pitch * octave;
  ///////////trgger note///////////////////
  if(buttonState == LOW) { //while button pressed, play tone
    //while(buttonState != HIGH){tone(2, pitch);}
    tone(3, pitch);
    Serial.println("button pressed");
  }else if(buttonState == HIGH){
  Serial.println("unpressed");
   noTone(3);
  }
  

   //delay(10);
   //delay(2500);
}
