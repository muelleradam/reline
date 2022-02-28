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

float arr[64] = {0};

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {

//  Serial.println(char(incomingData[0]));
//  float arr[64] = {0};
//  for(int i=0; i<64; i++) Serial.print(arr[i]);

  if(char(incomingData[0]) == 'C')
  {
    Serial.println(char(incomingData[0]));
    for(int i = 2; i < len; i++)
    {
      Serial.print(char(incomingData[i]));
    }
  }
  else
  {
    int temp_val_1 = 0;
    float temp_val_2 = 0;
    int temp_cnt = 3;
    char temp_arr_1[3] = {};
    char empty_temp_arr_1[3] = {};
    char temp_arr_2[5] = {};
    char empty_temp_arr_2[5] = {};

    for(int i = 0; i < len; i++)
    {
      Serial.print(char(incomingData[i]));
      if(char(incomingData[i]) == ':')
      {
//        Serial.println(i);
//        Serial.println(temp_arr_1);
        temp_val_1 = atoi(temp_arr_1);
//        Serial.println(temp_val_1);

        temp_cnt = i;
//        temp_cnt = strlen(temp_arr_1);

        memcpy(temp_arr_1, empty_temp_arr_1, 3);
      }
      if(i > temp_cnt)
      {
        temp_arr_2[i-temp_cnt-1] = char(incomingData[i]);
        if(i == len-1)
        {
          Serial.println("LEL");
          temp_val_2 = atoi(temp_arr_2);
          Serial.println(temp_val_1);
          Serial.println(temp_val_2);
          Serial.println(temp_arr_2);
          arr[temp_val_1] = temp_val_2;

//          Serial.println(arr);
          for(int j=0; j<64; j++) Serial.print(arr[j]);

          memcpy(temp_arr_2, empty_temp_arr_2, 5);
          temp_val_1 = 0;
          temp_val_2 = 0;
          temp_cnt = 3;
        }
      } else {
        temp_arr_1[i] = char(incomingData[i]);
      }
    
    }

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