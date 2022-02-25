#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>


const unsigned int MAX_MESSAGE_LENGTH = 10;



// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x7B, 0xAD, 0x08};

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(9600);
  Serial.println("STARTUP");

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {

  
  static char message[MAX_MESSAGE_LENGTH];
  static char empty_message[MAX_MESSAGE_LENGTH] = {0};
  static unsigned int message_pos = 2;

  while(Serial.available() > 0)
  {
    char inputByte = Serial.read();

    if(inputByte == '\n')
    {
      message[0] = 'C';
      message[1] = ':';

      message[message_pos] = inputByte;
      message_pos++;

      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, message_pos);


      if (result == ESP_OK) {
        Serial.println("Sent with success");
      }
      else {
        Serial.println("Error sending the data");
      }


      memcpy(message, empty_message, MAX_MESSAGE_LENGTH);
      message_pos = 2;

      break;
    }
    else
    {
      message[message_pos] = inputByte;
      message_pos++;
    }
  }

  delay(10);
}