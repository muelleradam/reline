/*
* IR Sensor Code
* by Adam Müller, 2022
*
* AMG class. Handling the AMG8833 sensor and sending the data.
*/

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

#include "config.h"
#include "en_class.h"

class amg{

espnow hespnow;
Adafruit_AMG88xx amg;
uint8_t broadcastAddress[6] = {addr_1, addr_2, addr_3, addr_4, addr_5, addr_6};
float out = 0;
char new_out[MESSAGE_LENGTH];
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

public:
  void init();
  void read();
};