/*
 * Example of GetInTouch Arduino
 * name: parallelizeActions
 * author: rootfrogs UG
 * 
 * setting:
 * connect a LED on pin 13 and 10
 * 
 * description: 
 * This example shows how to handle long time running actions.
 * Another action can be started during the first one runs.
 * action 1 (LED blink):
 *   let the LED on pin 13 blink for 6 seconds
 * action 2 (annoying apple):
 *   turns the LED on pin 10 on for 6 seconds
 */

#include <GetInTouch.h>

//create GetInTouch library object and the actions
GetInTouch git;
GITAction ledBlink("LED blink");
GITAction annoyingApple("annoying apple");

void setup() {
  //set LED pins to output-mode
  pinMode(13, OUTPUT);
  pinMode(10, OUTPUT);
  
  //initialize the GetInTouch library
  git.init();
  
  //add your actions to the library
  git.addAction(&ledBlink);
  git.addAction(&annoyingApple);
}

void loop() {
  delay(10);
  
  git.run(); //call this method frequently to make the GetInTouch library work

  //If an action was triggered from you viewers, the isTriggered() method will return true. 
  //Keep in mind: The method returns true until ended() method is called
  if(ledBlink.isTriggered()) {
      doLedBlink();		//ledBlink was triggered, do the LED stuff
  }
  if(annoyingApple.isTriggered()) {
      doAnnoyingApple();	//annoyingApple was triggered, do the annoyingApple stuff
  }
}
void doLedBlink()
{
  //Do your action things here. Rmember: This method will be called until you call the ended() method.
  static int cnt = 0;
  if(!(cnt % 20)) {
    digitalWrite(13, !digitalRead(13));
  }
  cnt++;
  if(cnt >= 600) {
    cnt = 0;
    digitalWrite(13, LOW);

    //Now, the LED blinking is finished and we can call the ended() method. 
    //This will release the action and the next viewer can trigger it:
    ledBlink.ended();
  }
}
void doAnnoyingApple()
{
  //Do your action things here. Rmember: This method will be called until you call the ended() method.
  static int cnt = 0;
  if(cnt == 0) {
    digitalWrite(10, HIGH);
  }
  cnt++;
  if(cnt >= 600) {
    cnt = 0;
    digitalWrite(10, LOW);

    //Now, the LED blinking is finished and we can call the ended() method. 
    //This will release the action and the next viewer can trigger it:
    annoyingApple.ended();
  }
}


