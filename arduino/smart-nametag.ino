#include <Arduino.h>
#include <Wire.h>
#include "rgb_lcd.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

// WiFi details on line 

// Location of IoT Server, delimiter
const String SERVER_ADDRESS = "";
char delimiter = '|';

// LCD Setup
rgb_lcd lcd;
const int colorR = 0;
const int colorG = 77;
const int colorB = 255;
String upper_line = "";
String lower_line = "";


ESP8266WiFiMulti WiFiMulti;

void parsePayload(String payload) {
  // Reset the lines
  upper_line = "";
  lower_line = "";

  // Iterate till delimiter
  int x = 0;
  for(x;x<payload.length();x++) {
    if(payload[x] != delimiter) {
      upper_line += payload[x];
    }else {
      break;
    }
  }

  // Iterate past delimiter
  x+=1;
  
  // Iterate till end
  for(x;x<payload.length();x++) {
    lower_line += payload[x];
  }
}

void printOnLcd() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(upper_line);
  lcd.setCursor(0, 1);
  lcd.print(lower_line);
}

void setup() {
  // Setup Serial
  Serial.begin(115200);
  
  // Setup the LCD
  lcd.begin(16, 2);
  lcd.setRGB(colorR, colorG, colorB);

  // Announce status
  upper_line = "Connecting to";
  lower_line =  "WiFi";
  printOnLcd();

  // Connect to WiFi
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("WIFI_SSID", "WIFI_PASSWORD");

  // Announce status
  upper_line = "Connected!";
  lower_line =  "";
  printOnLcd();
  delay(1000);
}


void loop() {
  // Do this if WiFi is connected
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    // Connect to server
    HTTPClient http;
    http.begin(SERVER_ADDRESS);

    // Get the response code
    Serial.println("[HTTP] GET...\n");
    int httpCode = http.GET();
    Serial.println(httpCode);

    // If good response code
    if (httpCode > 0) {
      if (httpCode == HTTP_CODE_OK) {
        // Get the payload and parse into top / bottom
        String payload = http.getString();

        // Debug print payload
        Serial.println(payload);

        // Parse the payload
        parsePayload(payload);
      } else {
          upper_line = "HTTP Error:";
          lower_line = "Cannot connect";
          printOnLcd();
      }
    } else {
      Serial.println(http.errorToString(httpCode).c_str());
    }

    // End the HTTP session
    http.end();
  }
  
  // Print on the board
  printOnLcd();

  // Delay 10 seconds
  delay(10000);
}
