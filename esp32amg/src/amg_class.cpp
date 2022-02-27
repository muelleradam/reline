#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_AMG88xx.h>

#include "amg_class.h"



void amg::init()
{
  amg.begin(105);    // if 105 doesnt work you might need 104 (if solderjumper set)
  hespnow.connect(broadcastAddress);
//  resetMsgBuff();
}


void amg::read()
{
  //read all the pixels
  amg.readPixels(pixels);




  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
    out = pixels[i-1];
    sprintf(new_out, "%i:%.2f", i-1, out);

    hespnow.send(new_out, strlen(new_out));
////    Serial.println(new_out);
//
//    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) new_out, strlen(new_out));
////    if (result == ESP_OK) {
////      Serial.println("Sent with success");
////    }
////    else {
////      Serial.println("Error sending the data");
////    }

  }
}


void amg::resetMsgBuff()
{
  memcpy(new_out, empty_out, MESSAGE_LENGTH);
  out = 0;
}

// Getter function
uint8_t amg::getMsg(char *buf)
{
  uint8_t len = strlen(new_out);
  memcpy(buf, new_out, len);
  resetMsgBuff();
  return len;
}