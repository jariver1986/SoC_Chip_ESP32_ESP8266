

/*
 * 
 * #include <SoftwareSerial.h>
 *  #define TOKEN "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x" // Your Ubidots TOKEN
    #define WIFINAME "NETLIFE-RIVERA" //Your SSID
    #define WIFIPASS "0909039109" // Your Wifi Pass
    SoftwareSerial mySerial(13, 15);
    mySerial.begin(115200);
 */

/****************************************
 * Include Libraries
 ****************************************/
#include <UbidotsESPMQTT.h>



/****************************************
 * Define Constants
 ****************************************/
             //BBFF-1tXWZhtt2GHBQGDZfcrlmaoEKIG2sb
#define TOKEN "BBFF-1tXWZhtt2GHBQGDZfcrlmaoEKIG2sb" // Your Ubidots TOKEN
#define WIFINAME "TC_SISTEMAS" //Your SSID
#define WIFIPASS "Ingenieria2022" // Your Wifi Pass

//#define WIFINAME "NETLIFE-RIVERA" //Your SSID
//#define WIFIPASS "0909039109" // Your Wifi Pass
Ubidots client(TOKEN);
String incomingString1="";
String incomingString2="";
String incomingString3="";
String incomingString4="";
String incomingString5="";
String incomingString6="";
String incomingString7="";
String incomingString8="";
float valor1=0;
float valor2=0;
float valor3=0;
float valor4=0;
float valor5=0;
float valor6=0;
float valor7=0;
float valor8=0;
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
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  }

void loop() {
  //1.7@77@100@1@1@1@1@
  if (Serial1.available() > 2) 
  {
    incomingString1 = Serial.readStringUntil('@');
    incomingString2 = Serial.readStringUntil('@');
    incomingString3 = Serial.readStringUntil('@');
    incomingString4 = Serial.readStringUntil('@');
    incomingString5 = Serial.readStringUntil('@');
    incomingString6 = Serial.readStringUntil('@');
    incomingString7 = Serial.readStringUntil('@');
    incomingString8 = Serial.readStringUntil('@');
    
    if((incomingString1!=0)||(incomingString2!=0)||(incomingString3!=0))
    {
      Serial.print("He recibido: ");
      Serial.println(incomingString1);
      valor1 = incomingString1.toFloat();
      valor2 = incomingString2.toFloat();
      valor3 = incomingString3.toFloat();
      valor4 = incomingString4.toFloat();
      valor5 = incomingString5.toFloat();
      valor6 = incomingString6.toFloat();
      valor7 = incomingString7.toFloat();
      valor8 = incomingString8.toFloat();
      // put your main code here, to run repeatedly:
      if(!client.connected()){
          client.reconnect();
          }
      
      // Publish values to 2 different data sources
      
      client.add("lux", valor1); 
      client.add("luxS", valor2); 
      client.add("voltaje", valor3); 
      client.ubidotsPublish("Datos1");
      client.add("corriente", valor4);
      client.add("power", valor5);
      client.add("energia", valor6);
      client.add("frecuencia", valor7);
      client.add("fp", valor8);
      client.ubidotsPublish("Datos2");
      client.loop();
    }
  }
  
  delay(1000);
  }
