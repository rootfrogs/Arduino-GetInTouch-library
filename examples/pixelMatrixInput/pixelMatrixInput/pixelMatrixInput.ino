/*
 * Example of GetInTouch Arduino
 * name: pixelMatrixInput
 * author: rootfrogs UG
 * 
 * setting on an Arduino MEGA:
 *   connect a WS2812 LED matrix to any pin you want
 *   setup the defines at the beginning of the code
 *   Additionally you will need the Adafruit_NeoPixel library in your Arduino IDE to use this example

 * 
 * Description: 
 *   This example will add a pixel matrix action to your GetInTouch Extension
 *   Your viewers can set the individaual color of each LED on the matrix.
 */

#include <GetInTouch.h>
#include <Adafruit_NeoPixel.h>

//create GetInTouch library object and the action
GetInTouch git;
GITAction ledMatrix("draw a pixel image");

#define MATRIX_PIN         3      //Pin where the WS2821 matrix is connected to
#define MATRIX_COLS        8      //number of coloumns of the matrix (1-32 allowed)
#define MATRIX_ROWS        8      //number of rows of the matrix (1-32 allowed)
#define MATRIX_BRIGHTNESS  10     //The brightness the LEDs should shine (1-255)

Adafruit_NeoPixel neoPixels = Adafruit_NeoPixel(MATRIX_COLS * MATRIX_ROWS, MATRIX_PIN, NEO_GRB + NEO_KHZ800);

void setup() {
  //setup the ws2812 and clear the LEDs
  neoPixels.begin();
  neoPixels.show();

  //add the user input field to the action:
  ledMatrix.addPixelMatrix("matrix", MATRIX_COLS, MATRIX_ROWS);

  //initialize the GetInTouch library
  git.init();

  //add your actions to the library
  git.addAction(&ledMatrix);
}

void loop() {
  //call this method frequently to make the GetInTouch library work
  git.run();

  if(ledMatrix.isTriggered()) {
      //show the user input
      showOnMatrix(ledMatrix.getParameter("matrix"));

      //tell the server that the action is now ended
      ledMatrix.ended();
  }
}

void showOnMatrix(String data)
{
  for(uint8_t i = 0; i < data.length() && i < (MATRIX_COLS * MATRIX_ROWS); i++)
  {
    if(data.charAt(i)=='1')
      neoPixels.setPixelColor(i, MATRIX_BRIGHTNESS,0,0);
    else if(data.charAt(i)=='2')
      neoPixels.setPixelColor(i, 0, MATRIX_BRIGHTNESS,0);
    else if(data.charAt(i)=='3')
      neoPixels.setPixelColor(i, 0, 0, MATRIX_BRIGHTNESS);
    else if(data.charAt(i)=='4')
      neoPixels.setPixelColor(i, MATRIX_BRIGHTNESS, MATRIX_BRIGHTNESS, MATRIX_BRIGHTNESS);
    else if(data.charAt(i)=='5')
      neoPixels.setPixelColor(i, MATRIX_BRIGHTNESS, MATRIX_BRIGHTNESS, 0);
    else if(data.charAt(i)=='6')
      neoPixels.setPixelColor(i, MATRIX_BRIGHTNESS, (MATRIX_BRIGHTNESS >> 1), 0);
    else 
      neoPixels.setPixelColor(i, 0, 0, 0);
  }
  
  neoPixels.show();
}

