/*
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-esp32-arduino-ide/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <esp_now.h>
#include <WiFi.h>

// REPLACE WITH YOUR RECEIVER MAC Address
uint8_t broadcastAddress[] = {0x60, 0x55, 0xF9, 0xD9, 0x8B, 0x9A};

// Structure example to send data
// Must match the receiver structure
typedef struct struct_message {
  int a;
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
  int h;
  int i;
  int j;  
} struct_message;

const int Button1 = 10;
const int Button2 = 11;
const int Button3 = 12;
const int Button4 = 14;
const int Sel1 = 6;
const int Sel2 = 9;
int ledpin = 13;
int x1 = 0;
uint8_t x1_send = 0;
int y_1 = 0;
uint8_t y1_send = 0;
int x2 = 0;
uint8_t x2_send = 0;
int y2 = 0;
uint8_t y2_send = 0;
// Create a struct_message called myData
struct_message myData;

esp_now_peer_info_t peerInfo;

// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) {
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void setup() {
  pinMode(Button1, INPUT_PULLUP);
  pinMode(Button2, INPUT_PULLUP);
  pinMode(Button3, INPUT_PULLUP);
  pinMode(Button4, INPUT_PULLUP);
  pinMode(Sel1, INPUT_PULLUP);
  pinMode(Sel2, INPUT_PULLUP);
  pinMode(ledpin, OUTPUT);
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
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Failed to add peer");
    return;
  }
  digitalWrite(ledpin, LOW);
}

void loop() {
  // Set values to send
  myData.a = digitalRead(Button1);
  myData.b = digitalRead(Button2);
  myData.c = digitalRead(Button3);
  myData.d = digitalRead(Button4);
  myData.e = digitalRead(Sel1);
  myData.f = digitalRead(Sel2);
  x1 = analogRead(7);
  x1_send = map(x1, 0, 8191, 0, 255);
  x1_send = constrain(x1_send, 0, 255);
  y_1 = analogRead(8);
  y1_send = map(y_1, 0, 8191, 0, 255);
  y1_send = constrain(y1_send, 0, 255);
  myData.g = x1_send;
  myData.h = y1_send;
  x2 = analogRead(5);
  x2_send = map(x2, 0, 8191, 0, 255);
  x2_send = constrain(x2_send, 0, 255);
  y2 = analogRead(4);
  y2_send = map(y2, 0, 8191, 0, 255);
  y2_send = constrain(y2_send, 0, 255);
  myData.i = x2_send;
  myData.j = y2_send;
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));

  if (result == ESP_OK) {
    Serial.println("Sent with success");
  }
  else {
    Serial.println("Error sending the data");
  }
  delay(10);
}
