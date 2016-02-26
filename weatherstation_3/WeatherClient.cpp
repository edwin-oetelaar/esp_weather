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

#include "WeatherClient.h"
#include <ESP8266WiFi.h>

void WeatherClient::updateWeatherData(String apiKey, double lat, double lon) {
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect("eventcaster.nl", httpPort)) {  
    Serial.println("connection failed");
    return;
  }
  
  // We now create a URI for the request
String url = "/api/weather/v1.py?apikey="+ apiKey + "&lat=" + String(lat) + "&lon=" + String(lon)+"&lang=en";

  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.0\r\n" +
               "Host: eventcaster.nl\r\n" + 
               "Connection: close\r\n\r\n");
  while(!client.available()) {
    
    delay(200); 
  }
  
  // Read all the lines of the reply from server and print them to Serial
  
  delay(500);
  while(client.available()){

  String line = client.readStringUntil('\n');
  Serial.println(line);
  }
  
  Serial.println();
  Serial.println("closing connection"); 
}

