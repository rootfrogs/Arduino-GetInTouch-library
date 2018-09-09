/*
 * Example of GetInTouch Arduino
 * name: basic
 * author: rootfrogs UG
 * 
 * setting on an Arduino MEGA:
 *   connect two LEDs on following pins: 
 *     Pin 13 - LED1 
 *     Pin 12 - LED2
 * 
 * description: 
 *   This example will add two actions to your extension.
 *   ("LED blink" and "annoying apple")
 *   each action will activates one of the LEDs for two seconds.
 */

#include <GetInTouch.h>

//create GetInTouch library object and the actions
GetInTouch git;
GITAction ledBlink("LED blink");
GITAction annoyingApple("annoying apple");

const int LED1 = 13;
const int LED2 = 12;

void setup() {
  //set LED pins to output-mode
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  //initialize the GetInTouch library
  git.init();

  //add your actions to the library
  git.addAction(&ledBlink);
  git.addAction(&annoyingApple);
}

void loop() {
  //call this method frequently to make the GetInTouch library work
  git.run();

  if(ledBlink.isTriggered()) {
      //ledBlink action was activated by a viewer and is now disabled for the viewers till ended() is called
      digitalWrite(LED1, HIGH);
      delay(2000);  //If you don't like the blocking call here, see our parallelizeActions.ino solution
      digitalWrite(LED1, LOW);

      //Set action ended, so viewers can activate them again
      ledBlink.ended();
  }
  
  if(annoyingApple.isTriggered()) {
      //annoyingApple action was activated by a viewer and is now disabled for the viewers till ended() is called
      digitalWrite(LED2, HIGH);
      delay(2000);  //If you don't like the blocking call here, see our parallelizeActions.ino solution
      digitalWrite(LED2, LOW);

      //Set action ended, so viewers can activate them again
      annoyingApple.ended();
  }
}

