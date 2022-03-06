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
char matlab_arr[453] = {0};    // 64*7 + 7
char empty_matlab_arr[453] = {0};    // 64*7 + 7

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {

//  Serial.println(char(incomingData[0]));
//  float arr[64] = {0};
//  for(int i=0; i<64; i++) Serial.print(arr[i]);

  matlab_arr[5] = ',';
  if(char(incomingData[0]) == 'C')
  {
    char temp_arr[len-2] = {};
    char empty_temp_arr[len-2] = {};
    for(int i = 2; i < len; i++)
    {
      temp_arr[i-2] = char(incomingData[i]);
      matlab_arr[i-2] = char(incomingData[i]);
    }
//    matlab_arr[0] = atoi(temp_arr);
//    for(int j=0; j<len-2; j++) Serial.print(temp_arr[j]);
    memcpy(temp_arr, empty_temp_arr, len-2);
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
//      Serial.print(char(incomingData[i]));
//      Serial.println();
      if(char(incomingData[i]) == ':')
      {
        temp_val_1 = atoi(temp_arr_1);

        temp_cnt = i;

        memcpy(temp_arr_1, empty_temp_arr_1, 3);
      }
      if(i > temp_cnt)
      {
//        matlab_arr[strlen(matlab_arr+1)] = char(incomingData[i]);
//        matlab_arr[i-temp_cnt-1+5] = char(incomingData[i]);
        temp_arr_2[i-temp_cnt-1] = char(incomingData[i]);
        if(i == len-1)
        {
          temp_val_2 = atof(temp_arr_2);
          arr[temp_val_1] = temp_val_2;

//          for(int j=0; j<64; j++) Serial.print(arr[j]);               // irgendwo hier werden die Nachkommastellen zu 0 ????????????????
//          Serial.println();
          char test_temp[10] = {};
          char empty_test_temp[10] = {};
          for(int k=0; k<64; k++)
          {
            if(arr[k] <= 0.001)
            {
              sprintf(test_temp, "%.2f", 10.00);
            }
            else
            {
              sprintf(test_temp, "%.2f", arr[k]);
            }
//            sprintf(test_temp, "%.2f", arr[k]);
//            Serial.println(test_temp);
            for(int g=0; g<strlen(test_temp); g++)
            {
              matlab_arr[(k*(strlen(test_temp)+1))+6+g] = test_temp[g];
//              matlab_arr[(k*strlen(test_temp))+7+g] = test_temp[g];
              //matlab_arr[k+6+g] = char(k+6+g);
//              Serial.print((k*strlen(test_temp))+6+g);
//              Serial.println(test_temp);
            }
//            matlab_arr[(k*strlen(test_temp))+7+strlen(test_temp)] = ',';
            matlab_arr[(k*(strlen(test_temp)+1))+6+strlen(test_temp)] = ',';
            memcpy(test_temp, empty_test_temp, 10);
          }

          memcpy(temp_arr_2, empty_temp_arr_2, 5);
          temp_val_1 = 0;
          temp_val_2 = 0;
          temp_cnt = 3;
        }
//        for(int k=1; k<65; k++) matlab_arr[k] = arr[k-1];
      }
      else
      {
        temp_arr_1[i] = char(incomingData[i]);
      }
    
    }

  }

//  Serial.println("MATLAB ARR:");
//  Serial.println(matlab_arr);
//  Serial.println("-----------");
//  memcpy(matlab_arr, empty_matlab_arr, strlen(matlab_arr));

  for(int h=0; h<strlen(matlab_arr); h++) Serial.print(matlab_arr[h]);
  Serial.println();

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