/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"

/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBFF-VLskigNZ34QSm55qV5NlrrPxRHpDWT"
//#define TOKEN "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x" // Your Ubidots TOKEN
//#define WIFINAME "NETLIFE-RIVERA" //Your SSID
//#define WIFIPASS "0909039109" // Your Wifi Pass
#define WIFINAME "SERSOCA_Ext" //Your SSID
#define WIFIPASS "S3rs0C@2021+*" // Your Wifi Pass


Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

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

/****************************************
 * Main Functions
 ****************************************/
int dato=0;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  client.ubidotsSubscribe("esp8266","dato"); //Insert the dataSource and Variable's Labels
  pinMode(D2, OUTPUT);
  digitalWrite(D2, HIGH);
  }

void loop() {
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      client.ubidotsSubscribe("esp8266","dato"); //Insert the dataSource and Variable's Labels
      }
  client.loop();
  if(Serial.available()>0)
  {
    dato =Serial.read();
  }

  if(dato=='@')
  {
    digitalWrite(D2, LOW);
    delay(1000);
    digitalWrite(D2, HIGH);
    Serial.println("Abierto por BT");
    dato=0;
  }
  if(dato=='#')
  {
    digitalWrite(D2, LOW);
    delay(500);
    digitalWrite(D2, HIGH);
    Serial.println("Abierto por BT");
    dato=0;
  }
  if(dato=='J')
  {
    digitalWrite(D2, LOW);
    delay(500);
    digitalWrite(D2, HIGH);
    Serial.println("Abierto por BT");
    dato=0;
  }
  
  delay(3000);
  }
