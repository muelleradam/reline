#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

#include "uart_class.h"
#include "en_class.h"
#include "config.h"

uart huart;

espnow hespnow;

// MAC Address Receiver
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x7B, 0xAD, 0x08};

void setup() {
  huart.init();
  hespnow.connect(broadcastAddress);
}
 
void loop() {
  
  if(huart.update())
  {
    char message[MAX_MESSAGE_LENGTH];
    uint8_t len = huart.getMsg(message);

    hespnow.send(message, len);
  }
}