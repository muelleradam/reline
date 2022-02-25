#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>


//const unsigned int MAX_MESSAGE_LENGTH = 10;


// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t broadcastAddress[] = {0x24, 0x6F, 0x28, 0x7B, 0xAD, 0x08};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
    int id; // must be unique for each sender board
    int x;
    int y;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// Create peer interface
esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  // Init Serial Monitor
  Serial.begin(115200);

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
  // Set values to send
//  myData.id = 1;
//  myData.x = random(0,50);
//  myData.y = random(0,50);
//  char message[10] = "test";
  char concat[8];
  char preamble[] = "T";

  float val = 20.33;
//  int val = 20;

//  dtostrf(val,5,2,message);
//
//  message[6] = " ";
//  message[7] = "T";

  int n = sprintf(concat,"%s:%.2f%s", preamble, val, "\n");
//  sprintf(concat,"%s:%s", concat, '\n');

//  Serial.println(concat);
  Serial.print(concat);
  Serial.println(n);

//  static char message[MAX_MESSAGE_LENGTH];
//  static unsigned int message_pos = 0;
//  float rnd = 20.336;
//  for(int i=0; i<=sizeof(rnd); i++)
//    message[i] = rnd;
//  Serial.println(message);

  // Send message via ESP-NOW
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, sizeof(message));
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &concat, sizeof(concat));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(1000);
}