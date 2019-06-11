/*==========================================================================
 * Project: Grove PIR Motion detection with RGB LCD
 * Author:  Scott C
 * Date created: 11 Jun 2019
 * Arduino IDE version: 1.8.5
 * Operating System: Windows 10 Pro
 * Tutorial Link: TBA
 * 
 * Description: 
 * Detect motion using the Grove PIR motion detector and print the status to the RGB LCD module.
 * LCD module is connected via I2C.
 *  
 * Acknowledgements:
 * http://wiki.seeedstudio.com/Grove-PIR_Motion_Sensor/
 * http://wiki.seeedstudio.com/Grove-LCD_RGB_Backlight/


/*============================================================================
 * Global Variables
 * PIR:       The PIR sensor is connected to digital pin 2
 * detection: Helps to identify when the sensor is detecting movement or not
 * lcd:       Used to control the lcd module
 ============================================================================*/
#include <Wire.h>
#include "rgb_lcd.h"


int PIR = 2;
boolean detection = false;
rgb_lcd lcd;


/*===========================================================================
 * setup:
 * Initialise lcd module (16x2) and set digital pin 2 (PIR) as an input
 * Set the background of the lcd to BLUE
 ============================================================================*/
void setup() {
  lcd.begin(16, 2);
  pinMode(PIR,INPUT);
  lcd.setRGB(0,0,255);
}

/*===========================================================================
 * loop:
 * Print the words "Detected" to the LCD module only when the
 * PIR sensor changes to a motion detection state. LCD background = RED.
 * 
 * Print the words "All alone" when the PIR sensor changes to a motionless
 * detection state. LCD background = GREEN
 * 
 * The words will only print when the state changes.
 * The words overwrite each other because they are on the same line and 
 * are the same length
 ============================================================================*/
void loop() {
  if(!detection){
    if(digitalRead(PIR)){
      lcd.setRGB(255,0,0);
      lcd.setCursor(0, 1);
      lcd.print("Detected ");
      detection = true;
    }
  }

  if(detection){
    if(!digitalRead(PIR)){
      lcd.setRGB(0,255,0);
      lcd.setCursor(0, 1);
      lcd.print("All alone");
      detection = false;
    }
  }
}
