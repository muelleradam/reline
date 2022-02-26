/*
* Capacitive Sensor Code
* by Adam Müller, 2022
*
* Uart class. Handling the serial communication.
*/

#include <Arduino.h>

#include "uart_class.h"

// Init communication and reset buffer
void uart::init()
{
  Serial.begin(9600);
  resetMsgBuff();
}

// Handle serial that is being received
bool uart::update()
{

  if(Serial.available() > 0)
  {
    char inByte = Serial.read();

    // check if message is complete
    if(inByte == '\n')
    {
      message[0] = 'C';
      message[1] = ':';
  
      message[message_pos] = inByte;
      message_pos++;
  
      return true;
    }
    else
    {
      message[message_pos] = inByte;
      message_pos++;
    }
  }

  return false;
}

// Reset used variables
void uart::resetMsgBuff()
{
  memcpy(message, empty_message, MAX_MESSAGE_LENGTH);
  message_pos = 2;
}

// Getter function
uint8_t uart::getMsg(char *buf)
{
  uint8_t len = message_pos;
  memcpy(buf, message, len);
  resetMsgBuff();
  return len;
}