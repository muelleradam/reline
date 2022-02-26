#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {

  Serial.println(char(incomingData[0]));

  for(int i = 2; i < len; i++)
  {
    Serial.print(char(incomingData[i]));
  }
}
 
void setup() {
  //Initialize Serial Monitor
  Serial.begin(115200);
  
  //Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  //Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
}
 
void loop() {
  delay(10000);  
}