#include <Arduino.h>

#include "uart_class.h"

void uart::init()
{
  Serial.begin(9600);
  resetMsgBuff();
}

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

void uart::resetMsgBuff()
{
  memcpy(message, empty_message, MAX_MESSAGE_LENGTH);
  message_pos = 2;
}

uint8_t uart::getMsg(char *buf)
{
  uint8_t len = message_pos;
  memcpy(buf, message, len);
  resetMsgBuff();
  return len;
}

//String uart::request_temperatures()
//{
//  static unsigned int message_pos = 2;
//
//  while(Serial.available() > 0){
//
//    char inputByte = Serial.read();
//  
//    if(inputByte == '\n')
//    {
//      message[0] = 'C';
//      message[1] = ':';
//  
//      message[message_pos] = inputByte;
//      message_pos++;
//  
////      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, message_pos);
//      return message, message_pos;
//  
//  
//      memcpy(message, empty_message, MAX_MESSAGE_LENGTH);
//      message_pos = 2;
//  
//      break;
//    }
//    else
//    {
//      message[message_pos] = inputByte;
//      message_pos++;
//    }
//
//  }
//
//
//  return false;
//}