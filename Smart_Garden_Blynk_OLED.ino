#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "**************"
#define BLYNK_DEVICE_NAME "************"
#define BLYNK_AUTH_TOKEN "*********************"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH1106.h>

char auth[] = "****************************";

char ssid[] = "*********************";
char pass[] = "****************";

#define pump 26
#define moisture 36
float water;

#define DHTPIN 14         
#define DHTTYPE DHT11     
DHT dht(DHTPIN, DHTTYPE);

#define OLED_SDA 21
#define OLED_SCL 22
Adafruit_SH1106 display(21, 22);

BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  
  Blynk.virtualWrite(V1, h); 
  Blynk.virtualWrite(V2, t); 
  Blynk.virtualWrite(V0, water);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  display.print("Temp:");
  display.println(t);
  display.setCursor(0, 25);
  display.print("Humi:");
  display.println(h);
  display.display();
}

BLYNK_WRITE(V3){
  int value = param.asInt();
  digitalWrite(pump, value);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  Blynk.begin(auth, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  pinMode(moisture, INPUT);
  pinMode(pump, OUTPUT);

  //OLED Code Initiation
  display.begin(SH1106_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(35, 10);
  display.print("Smart");
  display.setCursor(30, 40);
  display.print("Garden");
  display.display();
  delay(2000);
  display.clearDisplay();
}

void loop()
{
  Blynk.run();
  timer.run();
  water = analogRead(moisture);
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 40);
  display.print("Water: ");
  display.println(water);
  display.display();
  delay(2000);
  display.clearDisplay();
}
