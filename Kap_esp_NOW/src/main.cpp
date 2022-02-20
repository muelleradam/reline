#include <Arduino.h>

#include <esp_now.h>
#include <WiFi.h>


const unsigned int MAX_MESSAGE_LENGTH = 10;



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
//  Serial.begin(115200);
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
  static unsigned int message_pos = 0;

  while(Serial.available() > 0)
  {
    char inputByte = Serial.read();

    if(inputByte == '\n')
    {
      //Serial.println(message);

      message[message_pos] = inputByte;
      message_pos++;

      message[message_pos] = ' ';
      message_pos++;
      message[message_pos] = 'C';
      message_pos++;

      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &message, message_pos);


      

//      float number = atof(message);
//      Serial.println(number);
//
//      // Send message via ESP-NOW
//      esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &number, sizeof(number));
       
      if (result == ESP_OK) {
        Serial.println("Sent with success");
      }
      else {
        Serial.println("Error sending the data");
      }



//      message_pos = 0;
//      message[message_pos] = 'C';
//      message_pos++;
//      message[message_pos] = ' ';
//      message_pos++;

      break;
    }
    else
    {
      message[message_pos] = inputByte;
      message_pos++;
    }
  }



//  // Set values to send
//  myData.id = 1;
//  myData.x = random(0,50);
//  myData.y = random(0,50);
//
//  // Send message via ESP-NOW
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));
//   
//  if (result == ESP_OK) {
//    Serial.println("Sent with success");
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
//  delay(1000);

  delay(10);
}