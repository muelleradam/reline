/*
* Capacitive Sensor Code
* by Adam Müller, 2022
*
* Esp-now class. Handling the wifi communication.
*/

#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>

class espnow{

uint8_t addr[6];
esp_now_peer_info_t peerInfo;

public:
  bool connect(uint8_t *broadcastAddress);
  void send(char *message, uint8_t len);
};