/*
* Capacitive Sensor Code
* by Adam Müller, 2022
*
* Uart class. Handling the serial communication.
*/

#include <Arduino.h>

#include "config.h"

class uart{

char message[MAX_MESSAGE_LENGTH];
char empty_message[MAX_MESSAGE_LENGTH] = {0};
unsigned int message_pos = 2;

public:
  void init();
  bool update();
  void resetMsgBuff();
  uint8_t getMsg(char *buf);
};