/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include<SimpleDHT.h>
#include <Wire.h>
#include <SoftwareSerial.h>

/****************************************
 * Define Constants
 ****************************************/
//ubb1 Neveras A
//#define TOKEN "BBFF-LXGwPPTaT5yeOocJNYYUFFJeBUmQmu" // Your Ubidots TOKEN LaboratorioHP
//usuario: laboratoriohp
//laboratorio.hp.nodo.01@gmail.com
//1Corintios13
//variable1=
//variable2=

//ubi2 Estufas A
//#define TOKEN "BBFF-Zzh9TvCUibEC8TpiI8RyLQCS6RgOCE" // Your Ubidots TOKEN LaboratorioHP
//usuario: laboratoriohp2
//laboratorio.hp.nodo.02@gmail.com
//1Corintios13

//ubi3 NeveraB y EstufaB
//#define TOKEN "BBFF-YLLALz8VStDhPsDYfahSrkhJ0LAGKb" // Your Ubidots TOKEN LaboratorioHP
//usuario: laboratoriohp3
//laboratorio.hp.nodo.03@gmail.com
//1Corintios13

//ubi4
#define TOKEN "BBFF-dYcd6QKQtxexVmGrVqqyavDaUrQVcG" // NEV-004
//usuario: laboratorioho4
//laboratorio.hp.nodo.04@gmail.com
//1Corintios13

//ubi5
//#define TOKEN "BBFF-3PIO6kmhwzCdDbFMCH4AiAJMmmInie" // FRZ-002
//usuario: laboratorio.hp5
//laboratorio.hp.nodo.08@gmail.com
//1Corintios13
  
//ubi6 
//#define TOKEN "BBFF-z98G07JjHkFuSCfY3Dk9UTGKSTA7zH" // Your Ubidots TOKEN LaboratorioHP
//usuario: laboratoriohp6
//laboratorio.hp.nodo.06@gmail.com
//1Corintios13

//ubi7
//#define TOKEN "BBFF-e45XW51w7LwlN7BBGOFgAysiwB4U8p" // INC-004 AMBIENTE E
//usuario: laboratoriohp7
//laboratorio.hp.nodo.07@gmail.com
//1Corintios13

//ubi8
//#define TOKEN "BBFF-QbSNtcKG2MZ2TvwmxzrsYCqj2UfYlp" // Your Ubidots TOKEN LaboratorioHP
//usuario: laboratoriohp8
//laboratorio.hp.nodo.08@gmail.com
//1Corintios13

//ubi9             
//#define TOKEN "BBFF-4QZGEY09H4HAEmTCN2puoNWrHZjw5X" // Your Ubidots TOKEN LaboratorioHP
//usuario: laboratoriohp9
//laboratorio.hp.nodo.09@gmail.com
//1Corintios13

//ubi10
//#define TOKEN "BBFF-VLAuqIPfocMzwwoeuwHtBpY4yXux2h" // ambiente C NEV-005
//usuario: laboratoriohp10
//laboratorio.hp.nodo.10@gmail.com
//1Corintios13

//ubi11
#define TOKEN "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x" // JOHNCITO
//usuario: jariver86



//#define WIFINAME "NETLIFE-PARRA" //Your SSID
//#define WIFIPASS "0913233904" // Your Wifi Pass

#define WIFINAME "NETLIFE-RIVERA" //Your SSID
#define WIFIPASS "0909039109" // Your Wifi Pass

Ubidots client(TOKEN);

SoftwareSerial mySerialbt(13,15); // RX-D7, TX-D8 Serial Node remapeado 
/****************************************
 * Auxiliar Functions
 ****************************************/
int pinDHT11 = 14; //PIN D5 / GPIO14
int contador=0;
int centinela=0;
int contador2=0;


SimpleDHT11 dht11(pinDHT11);
//SimpleDHT22 dht11(pinDHT11);

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
  mySerialbt.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  }

void loop() {

  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    //Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
    return;
  }
  Serial.print("temperatura: ");
  Serial.print(String(temperature));
  Serial.print(" C, ");
  Serial.print("Humedad: ");
  Serial.print((int)humidity); 
  Serial.println(" H");
  
  
  
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      }
  if((centinela==0)&&(contador2<10))
  {
    contador2++; 
  }
  else  {centinela=1;}
  
  // Publish values to 2 different data sources
  if((contador>90)||(centinela==0))
  {
    //#define TOKEN
    //client.add("NeveraA1G", temperature); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp1_ZA");
    //client.add("NeveraA2M", temperature); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp1_ZA");

    //#define TOKEN 
    //client.add("EstufaA1G", temperature); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp2_ZA");
    //client.add("EstufaA2P", temperature+2); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp2_ZA");

    //#define TOKEN 
    //client.add("NeveraBP", temperature-5); //NEV-002
    //client.ubidotsPublish("Laboratoriohp3_ZB");//NEV-002
    //client.add("EstufaBG", temperature+3); //INC002
    //client.ubidotsPublish("Laboratoriohp3_ZB");//INC002

    //#define TOKEN
    client.add("NeveraCG", temperature); //NEV-005 NG con DHT11
    client.ubidotsPublish("Laboratoriohp4_ZCF");//NEV-005 NG desconectado
    //client.add("NeveraFP", temperature+2); //NEV-004
    //client.ubidotsPublish("Laboratoriohp4_ZCF");//NEV-004

    //#define TOKEN
    //client.add("NeveraD1P", temperature-4); //NEV-001
    //client.ubidotsPublish("Laboratoriohp5_ZD");//NEV-001
    //client.add("NeveraD2G", temperature-255); //FRZ-002
    //client.ubidotsPublish("Laboratoriohp5_ZD");//FRZ-002

    //#define TOKEN
    //client.add("EstufaE1P", temperature+3); //EST-002
    //client.ubidotsPublish("Laboratoriohp6_ZE");//EST-002
    //client.add("EstufaE2P", temperature); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp6_ZE");

    //#define TOKEN
    //client.add("EstufaE3P", temperature-1); //// INC-004
    //client.ubidotsPublish("Laboratoriohp7_ZE");// INC-004
    //client.add("AmbienteE", temperature-3); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp7_ZE");

    //#define TOKEN
    //client.add("AmbienteF", temperature); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp8_ZFD");
    //client.add("AmbienteD", temperature-3); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp8_ZFD");

    //#define TOKEN
    //client.add("AmbienteA", temperature); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp9_ZAB");

    //DHT11
    //client.add("AmbienteB", temperature-10); //Insert your variable Labels and the value to be sent
    //client.ubidotsPublish("Laboratoriohp9_ZAB");

    //#define TOKEN
    //client.add("AmbienteC", temperature-7); // Ambiente C NEV-005
    //client.ubidotsPublish("Laboratoriohp10_ZC"); // Ambiente C NEV-005
    //client.add("disponible", temperature); //
    //client.ubidotsPublish("Laboratoriohp10_NZ");
    
    client.loop();
    Serial.println("Dato enviado a web temperatura: " + String(temperature));//Informacion a PC
    mySerialbt.println("_@"+String(temperature)+"@;");//Actualizo dato a BT
    contador=0;
  }
  Serial.println("Dato enviado a web temperatura: " + String(temperature));//Informacion a PC
  contador++;
  delay(10000);
  }
