/**The MIT License (MIT)

Copyright (c) 2015 by Daniel Eichhorn

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Wire.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include "WeatherClient.h"

WeatherClient weather;
Ticker ticker;

// your network SSID (name)
char ssid[] = "<put here your SSID from your wireless network>";
// your network password
char pass[] = "<put here your password from your wireless network>";  

// Go to forecast.io and register for an API KEY
String forecastApiKey = "<put here the API-key from www.forecast.io";

// Coordinates of the place you want weather information for
double latitude = 51.5;
double longitude = 5.0;

// flag changed in the ticker function every 10 minutes
bool readyForWeatherUpdate = true;

void setup() {
  Serial.begin(9600);
  Serial.println();
  Serial.println();
  
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // update the weather information every 10 mintues only
  // forecast.io only allows 1000 calls per day
  ticker.attach(60 * 9, setReadyForWeatherUpdate);
}

void loop() {
    if (readyForWeatherUpdate) {
      readyForWeatherUpdate = false;
      Serial.println(WiFi.localIP()); 
      weather.updateWeatherData(forecastApiKey, latitude, longitude);
  }
}

void setReadyForWeatherUpdate() {
  readyForWeatherUpdate = true;  
}

