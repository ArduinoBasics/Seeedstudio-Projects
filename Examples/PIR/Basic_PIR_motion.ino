/*==========================================================================
 * Project: Grove PIR Motion detection
 * Author:  Scott C
 * Date created: 11 Jun 2019
 * Arduino IDE version: 1.8.5
 * Operating System: Windows 10 Pro
 * Tutorial Link: TBA
 * 
 * Description: 
 * Detect motion using the Grove PIR motion detector
 *  
 * Acknowledgements:
 * http://wiki.seeedstudio.com/Grove-PIR_Motion_Sensor/


 ============================================================================
 * Global Variables
 * PIR:       The PIR sensor is connected to digital pin 2
 * detection: Helps to identify when the sensor is detecting movement or not
 ============================================================================*/
int PIR = 2;
boolean detection = false;


/*===========================================================================
 * setup:
 * Initialise Serial communication and set digital pin 2 (PIR) as an input
 ============================================================================*/
void setup() {
  Serial.begin(9600);
  pinMode(PIR,INPUT);
}

/*===========================================================================
 * loop:
 * Print the words "Detected" to the serial monitor only when the
 * PIR sensor changes to a motion detection state.
 * 
 * Print the words "All alone" when the PIR sensor changes to a motionless
 * detection state
 * 
 * The words will only print when the state changes.
 ============================================================================*/
void loop() {
  if(!detection){
    if(digitalRead(PIR)){
      Serial.println("Detected");
      detection = true;
    }
  }

  if(detection){
    if(!digitalRead(PIR)){
      Serial.println("All alone");
      detection = false;
    }
  }
}
