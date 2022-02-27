/*
* IR Sensor Code
* by Adam Müller, 2022
*
* Esp-now class. Handling the wifi communication.
*/

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

#include "en_class.h"

bool espnow::connect(uint8_t *broadcastAddress)
{
  memcpy(addr, broadcastAddress, 6);

  // Set device as wifi station
  WiFi.mode(WIFI_STA);

  // Init esp-now
  if (esp_now_init() != ESP_OK) return false;

  // Register peer
  memcpy(peerInfo.peer_addr, addr, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK) return false;

  return true;
}

// send data over wifi
void espnow::send(char *message, uint8_t len)
{
  esp_err_t result = esp_now_send(addr, (uint8_t *) message, len);
}