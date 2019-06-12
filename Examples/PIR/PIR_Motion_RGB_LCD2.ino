/*==========================================================================
 * Project: Grove PIR Motion detection with RGB LCD
 * Author:  Scott C
 * Date created: 11 Jun 2019
 * Arduino IDE version: 1.8.5
 * Operating System: Windows 10 Pro
 * Tutorial Link: TBA
 * 
 * Description: 
 * Detect motion using the Grove PIR motion detector (D2) and print the status to the RGB LCD module.
 * LCD module is connected via I2C.
 *  
 * Acknowledgements:
 * http://wiki.seeedstudio.com/Grove-PIR_Motion_Sensor/
 * http://wiki.seeedstudio.com/Grove-LCD_RGB_Backlight/
 * https://github.com/FastLED/FastLED/blob/master/examples/RGBSetDemo/RGBSetDemo.ino
 * 
 * Libraries: 
 * Wire - built-in to the Arduino IDE
 * rgb_lcd.h - from Seeedstudio
 * FastLed.h - from http://fastled.io  : makes it easier to perform HSV transitions using the RGB LCD
 ============================================================================*/
#include <Wire.h>
#include "rgb_lcd.h"
#include <FastLED.h>


/*============================================================================
 * Global Variables
 * NUM_LEDS   There is only one LED
 * leds       An array to hold the RGB values
 * PIR:       The PIR sensor is connected to digital pin 2
 * detection: Helps to identify when the sensor is detecting movement or not
 * lcd:       Used to control the lcd module. Connected via I2C.
 ============================================================================*/
#define NUM_LEDS 1
CRGBArray<NUM_LEDS> leds;
int PIR = 2;
boolean detection = false;
rgb_lcd lcd;


/*===========================================================================
 * setup:
 * Initialise lcd module (16x2) and set digital pin 2 (PIR) as an input
 * The for-loops are used to create the rainbow effect when the Arduino is powered up
 * clearScreen() is a custom function - see below.
 ============================================================================*/
void setup() {
  lcd.begin(16, 2);
  pinMode(PIR,INPUT);
  int k = 0;
  
  for(int j=0; j<3; j++){
    if(j==1||j==2){
      lcd.setCursor(0,0);
      lcd.print("ArduinoBasics");
    } else {
      clearScreen();
    }
    for(int i=0; i<255; i++){
      leds[0] = CHSV(i,255,k);
      lcd.setRGB(leds[0].r,leds[0].g,leds[0].b);
      if(j==0){
        k++;
      } 
      if(j==2){
        k--;
      }
      delay(20);
    }
  }
  lcd.setRGB(0,0,0);
  clearScreen();
}



/*===========================================================================
 * loop:
 * Print the words "Motion detected" to the LCD module only when the
 * PIR sensor changes to a motion detection state. LCD background = Yellow.
 * 
 * Print the words "Going to sleep" when the PIR sensor changes to a motionless
 * detection state. LCD background = Light blue - that fades to black.
 * 
 * The words will only print when the state changes.
 * clearScreen() is a custom function that clears the LCD screen text
 ============================================================================*/
void loop() {
  if(!detection){
    if(digitalRead(PIR)){
      clearScreen();
      lcd.setRGB(255,255,0);
      lcd.setCursor(0, 0);
      lcd.print("Motion detected ");
      detection = true;
    }
  }

  if(detection){
    if(!digitalRead(PIR)){
      clearScreen();
      lcd.setRGB(0,255,255);
      lcd.setCursor(0, 1);
      lcd.print("Going to sleep  ");
      detection = false;
      for(int i=50; i>0; i--){
        lcd.setRGB(0,i,i);
        delay(20);
      }
      clearScreen();
      lcd.setRGB(0,0,0);
    }
  }
}

/*===========================================================================
 * clearScreen:
 * This function was created to make it easier to clear the LCD screen text
 ============================================================================*/
void clearScreen(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
}
