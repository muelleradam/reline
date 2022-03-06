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

// global values only to gather all of the data in one spot
// in future versions this may be redone in a way that the data can be worked with
// for now the data only gets visualised so there is no need for complex code in this stage
float arr[64] = {0};
float cap_val = 0;
int count = 0;

// callback for received data
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {

  // differentiate between the capacitive value and the temperature data
  // capacitive value has a leading 'C'
  if(char(incomingData[0]) == 'C')
  {
    // temporary variables used
    char temp_arr[len-2] = {};
    char empty_temp_arr[len-2] = {};

    // get every sent character and convert to a floating value
    for(int i = 2; i < len; i++)
    {
      temp_arr[i-2] = char(incomingData[i]);
    }

    cap_val = atof(temp_arr);
    // clear temporary variables
    memcpy(temp_arr, empty_temp_arr, len-2);
  }
  // part for reading the temperature data
  else
  {
    // count for visualisation, comment out when not in use
    count  = count + 1;

    // temporary variables used
    int temp_val_1 = 0;
    float temp_val_2 = 0;
    int temp_cnt = 3;
    char temp_arr_1[3] = {};
    char empty_temp_arr_1[3] = {};
    char temp_arr_2[5] = {};
    char empty_temp_arr_2[5] = {};

    // read temperature matrix and convert to array of float values
    for(int i = 0; i < len; i++)
    {
      // the data is formated as such: [Nr. of the pixel in the matrix : value for according pixel]
      if(char(incomingData[i]) == ':')
      {
        temp_val_1 = atoi(temp_arr_1);

        temp_cnt = i;

        memcpy(temp_arr_1, empty_temp_arr_1, 3);
      }
      // split the data accordingly and format into the correct space in the array 
      if(i > temp_cnt)
      {
        temp_arr_2[i-temp_cnt-1] = char(incomingData[i]);
        if(i == len-1)
        {
          temp_val_2 = atof(temp_arr_2);
          arr[temp_val_1] = temp_val_2;

          memcpy(temp_arr_2, empty_temp_arr_2, 5);
          temp_val_1 = 0;
          temp_val_2 = 0;
          temp_cnt = 3;
        }
      }
      else
      {
        temp_arr_1[i] = char(incomingData[i]);
      }    
    }
  }

//---------- mainly used for visualisation in matlab, comment out when not in use ----------
  if(count == 64)
  {
    Serial.print(cap_val, 3);
    Serial.print(", ");
    for(int j=0; j<64; j++)
    {
      Serial.print(arr[j]);
      Serial.print(", ");
    }
    Serial.println();
    count = 0;
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
}