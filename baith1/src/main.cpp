#include <Arduino.h>
#include <DHT.h>
#include <DHT_U.h> 
#include <ThingSpeak.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>

#define DHTTYPE DHT11
#define DHTPIN 4
DHT_Unified dht(DHTPIN,DHTTYPE);

WiFiClient wificlient;
const char* ssid = "TP-Link_4FA64FA6";
const char* password = "65371010379536537101037953";

unsigned long myChannelNumber = 1;
const char* myWriteAPIKey = "YQWYXLBHEISNJ72E";

void setup() {
 Serial.begin(115200);
 WiFi.mode(WIFI_STA);
 WiFi.begin(ssid, password);
 Serial.println("\nConnecting to Wifi netword ..");
 while (WiFi.status() != WL_CONNECTED)
 {
  Serial.print(".");
  delay(100);
 }
 Serial.println("\nConnected to Wifi Network");
 Serial.println("Local ESP32 IP: ");
 Serial.println(WiFi.localIP());
 dht.begin();
 ThingSpeak.begin(wificlient);
}

void loop() {
 sensors_event_t event;
 dht.temperature().getEvent(&event);
 if (isnan(event.temperature))
 {
  Serial.println(F("Error reading temperature"));
 } else
 {
  Serial.print(F("temperature: "));
  Serial.print(event.temperature);
  Serial.println(F("C"));
  int x = ThingSpeak.writeField(myChannelNumber,2,event.temperature,myWriteAPIKey);
  if (x == 200)
  {
    Serial.println("Channel update successfull");
  }else
  {
    Serial.println("Problem updatin channel. HTTP error code " + String(x));

  }
 }
 delay(1000);
 dht.humidity().getEvent(&event);
 if (isnan(event.relative_humidity))
 {
  Serial.println(F("humidity: "));
 }else
 {
  Serial.print(F("humidity: "));
  Serial.print(event.relative_humidity);
  Serial.println(F("%"));
  int y = ThingSpeak.writeField(myChannelNumber,1,event.relative_humidity,myWriteAPIKey);
  if (y == 200)
  {
    Serial.println("Channel update successfull");
  }else
  {
    Serial.println("Problem updatin channel. HTTP error code " + String(y));
  }
  
 }
delay(1000);
 }

