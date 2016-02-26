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
//char ssid[] = "AirPort Extreme";
char ssid[] = "your_ssid";
// your network password
//char pass[] = "";  
char pass[] = "your_passwd";

// Go to forecast.io and register for an API KEY
// not REAL keys in this file
//String forecastApiKey = "fe2635fb779ba8f689dad525"; // key Ben
//String forecastApiKey = "8cfdfa91d278b19c4abb38f3";  // key Theo
//String forecastApiKey = "1a666f82a9af133f31645bee"; //Key Edwin
//String forecastApiKey = "4dff7b6ecd5d8232ef32d195"; //Key Wim Westerburgen
String forecastApiKey = "cb6123e49d96db89fc171d21"; //Mark Alberts

// Coordinates of the place you want weather information for

//Peer Gruijters - Beek en Donk - Kanaaldijk
//double latitude = 51.5187092;
//double longitude = 5.6392702;

//Ben Zijlstra - Tilburg - Wolvegastraat
//double latitude = 51.5700772;
//double longitude = 5.0007599;

//Wim Westerburgen - Simon v.d. Steltstraat
//double latitude = 51.5576;
//double longitude = 5.10543;

//Wim Westerburgen - W.S.V. - Herkingen
//double latitude2 = 51.706559;
//double longitude2 = 4.0839951;

//Edwin van den Oetelaar - Best
//double latitude = 51.5149004;
//double longitude = 5.3975379;

//Edwin van den Oetelaar - Helmond - deze krijg ik nog door van Edwin
//double latitude2 = 51.5;
//double longitude2 = 5.0;

//Mark Alberts - Amsterdam - De Dam
double latitude2 = 52.3725651;
double longitude2 = 4.88882673;

//Mark Alberts - Almere-Haven
double latitude = 52.3465909;
double longitude = 5.2164959;

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
      Serial.println("LOCATION=Amsterdam\n"); 
      weather.updateWeatherData(forecastApiKey,latitude2,longitude2);
      delay(60000);
      Serial.println("LOCATION=Almere-Haven\n");
      weather.updateWeatherData(forecastApiKey,latitude,longitude);
  }
}

void setReadyForWeatherUpdate() {
  readyForWeatherUpdate = true;  
}


