/*
* Capacitive Sensor Code
* by Adam Müller, 2022
*
* Read data over serial which is being sent by the atmega.
* Then process that data and send it over esp-now to the receiver.
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

#include "uart_class.h"
#include "en_class.h"
#include "config.h"

// class for serial communication
uart huart;
// class for wifi communication
espnow hespnow;

// MAC Address Receiver
uint8_t broadcastAddress[] = {addr_1, addr_2, addr_3, addr_4, addr_5, addr_6};

void setup() {
  // initialize serial
  huart.init();
  // initialize wifi
  hespnow.connect(broadcastAddress);
}
 
void loop() {
  // when data is received from the atmega
  if(huart.update())
  {
    char message[MAX_MESSAGE_LENGTH];
    uint8_t len = huart.getMsg(message);

    // send over wifi
    hespnow.send(message, len);
  }
}