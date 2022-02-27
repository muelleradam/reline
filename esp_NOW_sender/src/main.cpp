#include <Arduino.h>

#include <Wire.h>
#include <Adafruit_AMG88xx.h>
#include <esp_now.h>
#include <WiFi.h>

Adafruit_AMG88xx amg;

float pixels[AMG88xx_PIXEL_ARRAY_SIZE];

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



  bool status;
    
  // default settings
//    status = amg.begin(104);
  status = amg.begin(105);
  if (!status) {
      Serial.println("Could not find a valid AMG88xx sensor, check wiring!");
      while (1);
  }
  
  Serial.println("-- Pixels Test --");
  Serial.println();

//    pinMode(13, OUTPUT);

  delay(100); // let sensor boot up
}
 
void loop() {
  //read all the pixels
  amg.readPixels(pixels);

  char values[400];
  float out = 0;

  Serial.print("[");
  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
    out = pixels[i-1];
//    Serial.print(pixels[i-1]);
    Serial.print(out);
    Serial.print(", ");
    sprintf(values, "%s%.2f", values, out);
  }
  Serial.println("]");
  Serial.println();

//  delay(250);



  char concat[400];
  char preamble[] = "T";

//  float val = 20.33;
//
//  int n = sprintf(concat,"%s:%.2f%s", preamble, val, "\n");
  int n = sprintf(concat,"%s:%s%s", preamble, values, "\n");

  Serial.print(concat);
  Serial.println(n);

  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &concat, sizeof(concat));
   
  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
//  delay(1000);
  delay(250);
}