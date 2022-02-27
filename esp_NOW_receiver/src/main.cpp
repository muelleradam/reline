/*
* Networkmaster Code
* by Adam Müller, 2022
*
* Read data received over wifi and print it to serial for further evaluation in graphic tools such as matlab.
*
* MIT License
* 
* Copyright (c) 2022 Adam M.
* 
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
* 
* The above copyright notice and this permission notice shall be included in all
* copies or substantial portions of the Software.
* 
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
*/

#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {

  Serial.println(char(incomingData[0]));

  for(int i = 2; i < len; i++)
  {
    Serial.print(char(incomingData[i]));
  }
}
 
void setup() {
  Serial.begin(115200);
  
  // device as wifi station
  WiFi.mode(WIFI_STA);

  // init esp-now
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // register callback
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  delay(10000);  
}