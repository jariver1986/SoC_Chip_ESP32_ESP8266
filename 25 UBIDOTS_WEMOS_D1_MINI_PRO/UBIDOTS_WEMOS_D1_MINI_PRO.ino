/****************************************
 * Include Libraries
 ****************************************/
#include "UbidotsESPMQTT.h"
#include<SimpleDHT.h>
#include <Wire.h>
/****************************************
 * Define Constants
 ****************************************/
#define TOKEN "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x" // Your Ubidots TOKEN
#define WIFINAME "NETLIFE-RIVERA" //Your SSID
#define WIFIPASS "0909039109" // Your Wifi Pass

Ubidots client(TOKEN);



/****************************************
 * Auxiliar Functions
 ****************************************/
//Include the library
#include <MQUnifiedsensor.h>
/************************Hardware Related Macros************************************/
#define         Board                   ("LOLIN")
#define         Pin                     (A0)  //Analog input 3 of your arduino
/***********************Software Related Macros************************************/
#define         Type                    ("MQ-2") //MQ2
#define         Voltage_Resolution      (5)
#define         ADC_Bit_Resolution      (10) // For arduino UNO/MEGA/NANO
#define         RatioMQ2CleanAir        (9.83) //RS / R0 = 9.83 ppm 

/*****************************Globals***********************************************/
MQUnifiedsensor MQ2(Board, Voltage_Resolution, ADC_Bit_Resolution, Pin, Type);
/*****************************Globals***********************************************/




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
//int pinDHT11 = D2;   //14; //PIN D5 / GPIO14
//SimpleDHT11 dht11(pinDHT11);
//SimpleDHT22 dht11(pinDHT11);
int contador=0;
int centinela=0;
int contador2=0;
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);

  //////////////////////////////////////////////////MOQ

   //Set math model to calculate the PPM concentration and the value of constants
  MQ2.setRegressionMethod(1); //_PPM =  a*ratio^b
  MQ2.setA(36974); MQ2.setB(-3.109); // Configure the equation to to calculate LPG concentration
  /*
    Exponential regression:
    Gas    | a      | b
    H2     | 987.99 | -2.162
    LPG    | 574.25 | -2.222
    CO     | 36974  | -3.109
    Alcohol| 3616.1 | -2.675
    Propane| 658.71 | -2.168
  */

  /*****************************  MQ Init ********************************************/ 
  //Remarks: Configure the pin of arduino as input.
  /************************************************************************************/ 
  MQ2.init(); 
  /* 
    //If the RL value is different from 10K please assign your RL value with the following method:
    MQ2.setRL(10);
  */
  /*****************************  MQ CAlibration ********************************************/ 
  // Explanation: 
   // In this routine the sensor will measure the resistance of the sensor supposedly before being pre-heated
  // and on clean air (Calibration conditions), setting up R0 value.
  // We recomend executing this routine only on setup in laboratory conditions.
  // This routine does not need to be executed on each restart, you can load your R0 value from eeprom.
  // Acknowledgements: https://jayconsystems.com/blog/understanding-a-gas-sensor
  Serial.print("Calibrating please wait.");
  float calcR0 = 0;
  for(int i = 1; i<=10; i ++)
  {
    MQ2.update(); // Update data, the arduino will read the voltage from the analog pin
    calcR0 += MQ2.calibrate(RatioMQ2CleanAir);
    Serial.print(".");
  }
  MQ2.setR0(calcR0/10);
  Serial.println("  done!.");
  
  if(isinf(calcR0)) {Serial.println("Warning: Conection issue, R0 is infinite (Open circuit detected) please check your wiring and supply"); while(1);}
  if(calcR0 == 0){Serial.println("Warning: Conection issue found, R0 is zero (Analog pin shorts to ground) please check your wiring and supply"); while(1);}
  /*****************************  MQ CAlibration ********************************************/ 

  MQ2.serialDebug(true);
}

void loop() 
{

      //byte temperature = 0;
      //byte humidity = 0;
      //int err = SimpleDHTErrSuccess;
      //if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
        //Serial.print("Read DHT11 failed, err="); Serial.println(err);delay(1000);
      //  return;
      //}
      //Serial.print("temperatura: ");
      //Serial.print(String(temperature));
      //Serial.print(" C, ");
      //Serial.print("Humedad: ");
      //Serial.print((int)humidity); 
      //Serial.println(" H");

      
      ////////////////////////////////////////////////////////MOQ
      MQ2.update(); // Update data, the arduino will read the voltage from the analog pin
      MQ2.readSensor(); // Sensor will read PPM concentration using the model, a and b values set previously or from the setup
      MQ2.serialDebug(); // Will print the table on the serial port
      //delay(500); //Sampling frequency
      ////////////////////////////////////////////////////////////
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
      if((contador>90)||(centinela==0))  //90U SON 15 MINUTOS
      {
        client.add("co", MQ2.readSensor());    // 2 - 8
        client.ubidotsPublish("casa_isabel");
        //client.add("INC003", temperature);  // 32 - 37
        //client.ubidotsPublish("NEVERA006");
      
      client.loop();
      //Serial.println("Dato enviado a web temperatura: " + String(temperature));//Informacion a PC
      //mySerialbt.println("_@"+String(temperature)+"@;");//Actualizo dato a BT
      contador=0;
  }
  //Serial.println("Dato enviado a web temperatura: " + String(temperature));//Informacion a PC
  Serial.print(MQ2.readSensor());
  contador++;
  delay(10000);
  }
