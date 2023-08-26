/******************************************
 *
 * This example works for both Industrial and STEM users.
 *
 * Developed by Jose Garcia, https://github.com/jotathebest/
 *
 * ****************************************/

/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsEsp32Mqtt.h"

/****************************************
 * Define Constants
 ****************************************/
const char *UBIDOTS_TOKEN = "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x";            // Put here your Ubidots TOKEN
const char *WIFI_SSID = "NETLIFE-RIVER";                // Put here your Wi-Fi SSID
const char *WIFI_PASS = "0909039109";                // Put here your Wi-Fi password
const char *PUBLISH_DEVICE_LABEL = "master";     // Put here your Device label to which data  will be published
const char *PUBLISH_VARIABLE_LABEL = "m";   // Put here your Variable label to which data  will be published
const char *DEVICE_LABEL = "master";   // Replace with the device label to subscribe to
const char *VARIABLE_LABEL = "m"; // Replace with the variable label to subscribe to
int dato=0;
Ubidots ubidots(UBIDOTS_TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();
  ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); // Insert the dataSource and Variable's Labels
}

void loop()
{
  // put your main code here, to run repeatedly:
  if (!ubidots.connected())
  {
    ubidots.reconnect();
    ubidots.subscribeLastValue(DEVICE_LABEL, VARIABLE_LABEL); // Insert the dataSource and Variable's Labels
  }
  ubidots.loop();

  if(Serial.available()>0)
  {
    dato = Serial.read();
  }
  if(dato == '@')
  {
    Serial.println("Abierto");
    dato=0;
  }
  
}
