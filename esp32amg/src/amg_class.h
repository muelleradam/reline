#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

#include "config.h"
#include "en_class.h"

class amg{

espnow hespnow;
Adafruit_AMG88xx amg;
uint8_t broadcastAddress[6] = {addr_1, addr_2, addr_3, addr_4, addr_5, addr_6};
float pixels[AMG88xx_PIXEL_ARRAY_SIZE];
float out = 0;
char new_out[MESSAGE_LENGTH];
char empty_out[MESSAGE_LENGTH] = {0};

public:
  void init();
  void read();
	void resetMsgBuff();
  uint8_t getMsg(char *buf);
};