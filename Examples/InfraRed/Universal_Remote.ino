/*==========================================================================
 * Project: ArduinoBasics Universal Remote
 * Author:  Scott C
 * Date created: 09 March 2019
 * Arduino IDE version: 1.8.5
 * Operating System: Windows 10 Pro
 * Tutorial Link: https//arduinobasics.blogspot.com/
 * 
 * Description: 
 * This project will turn an ordinary Keyes IR remote into a programmable multi-signal universal remote.
 * I will assign up to 3 SONY signal combinations onto a single Keyes remote button.
 * This sketch will allow you to program any 5 of the 17 possible buttons on the Keyes remote.
 * You can assign signal combinations from different remotes onto the single Keyes remote button.
 * An example combination could be to 
 *  - Turn on the TV and switch to a particular channel.
 *  - Turn on the TV and sound system at the same time
 *  - Turn up the volume x 3.
 *  
 *  Limitations of this sketch:
 *  You have to re-program the Key combinations every time you switch the Arduino off and on.
 *  The program WILL NOT allow duplicate signals for the Keyes remote. 
 *  If duplicates are detected, you will need to start over. You will need to Reset the Arduino.
 *  You can have duplicate signals for the sony remote combinations. 
 * 
 * -------------------------------------------------------------------------   
 * LIBRARIES:            
 * All of the IRLib***.h libraries listed below are from the IRLib2 library github page. Only the relevant libraries from that
 * github download are included in the sketch. Please note, if you plan to use a different IR protocol/remote, 
 * you may need to include some additional libraries.
 * The rgb_lcd and the Wire libraries are required to display the text on the LCD panel.
 * 
 * IRLib2 Library:  https://github.com/cyborg5/IRLib2
 * rgb_lcd library: https://github.com/Seeed-Studio/Grove_LCD_RGB_Backlight
 * Wire.h library is built into the Arduino IDE
 ============================================================================ */
#include <IRLibRecv.h>
#include <IRLibDecodeBase.h>  
#include <IRLibSendBase.h>    
#include <IRLib_P01_NEC.h>    
#include <IRLib_P02_Sony.h>   
#include <IRLibCombo.h>       
#include <Wire.h>
#include <rgb_lcd.h>


/* -------------------------------------------------------------------------- 
 *  CONSTANTS:
 *  The following constants are used for sizing the arrays
 *  arrSizeA:     The maximum number of buttons to use on the Keyes remote (because of sketch memory condsiderations)
 *  arrSizeB:     The maximum number of signal combinations per button.
 *  
 *  The following constants are used for Sensor and module pin assignments
 *  IR_RECV_PIN : IR receiver on digital Pin 2
 *  IR_SEND_PIN : IR transmitter on digital Pin 3   - cannot be changed.
 *  button1_PIN : Tactile Button on digital Pin 5
 ----------------------------------------------------------------------------*/
const int arrSizeA = 5;       
const int arrSizeB = 3;       

const int IR_RECV_PIN = 2;    
const int IR_SEND_PIN = 3;    
const int button1_PIN = 5;    


/* --------------------------------------------------------------------------
 *  GLOBAL VARIABLES:
 *  IR_sender:          Initalisation of the IR transmitter. Used to send signals to the TV. Defaults to digital pin 3.
 *  IR_receiver:        Initialisation of the IR receiver. Used to receive/record signals from all of the remotes. I used digital pin 2.
 *  decoded_Keyes_signals: The array used to hold the signals from the Keyes remote
 *  decoded_IR_signals: The array used to the hold the signal combinations from the Sony remote.
 *  IR_signal_reps:     SONY remotes transmit the signal 3 times. The number of reps will depend on the signal being transmitted.
 *  code_Breaker:       This variable is used later to identify when the button is being pressed.
 *  lcd:                This is used to control the LCD display.
 ----------------------------------------------------------------------------*/
IRsend IR_sender;
IRrecv IR_receiver(IR_RECV_PIN);
IRdecode decoded_Keyes_signals[arrSizeA];
IRdecode decoded_IR_signals[arrSizeA][arrSizeB];
uint8_t IR_signal_reps = 0;
boolean code_Breaker = false;
rgb_lcd lcd;


/* --------------------------------------------------------------------------
 *  SETUP:
 *  lcd.begin :       Initialise the 16 x 2 character LCD display
 *  displayText:      This user defined function provides an easy way to display text on the LCD
 *  button1_PIN:      Pin 5 on the Arduino is declared as an INPUT using the pinMode() function, which allows us to monitor button presses using the digitalRead() function.
 *  getCodes:         This user defined function is run once, to map Sony remote signal combinations to 5 buttons on the Keyes remote 
 ----------------------------------------------------------------------------*/
void setup() {
   lcd.begin(16, 2);
   displayText("ArduinoBasics","TV Remote",3000);
   displayText("Press Button","** to start **",1000); 
   pinMode(button1_PIN,INPUT);
   while(digitalRead(button1_PIN)==LOW){
      //waiting for button press to continue
   }
   getCodes();
}


/* --------------------------------------------------------------------------
 *  LOOP:
 *  The loop funtion begins after all of the codes have been mapped to the Keyes remote. 
 *  The Arduino sits here listening out for signals from the Keyes remote.
 *  When it recognises the signal, it will transmit the corresponding Sony remote signal combination.
 ----------------------------------------------------------------------------*/
void loop() {
  // put your main code here, to run repeatedly:
  displayText("Listening for","Signal.......",100);
  listening_for_signal();
}


/* --------------------------------------------------------------------------
 *  displayText:
 *  This user defined function helps to simplify the process of displaying text on the LCD
 *  line1:        The text to display on the first line of the LCD
 *  line2:        The text to display on the second line of the LCD
 *  displayTime:  The amount of time to display the text, before it continues on to the next section of code.
 ----------------------------------------------------------------------------*/
void displayText(String line1, String line2, int displayTime){
  lcd.clear();
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
  delay(displayTime);
}


/* --------------------------------------------------------------------------
 *  getCodes:
 *  This user defined function maps the Sony signal combinations to 5 buttons on the Keyes remote.
 *  There are 17 buttons on the Keyes remote. This sketch will allow you to program ANY 5 of those 17 buttons.
 *  Up to 3 Sony signals will be mapped or assigned to each Keyes button.
 *  You don't have to assign a Sony signal - but you may need to import an additional library if you choose a different protocol
 *  If you don't want to associate ANY signal to a particular Keyes button, or if you want less than 3 signals, you can skip that step by pressing the Tactile button.
 ----------------------------------------------------------------------------*/
void getCodes(){
  //obtain the IR code for 5 buttons on the Keyes remote
  for(int i=0; i<arrSizeA; i++){
    IR_receiver.enableIRIn();
    displayText("Keyes Signal #",String(i+1),1000);
    while(!IR_receiver.getResults()){
      //wait until we receive a signal
    }
    decoded_Keyes_signals[i].decode();
    displayText(String(Pnames(decoded_Keyes_signals[i].protocolNum)),String(decoded_Keyes_signals[i].value,HEX),1000);
    displayText("Code Bits",String(decoded_Keyes_signals[i].bits),1000);

    //Check for duplicates - no duplicates allowed.
    if(i>0){
      for(int d=0; d<i; d++){
        if(decoded_Keyes_signals[i].value==decoded_Keyes_signals[d].value){
          displayText("Error:Duplicates","Restart required",1000);
          while(true){
            //restart required.
          }
        }
      }
    }
    
    //Record up to 3 Sony signal combinations for each button on the Keyes remote.
    for(int j=0; j<arrSizeB; j++){
      IR_receiver.enableIRIn();
      code_Breaker = false;
      displayText("**Sony Signal #",String(j+1),1000);

      //wait until we receive a signal or a button press
      while(!IR_receiver.getResults()){
        if(digitalRead(button1_PIN)==HIGH){
          code_Breaker = true;
          break;
        }
      }
      
      //Only decode a signal if a signal was received. 
      //But not when a button was pressed. If a tactile button is pressed, it will move to the next Keyes assignment.
      if(!code_Breaker){
        decoded_IR_signals[i][j].decode();
        displayText(String(Pnames(decoded_IR_signals[i][j].protocolNum)),String(decoded_IR_signals[i][j].value,HEX),1000);
        displayText("Code Bits",String(decoded_IR_signals[i][j].bits),1000);
      } else {
        displayText("Button Pressed", "No signal recd", 2000);
        break;
      }
    }
  }
  displayText("Record Mode","COMPLETE****",2000);
}


/* --------------------------------------------------------------------------
 *  listening_for_signal:
 *  This user defined function is called in loop().
 *  The purpose of this function is to receive a Keyes IR signal, and then transmit the associated Sony signal combination.
 *  There is some extra processing required when transmitting SONY signals.
 *  The Sony protocol (protocolNum 2) has been programmed to transmit the signal 3 times.
 *  The codes being transmitted are displayed on the LCD for confirmation and debugging.
 ----------------------------------------------------------------------------*/
void listening_for_signal(){
  IRdecode universalRemote;
  IR_signal_reps = 1;
  IR_receiver.enableIRIn();  
  while(!IR_receiver.getResults()){
      //wait until we receive a signal
  }
  universalRemote.decode();
  for(int i=0; i<arrSizeA; i++){
    if(universalRemote.value==decoded_Keyes_signals[i].value){
      //A known Keyes signal was received. We will now transmit the SONY signal combination.
      for(int j=0; j<arrSizeB; j++){
        if(decoded_IR_signals[i][j].protocolNum > 0){
          if(decoded_IR_signals[i][j].protocolNum ==2){
            IR_signal_reps = 3; //Sony Remotes send the signal 3 times.
          }
          for(int k=0; k<IR_signal_reps; k++){
            IR_sender.send(decoded_IR_signals[i][j].protocolNum,decoded_IR_signals[i][j].value,decoded_IR_signals[i][j].bits);
          }
          displayText("Sent:",String(decoded_IR_signals[i][j].value,HEX),1000);
        }
      }
    }
  }
}
