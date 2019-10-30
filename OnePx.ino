// Welcome to the one pixel browser example code!

//-----------------------------------------------------------------------------------------------------------------------
// First, let's include the libraries we will need
//-----------------------------------------------------------------------------------------------------------------------

// documentation: https://www.arduino.cc/en/Reference/WiFi (WiFi methods under "WiFi class" section)
#include <WiFi.h>

// documentation: https://links2004.github.io/Arduino/dd/d8d/class_h_t_t_p_client.html#a24fc03fd795d0dbabbdb17f88908b17b
#include <HTTPClient.h>

// documentation: https://learn.adafruit.com/adafruit-neopixel-uberguide/arduino-library-use
#include <Adafruit_NeoPixel.h>

// documentation: https://arduinojson.org/v6/doc/
#include <ArduinoJson.h>

//-----------------------------------------------------------------------------------------------------------------------
// Next we'll initialize our variables
//-----------------------------------------------------------------------------------------------------------------------

// ssid and password for WiFi
const char* ssid = "your ssid";
const char* password = "your wifi password";

// initialize what pin will be the ledoutput pin and how many LEDs are in the strip
const int LED_PIN = 16;
const int LED_COUNT = 1;

// initializes the strip of LEDs. The third input can varry depending on what brand of LEDs are being used. se documentation for more details
Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, LED_PIN);

// allocates 800 bytes of space for json object. figure out how many bytes to allocate based on certain json here: https://arduinojson.org/v6/assistant/
DynamicJsonDocument jsonDoc(800);

// previous temperature for LED effect purposes
float previous_temp = 0;

// http request formatted according to openweathermap api (Note: format will vary depending on API)
const String endpoint = "http://api.openweathermap.org/data/2.5/weather?q=Cleveland,us&APPID=";
// API key
const String key = "your key here";

//-----------------------------------------------------------------------------------------------------------------------
// This function will run when the microcontroller starts up
//-----------------------------------------------------------------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  // join wifi
  setup_WiFi();

  // prepair LED strip
  init_LED_off();
}

//-----------------------------------------------------------------------------------------------------------------------
// This function will loop over and over the whole time the microcontroller is on
//-----------------------------------------------------------------------------------------------------------------------

// reads temperature value from API every 10 seconds and changed LED color based on the temperature value
void loop() {
  // if esp is connected to the WiFi
  if(WiFi.status() ==  WL_CONNECTED) {

    // httpclient that will make the API calls
    HTTPClient http;

    // call to API
    http.begin(endpoint + key);

    // status of http request (will be > 0 if http call was successful)
    int httpCode = http.GET();

    // if http request was successful
    if(httpCode > 0){
      // data from API in string form (json formatting)
      String payload = http.getString();

      // turn payload into deserialized json
      DeserializationError error = deserializeJson(jsonDoc,payload);

      // if there was an error deserializing the json then print that error
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.c_str());
      }

      // TODO: else?

      // grab temerature (in kelven) from json and convert it to fahrenheit
      float temp_F = ((float)(jsonDoc["main"]["temp"]) - 273.15) * 9.0/5.0 + 32; // TODO: k to f func
      // TODO: examples of other stuff

      // print temperature to serial
      Serial.println(temp_F);

      // change color of LED based on temperature
      color_from_temp(temp_F);
      
      previous_temp = temp_F;
    }
    // print error message if failed http request
    else{
      Serial.println("Error on HTTP Request");
    }
    http.end();
  }
  delay(10000);
}

//-----------------------------------------------------------------------------------------------------------------------
// This function just makes it easier to connect to WiFi in the functions above
//-----------------------------------------------------------------------------------------------------------------------

// connect to WiFi
void setup_WiFi() {
  WiFi.begin(ssid, password);
 
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to the WiFi");
  }
  Serial.println("Connected to the WiFi");
}

//-----------------------------------------------------------------------------------------------------------------------
// This function just makes it easier to start the LED in the functions above
//-----------------------------------------------------------------------------------------------------------------------

// initialize LED strip as off
void init_LED_off() {
 
  // initialize LED
  strip.begin();                 // strip.begin(): prepair data pin for neopixel output
  strip.show();                  // strip.show(): takes the color that each LED in the strip is currently set to and displayes it on the LEDs

  // turns off LED
  strip.clear();                 // sets all LEDs in strip to off (RGB value 0, 0, 0)
  strip.show();                  // must have strip.show() after every time the pixel colors are changed otherwise LEDs will not display the set color
}

//-----------------------------------------------------------------------------------------------------------------------
// This function makes the LED blink to show the temperature
//-----------------------------------------------------------------------------------------------------------------------

// how the LED behaves based on different temp values
void color_from_temp(float temp) {
  // warm and cold temperature values (in fahrenheit, change based on preference) TODO: const at top
  float warm_temp = 75;
  float cold_temp = 60;
 
  // if the temperature increased since last time then blink red
  if (temp > previous_temp) {
    blinkRed();
  }
  // if the temperature decreased since last time then blink blue
  else if(temp < previous_temp) {
    blinkBlue();
  }

  // if the temperature is higher then warm temperature then setPixelColor to red
  if(temp > warm_temp) {
    strip.setPixelColor(0,255,0,0);
  }
  // if the temperature is a medium temperature then setPixelColor to green
  else if(temp < warm_temp && temp > cold_temp) {
    strip.setPixelColor(0,0,255,0);
  }
  // if the temperature is a cold temperature then setPixelColor to blue
  else {
    strip.setPixelColor(0,0,0,255);
  }
  strip.show();
}

//-----------------------------------------------------------------------------------------------------------------------
// This function makes the LED blink red, for use in color_from_temp
//-----------------------------------------------------------------------------------------------------------------------

// make LED blink red
void blinkRed() {
  // turn off LED
  strip.show();
  strip.clear();
  strip.show();
 
  delay(500);

  // set LED to red
  strip.setPixelColor(0,255,0,0);   // strip.setPixelColor(pixel (which LED (0 indexed), red value (0 to 255), green value (0 to 255), blue value (0 to 255))
  strip.show();
 
  delay(1000);

  // turn off LED
  strip.clear();
  strip.show();
 
  delay(500);
}

//-----------------------------------------------------------------------------------------------------------------------
// This function makes the LED blink blue, for use in color_from_temp
//-----------------------------------------------------------------------------------------------------------------------

// make LED blink blue
void blinkBlue() {
  // turn off LED
  strip.show();
  strip.clear();
  strip.show();
 
  delay(500);
 
  // set LED to blue
  strip.setPixelColor(0,0,0,255);
  strip.show();
 
  delay(1000);
 
  // turn off LED
  strip.clear();
  strip.show();
 
  delay(500);
}
