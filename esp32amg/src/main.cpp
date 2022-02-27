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
    Serial.begin(115200);
    Serial.println(F("AMG88xx pixels"));

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


    delay(100); // let sensor boot up
}


void loop() { 
  //read all the pixels
  amg.readPixels(pixels);


//  char values[400] = {};
//  char values[500] = {};
  float out = 0;
  char new_out[10];
//  int x = 0;


  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
    out = pixels[i-1];
    sprintf(new_out, "%i:%.2f", i-1, out);
    Serial.println(new_out);

    esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) new_out, strlen(new_out));
    if (result == ESP_OK) {
      Serial.println("Sent with success");
    }
    else {
      Serial.println("Error sending the data");
    }

//    for(int j=0; j < strlen(new_out); j++)
//    {
//      values[x] = new_out[j];
//      x = x+1;
//    }

  }
//  Serial.println(values);

////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  char concat[600];
//  char preamble[] = "T";
//
////  float val = 20.33;
////
////  int n = sprintf(concat,"%s:%.2f%s", preamble, val, "\n");
//  int n = sprintf(concat,"%s:%s%s", preamble, values, "\n");
//
//  Serial.print(concat);
//  Serial.println(n);
//
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) concat, strlen(concat));
//   
//  if (result == ESP_OK) {
//    Serial.println("Sent with success");
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
//
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++



  delay(250);
}




//void loop() { 
//  //read all the pixels
//  amg.readPixels(pixels);
//
//
////  char values[400] = {};
//  char values[500] = {};
//  float out = 0;
//  char new_out[10];
//  int x = 0;
//
//
//  for(int i=1; i<=AMG88xx_PIXEL_ARRAY_SIZE; i++){
//    out = pixels[i-1];
//    sprintf(new_out, "%.1f", out);
//    for(int j=0; j < strlen(new_out); j++)
//    {
//      if(new_out[j] != '.')
//      {
//        values[x] = new_out[j];
//        x = x+1;
//      }
//    }
//  }
//  Serial.println(values);
//
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//  char concat[600];
//  char preamble[] = "T";
//
////  float val = 20.33;
////
////  int n = sprintf(concat,"%s:%.2f%s", preamble, val, "\n");
//  int n = sprintf(concat,"%s:%s%s", preamble, values, "\n");
//
//  Serial.print(concat);
//  Serial.println(n);
//
//  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) concat, strlen(concat));
//   
//  if (result == ESP_OK) {
//    Serial.println("Sent with success");
//  }
//  else {
//    Serial.println("Error sending the data");
//  }
//
////++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
//
//
//  delay(250);
//}