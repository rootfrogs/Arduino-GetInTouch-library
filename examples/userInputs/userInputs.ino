/*
 * Example of GetInTouch Arduino
 * name: userInputs
 * author: rootfrogs UG
 * 
 * setting on an Arduino MEGA:
 *   connect three LEDs on following pins:
 *     Pin 13 - red LED
 *     Pin 12 - green LED
 *     Pin 11 - blue LED
 *   furthermore connect a LCD
 *     Pin 7 - RS
 *     Pin 6 - Enable
 *     Pin 5 - Data4
 *     Pin 4 - Data5
 *     Pin 3 - Data6
 *     Pin 2 - Data7
 * 
 * Description: 
 *   This example will add two actions to your extension.
 *   - The first action lets the viewer choose a led color, which will start blinking
 *   - The other action displays the username and the entered text of the viewer.
 */

#include <GetInTouch.h>
#include <LiquidCrystal.h>

//create GetInTouch library object and the actions
GetInTouch git;
GITAction ledBlink("LED blink");
GITAction annoyingApple("annoying apple");

const int LED_RED = 13;
const int LED_GRN = 12;
const int LED_BLU = 11;
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

void setup() {
  //set LED pins to output-mode
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GRN, OUTPUT);
  pinMode(LED_BLU, OUTPUT);

  //initialize the LCD
  lcd.begin(16, 2);

  //add the user input fields to the actions:
  ledBlink.addCombobox("select color", "red|green|blue");
  annoyingApple.addTextbox("annoying text");

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
      //activate the choosen LED
      if(ledBlink.getParameter(0).equals("red"))
        digitalWrite(LED_RED, HIGH);
      else if(ledBlink.getParameter(0).equals("green"))
        digitalWrite(LED_GRN, HIGH);
      else if(ledBlink.getParameter(0).equals("green"))
        digitalWrite(LED_BLU, HIGH);

      delay(2000);

      //turn off all LEDs
      digitalWrite(LED_RED, LOW);
      digitalWrite(LED_GRN, LOW);
      digitalWrite(LED_BLU, LOW);

      //tell the server that the action is now ended
      ledBlink.ended();
  }
  
  if(annoyingApple.isTriggered()) {
      lcd.clear();
      lcd.print("User: " + annoyingApple.getUsername());
      lcd.setCursor(0,1);
      lcd.print(annoyingApple.getParameter(0));
      annoyingApple.ended();
  }
}

