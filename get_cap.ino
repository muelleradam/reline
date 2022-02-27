/*
* Capacitive Sensor Code
* by Adam Müller, 2022
*
* Read capacitive sensor, prefilter by averaging over 50 values, and send data over serial.
* 
* This file is to be compiled in the arduino-ide. The compiled file can then be
* uploaded to the board via a seperate uploader and cmd.
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

// pins of cap-planes --> pcb planes preset, change in order to use external planes
const int OUT_PIN = A2;
const int IN_PIN = A0;

// value of the input cap determined by testing with known cap
const float IN_CAP_TO_GND = 24.48;
const int MAX_ADC_VALUE = 1023;


void setup(){
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, OUTPUT);

  // same baud as on the esp to communicate
  Serial.begin(9600);
}


void loop(){
  int counter = 0;
  float cum_cap = 0.0f;
  
  // read voltage
  pinMode(IN_PIN, INPUT);
  digitalWrite(OUT_PIN, HIGH);
  int val = analogRead(IN_PIN);
  digitalWrite(OUT_PIN, LOW);

  // only use nominal values
  if (val < 1000){
    // mean of 50 values
    if(counter <= 50)
    {
      pinMode(IN_PIN, OUTPUT);
      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);

      cum_cap += capacitance;
    }
    else
    {
      pinMode(IN_PIN, OUTPUT);
      float capacitance = (float)val * IN_CAP_TO_GND / (float)(MAX_ADC_VALUE - val);
      cum_cap += capacitance;

      float cap_mid = cum_cap / counter;

      // write over serial to esp
      Serial.println(cap_mid, 3);

      // reset variables
      counter = 0;
      cum_cap = 0;
    }
    counter += 1;
  }
    
  // use for debug
/*
  else
  {
    Serial.println("implausible values read");
  }
*/

  delay(5);
}
