#include <DS18B20.h>
DS18B20 ds(D1);

#define LOW_ALARM 20
#define HIGH_ALARM 25

#include "UbidotsESPMQTT.h"
#define TOKEN "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x"
#define WIFINAME "NETLIFE-RIVERA" //Your SSID
#define WIFIPASS "0909039109" // Your Wifi Pass
Ubidots client(TOKEN);
int dato=0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if((char)payload[0]=='1')
  {
    digitalWrite(D2, LOW);
    delay(1000);
    digitalWrite(D2, HIGH);
    Serial.println("Abierto");
  }
  if((char)payload[0]=='2')
  {
    digitalWrite(D2, LOW);
    delay(500);
    digitalWrite(D2, HIGH);
    Serial.println("Abierto");
  }
  if((char)payload[0]=='3')
  {
    digitalWrite(D2, LOW);
    delay(1500);
    digitalWrite(D2, HIGH);
    Serial.println("Abierto");
  }
}

void setup() 
{
  Serial.begin(115200);
  while (ds.selectNext()) 
  {
    ds.setAlarms(LOW_ALARM, HIGH_ALARM);
  }
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  //client.ubidotsSubscribe("dev01","dato"); //Insert the dataSource and Variable's Labels
  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);
}

void loop() 
{ 
  ds.doConversion();
  while (ds.selectNextAlarm()) 
  {
    float temp = ds.getTempC();
    Serial.println(temp);
  }
  // put your main code here, to run repeatedly:
  if(!client.connected())
  {
      client.reconnect();
  }
  client.add("temp",(int)ds.getTempC());
  client.ubidotsPublish("dev01");
  client.loop();
  delay(10000);
}
