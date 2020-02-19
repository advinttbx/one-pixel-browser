// Welcome to the one pixel browser example code!

//-----------------------------------------------------------------------------------------------------------------------
// First, let's include the libraries we will need
//-----------------------------------------------------------------------------------------------------------------------

// documentation: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
#include <Adafruit_NeoPixel.h>

// documentation: https://arduinojson.org/v6/doc/
#include <ArduinoJson.h>

//-----------------------------------------------------------------------------------------------------------------------
// Next we'll initialize our variables
//-----------------------------------------------------------------------------------------------------------------------

// initialize what pin will be the ledoutput pin and how many LEDs are in the led
const int LED_PIN = 16;
const int LED_COUNT = 1;


// initializes the led of LEDs. The third input can varry depending on what brand of LEDs are being used. se documentation for more details
Adafruit_NeoPixel led = Adafruit_NeoPixel(LED_COUNT, LED_PIN);

//-----------------------------------------------------------------------------------------------------------------------
// This function will run when the microcontroller starts up
//-----------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);
  
  // prepair LED
  init_LED_off();
}

//-----------------------------------------------------------------------------------------------------------------------
// This function just makes it easier to start the LED in the functions above
//-----------------------------------------------------------------------------------------------------------------------

void init_LED_off() {
 
  // initialize LED
  led.begin();                 // led.begin(): prepair data pin for neopixel output
  led.show();                  // led.show(): takes the color that each LED is currently set to and displayes it on the LEDs

  clearLED();                  // must have led.show() after every time the pixel colors are changed otherwise LEDs will not display the set color
}

//-----------------------------------------------------------------------------------------------------------------------
// This function will loop over and over the whole time the microcontroller is on
//-----------------------------------------------------------------------------------------------------------------------

void loop() {
  // example loop code
  setColor(255,0,0);
  blinkLED();
  setColor(0,255,0);
  blinkLED();
  setColor(0,0,255);
  blinkLED();
}

//-----------------------------------------------------------------------------------------------------------------------
// This function clears the LED
//-----------------------------------------------------------------------------------------------------------------------

void clearLED() {
  led.clear();
  led.show();
}

//-----------------------------------------------------------------------------------------------------------------------
// This function changes the color of the LED
//-----------------------------------------------------------------------------------------------------------------------

void setColor(int RedValue, int GreenValue, int BlueValue) {
  uint32_t color = led.getPixelColor(0);

  // if the LED is already set to the input color then nothing happens
  if (color != led.Color(RedValue,GreenValue,BlueValue)) {
    clearLED();
  
    led.setPixelColor(0,RedValue,GreenValue,BlueValue);
    led.show();
  }
}

//-----------------------------------------------------------------------------------------------------------------------
// This function makes the LED blink
//-----------------------------------------------------------------------------------------------------------------------

void blinkLED() {                        // what inputs could we add to change this?
  uint32_t color = led.getPixelColor(0);  
    
  clearLED();
  
  delay(1000);

  led.setPixelColor(0, color);
  led.show();

  delay(1000);
}

// what else can we make the LED do?
