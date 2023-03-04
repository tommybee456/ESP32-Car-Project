#include <esp_now.h>
#include <WiFi.h>
#include "driver/ledc.h"

// Structure example to receive data
// Must match the sender structure
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

int pos = 75;
//const int IN1 = 5; //PWM
//const int IN2 = 7; //PWN
const int nsleep = 4;
int x_low = 0;
int x_high = 0;
int x_low_adj = 0;
int x_high_adj = 0;
int y = 0;
int PWM1 = 0;
int PWM2 = 0;
int servoPin = 9;
int ledpin = 13;
int count = 0;
boolean dr_mode = false;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
}

void setup()
{
  // Initialize Serial Monitor
  //  Serial.begin(115200);

  ledcSetup(LEDC_CHANNEL_7, 50, LEDC_TIMER_10_BIT);
  ledcAttachPin(9, LEDC_CHANNEL_7);
  ledcWrite(LEDC_CHANNEL_7, pos);

  ledcSetup(LEDC_CHANNEL_1, 1000, LEDC_TIMER_8_BIT);
  ledcAttachPin(5, LEDC_CHANNEL_1);
  ledcWrite(LEDC_CHANNEL_1, 0);

  ledcSetup(LEDC_CHANNEL_2, 1000, LEDC_TIMER_8_BIT);
  ledcAttachPin(7, LEDC_CHANNEL_2);
  ledcWrite(LEDC_CHANNEL_2, 0);

  ledcSetup(LEDC_CHANNEL_1, 1000, LEDC_TIMER_8_BIT);
  ledcAttachPin(8, LEDC_CHANNEL_1);
  ledcWrite(LEDC_CHANNEL_1, 0);

  ledcSetup(LEDC_CHANNEL_2, 1000, LEDC_TIMER_8_BIT);
  ledcAttachPin(6, LEDC_CHANNEL_2);
  ledcWrite(LEDC_CHANNEL_2, 0);

  pinMode(nsleep, OUTPUT);
  digitalWrite(nsleep, HIGH);
  //  pinMode(IN1, OUTPUT);
  //  pinMode(IN2, OUTPUT);
  pinMode(ledpin, OUTPUT);

  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    //    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);
  digitalWrite(ledpin, LOW);
}

void loop()
{
  if (myData.f == 0 && myData.e == 1)
  {
    dr_mode = !dr_mode;
    delay(250);
  }
  x_high = map(myData.j, 155, 255, 0, 190);
  x_high = constrain(x_high, 0, 190);
  x_high_adj = map(x_high, 0, 190, 30, 190);
  x_low = map(myData.j, 0, 128, 0, 190);
  x_low = constrain(x_low, 0, 190);
  x_low = abs(190 - x_low);
  x_low_adj = map(x_low, 0, 190, 30, 190);
  y = constrain(myData.g, 81, 255);
  if (dr_mode == false && myData.e == 1)
  {
    y = 150 - map(y, 81, 255, 62, 92);
  }
  else
  {
    y = 150 - map(y, 81, 255, 70, 84);
  }
  if (myData.a == 0 && myData.e == 1)
  {
    PWM1 = count;
    PWM2 = 0;
    if (count < 254)
    {
      count++;
      delay(1);
    }
  }
  else if (myData.b == 0 && myData.e == 1)
  {
    PWM1 = count;
    PWM2 = 0;
    if (count < 190)
    {
      count++;
      delay(1);
    }
  }
  else if (myData.c == 0 && myData.e == 1)
  {
    PWM1 = 0;
    PWM2 = count;
    if (count < 254)
    {
      count++;
      delay(1);
    }
  }
  else if (myData.d == 0 && myData.e == 1)
  {
    PWM1 = 0;
    PWM2 = count;
    if (count < 190)
    {
      count++;
      delay(1);
    }
  }
  else if (x_high > 15 && myData.e == 1)
  {
    PWM1 = 0;
    PWM2 = count;
    if (count < x_high_adj)
    {
      count++;
      delay(1);
    }
    if (count > x_high_adj)
    {
      count--;
      delay(1);
    }
  }
  else if (x_low > 15 && myData.e == 1)
  {
    PWM1 = count;
    PWM2 = 0;
    if (count < x_low_adj)
    {
      count++;
      delay(1);
    }
    if (count > x_low_adj)
    {
      count--;
      delay(1);
    }
  }
  else
  {
    PWM1 = 0;
    PWM2 = 0;
    count = 0;
  }
  pos = y;
  ledcWrite(LEDC_CHANNEL_7, pos);
  ledcWrite(LEDC_CHANNEL_1, PWM2);
  ledcWrite(LEDC_CHANNEL_2, PWM1);

  //  Serial.print(myData.a);
  //  Serial.print(" ");
  //  Serial.print(myData.b);
  //  Serial.print(" ");
  //  Serial.print(myData.c);
  //  Serial.print(" ");
  //  Serial.print(myData.d);
  //  Serial.print(" ");
  //  Serial.print(myData.e);
  //  Serial.print(" ");
  //  Serial.print(myData.f);
  //  Serial.print(" ");
  //  Serial.print(myData.g);
  //  Serial.print(" ");
  //  Serial.print(myData.h);
  //  Serial.print(" ");
  //  Serial.print(pos);
  //  //    Serial.println(" ");
  //  Serial.print(" ");
  //  Serial.print(x_high);
  //  Serial.print(" ");
  //  Serial.print(x_low);
  //  Serial.print(" ");
  //  Serial.println(dr_mode);
  //    Serial.print(" ");
  //    Serial.println(y);
  //    Serial.print(" ");
  //    Serial.print(motor1);
  //    Serial.print(" ");
  //    Serial.print(PWM1);
  //    Serial.print(" ");
  //    Serial.print(motor2);
  //    Serial.print(" ");
  //    Serial.println(PWM2);

}
