#include <Arduino.h>
#include <math.h>
#include <WiFi.h>
#include <esp_now.h>
#include <SPI.h>
#include <Wire.h>
#include <Servo.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET -1
#define SCREEN_WIDTH 128 // pixel ความกว้าง
#define SCREEN_HEIGHT 64 // pixel ความสูง

Adafruit_SSD1306 OLED(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

typedef struct struct_player
{
  int hit;
  int hp;
} struct_player;

struct_player myData;
int hit = 0;
int hp = 30;
int pos = 0;
Servo arm;

void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len)
{
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Hit!");
  Serial.print(myData.hit);
  hit = myData.hit;
  hp = myData.hp;
}

void setup()
{

  // setup
  Serial.begin(115200);
  WiFi.mode(WIFI_MODE_STA);
  if (esp_now_init() != ESP_OK)
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  esp_now_register_recv_cb(OnDataRecv);
  if (!OLED.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // สั่งให้จอ OLED เริ่มทำงานที่ Address 0x3C
    Serial.println("SSD1306 allocation failed");
  }
  else
  {
    Serial.println("ArdinoAll OLED Start Work !!!");
  }

  // init
  arm.attach(13);



  Serial.println(WiFi.macAddress());
  Serial.print("Start");
}

void loop()
{
  if (hit == 1)
  {
    for (pos = 0; pos <= 180; pos += 1)
    { // goes from 0 degrees to 180 degrees
      // in steps of 1 degree
      arm.write(pos); // tell servo to go to position in variable 'pos'
      delay(1);       // waits 15ms for the servo to reach the position
    }
    for (pos = 180; pos >= 0; pos -= 1)
    {                 // goes from 180 degrees to 0 degrees
      arm.write(pos); // tell servo to go to position in variable 'pos'
      delay(1);       // waits 15ms for the servo to reach the position
    }
    hit = 0;
  }

  if(hp>=10){
    //OLED
    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setCursor(15, 0);
    OLED.setTextSize(9);
    OLED.print(hp);
    OLED.display();
  }
  else{
    OLED.clearDisplay();
    OLED.setTextColor(WHITE, BLACK);
    OLED.setCursor(40, 0);
    OLED.setTextSize(9);
    OLED.print(hp);
    OLED.display();
  }

}