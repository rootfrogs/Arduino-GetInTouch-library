/*
 * Example of GetInTouch Arduino
 * name: simpleAction
 * author: rootfrogs UG
 * 
 * description: 
 *   This example will add an action "LED Blink" to your Extension.
 *   This action will switch on the LED on the Arduinoboard for two seconds when the Action is triggered over the Twitch Panel.
 */

#include <GetInTouch.h>

//create GetInTouch library object and the actions
GetInTouch git;
GITAction ledBlinkAction("LED Blink");

const int LED1Pin = 13;

void setup() {
  //set LED pin to output-mode
  pinMode(LED1Pin, OUTPUT);

  //initialize the GetInTouch library
  git.init();

  //add your action to the library
  git.addAction(&ledBlinkAction);
}

void loop() {
  //call this method frequently to make the GetInTouch library work
  git.run();

  if(ledBlinkAction.isTriggered()) {
      //ledBlinkAction was activated by a viewer and is now disabled for the viewers till ended() is called
      digitalWrite(LED1Pin, HIGH);
      delay(2000);  //If you don't like the blocking call here, see our parallelActions.ino solution
      digitalWrite(LED1Pin, LOW);

      //Set action ended, so viewers can activate them again
      ledBlinkAction.ended();
  }
}

