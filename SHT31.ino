
#include <Arduino.h>
#include <Wire.h>
#include "Secret.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include "ThingSpeak.h"
#include "Adafruit_SHT31.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "time.nist.gov", 36000);
Adafruit_SHT31 sht31 = Adafruit_SHT31();
ESP8266WiFiMulti WiFiMulti;
WiFiClient  client;

// sleep for this many seconds
const int sleepMinutes = 10*60;

void setup() {
  Serial.begin(9600);
  timeClient.begin();
  timeClient.update();
  
  Serial.println("\n\nWake up");
  Serial.println(timeClient.getFormattedTime());

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println(WiFi.localIP());

  ThingSpeak.begin(client);

  // Connect D0 to RST to wake up
  pinMode(D0, WAKEUP_PULLUP);

  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  float temp = sht31.readTemperature();
  float humi = sht31.readHumidity();

  if (! isnan(temp)) {  // check if 'is not a number'
    Serial.print("Temp *C = "); Serial.println(temp);
  } else { 
    Serial.println("Failed to read temperature");
  }
  
  if (! isnan(humi)) {  // check if 'is not a number'
    Serial.print("Hum. % = "); Serial.println(humi);
  } else { 
    Serial.println("Failed to read humidity");
  }

  // convert to microseconds
  ThingSpeak.setField(5,temp);
  ThingSpeak.setField(6,humi);
  ThingSpeak.writeFields(THINKSPEAK_CHANNEL, THINGSPEAK_API_KEY);
  ESP.deepSleep(sleepMinutes * 1000000);
}


void loop() {}
