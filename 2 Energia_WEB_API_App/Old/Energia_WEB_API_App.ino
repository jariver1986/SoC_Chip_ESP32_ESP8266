#include <LiquidCrystal_I2C.h>  //Libreria para control de LCD.
#include <Wire.h>               //Libreria complemento para control de protocolo i2C.
#include <String.h>             //Libreria para manejo de cadena de caracteres.

LiquidCrystal_I2C lcd(0x27,20,4); // LCD
String UV = "0"; 
String luz = "0"; 
String estado1 = "apagado"; 
String estado2 = "apagado"; 
int dato=0;
int dato1=0;


  
void setup() 
{
  //analogReference(INTERNAL);
  Serial.begin(115200);
  lcd.init();                       //Inicio de pantalla LCD
  lcd.backlight();                  //Enciende iluminacion de fondo de pantalla LCD
  lcd.setCursor(0, 0);lcd.print("Medidor Energia");
  lcd.setCursor(0, 1);lcd.print("");
  lcd.setCursor(0, 2);lcd.print("");
  lcd.setCursor(0, 3);lcd.print("");
  delay(2000);
  //Pin programado para relay
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  //Pin programado para relay apagado inicialmente
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
}
float get_corriente()
{
  float voltajeSensor;
  float corriente=0;
  float Sumatoria=0;
  long tiempo=millis();
  int N=0;
  while(millis()-tiempo<500)//Duración 0.5 segundos(Aprox. 30 ciclos de 60Hz)
  { 
    voltajeSensor = analogRead(A1) * (1.1 / 1023.0);////voltaje del sensor
    corriente=voltajeSensor*30.0; //corriente=VoltajeSensor*(30A/1V)
    Sumatoria=Sumatoria+sq(corriente);//Sumatoria de Cuadrados
    N=N+1;
    delay(1);
  }
  Sumatoria=Sumatoria*2;//Para compensar los cuadrados de los semiciclos negativos.
  corriente=sqrt((Sumatoria)/N); //ecuación del RMS
  return(corriente);
}
String readSensor()
{
  String UVIndex = "0";
  int sensorValue = 0;
  
  sensorValue = analogRead(0);                        //connect UV sensor to Analog 0   
  int voltage = (sensorValue * (5.0 / 1023.0))*1000;  //Voltage in miliVolts
  
  if(voltage<50)
  {
    UVIndex = "0";
  }else if (voltage>50 && voltage<=227)
  {
    UVIndex = "0";
  }else if (voltage>227 && voltage<=318)
  {
    UVIndex = "1";
  }
  else if (voltage>318 && voltage<=408)
  {
    UVIndex = "2";
  }else if (voltage>408 && voltage<=503)
  {
    UVIndex = "3";
  }
  else if (voltage>503 && voltage<=606)
  {
    UVIndex = "4";
  }else if (voltage>606 && voltage<=696)
  {
    UVIndex = "5";
  }else if (voltage>696 && voltage<=795)
  {
    UVIndex = "6";
  }else if (voltage>795 && voltage<=881)
  {
    UVIndex = "7";
  }
  else if (voltage>881 && voltage<=976)
  {
    UVIndex = "8";
  }
  else if (voltage>976 && voltage<=1079)
  {
    UVIndex = "9";
  }
  else if (voltage>1079 && voltage<=1170)
  {
    UVIndex = "10";
  }else if (voltage>1170)
  {
    UVIndex = "11";
  }
  return UVIndex;
}
void loop() 
{
  //Lectura de sensor de UV
  UV = readSensor();
  
  //Lectura de Sensor de Luz
  luz=((1023-analogRead(A2))*100)/1023;

  //Medido de energia
  float Irms=get_corriente(); //Corriente eficaz (A)
  float P=Irms*110.0; // P=IV (Watts)
  Serial.print("Irms: ");
  Serial.print(Irms,3);
  Serial.print("A, Potencia: ");
  Serial.print(P,3);  
  Serial.println("W");
  delay(500);     

  //Presentacion de datos en pantalla
  lcd.setCursor(0, 0);lcd.print("Medidor Energia");
  lcd.setCursor(0, 1);lcd.print(UV);
  lcd.setCursor(0, 2);lcd.print(luz);
  lcd.setCursor(0, 3);lcd.print(P);

  //encendido de relay desde APP
  if (Serial.available() > 0) 
  {
     dato1 = Serial.read();
     if(dato1=='A')
      digitalWrite(2,HIGH);
     if(dato1=='B')
      digitalWrite(2,LOW);
     if(dato1=='C')
      digitalWrite(3,HIGH);
     if(dato1=='D')
      digitalWrite(3,LOW);
      //añadir 2 relays mas
  }

  //envio de datos a Arduino ESP32 
  Serial.println(String(UV)+"#"+String(luz)+"#"+String(P)+"#"+String(estado1)+"#"+String(estado2)+"#."); //añadir 2 relays mas
  
}

 
