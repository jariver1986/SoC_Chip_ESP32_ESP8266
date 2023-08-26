#include <LiquidCrystal_I2C.h>  //Libreria para control de LCD.
#include <Wire.h>               //Libreria complemento para control de protocolo i2C.
#include <String.h>             //Libreria para manejo de cadena de caracteres.

LiquidCrystal_I2C lcd(0x27,20,4); // LCD
String UV = "0"; 
int luz = "0"; 
String estado1 = "apagado"; 
String estado2 = "apagado";
String estado3 = "apagado"; 
String estado4 = "apagado"; 
int dato=0;
int dato1=0;
int contador=0;
int contador2=0;

int r1=0;
int r2=0;
int r3=0;
int r4=0;
int distancia=100;

int centinela=0;
int validador=0;
int master=0;
int trabaja=0;
int contador3=0;
void setup()
{
  //analogReference(INTERNAL);
  Serial.begin(115200);
  Serial1.begin(9600);
  Serial2.begin(115200);
  lcd.init();                       //Inicio de pantalla LCD
  lcd.backlight();                  //Enciende iluminacion de fondo de pantalla LCD
  lcd.setCursor(0, 0);lcd.print("Medidor Energia");
  lcd.setCursor(0, 1);lcd.print("");
  lcd.setCursor(0, 2);lcd.print("");
  lcd.setCursor(0, 3);lcd.print("");
  delay(2000);
  //Pin programado para relay
  pinMode(22,OUTPUT);
  pinMode(23,OUTPUT);
  pinMode(24,OUTPUT);
  pinMode(25,OUTPUT);
  //Pin programado para relay apagado inicialmente
  digitalWrite(22,HIGH);
  digitalWrite(23,HIGH);
  digitalWrite(24,HIGH);
  digitalWrite(25,HIGH);
  analogReference(INTERNAL1V1); //solo Arduino Mega
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
  /////////////////////////////////////////////////////////////////////
void loop() 
{
  //Sensor de movimiento
  int movimiento=analogRead(A3);
  int movimiento2=analogRead(A4);
  int movimiento3=analogRead(A5);

  if ((movimiento>50)||(movimiento2>50)||(movimiento3>50))
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("PIR1= "+String(movimiento));
    lcd.setCursor(0, 1);lcd.print("PIR2= "+String(movimiento2));
    lcd.setCursor(0, 2);lcd.print("PIR3= "+String(movimiento3));
    delay(1500);
    lcd.clear();
    trabaja=1;
  }

  contador3++;
  //inactividad mayor a 5 minutos
  if(contador3>300)  
  {
    trabaja=0;
    contador3=0;
  }
  
  //Lectura de sensor de UV
  UV = readSensor();
  
  //Lectura de Sensor de Luz
  luz=(1000-analogRead(A2))/10;
  if(luz<0) luz=0; if(luz>100) luz=100;

  //Medido de energia
  float Irms=get_corriente(); //Corriente eficaz (A)
  float P=Irms*110.0; // P=IV (Watts)

  //Presentacion de datos en pantalla
  lcd.setCursor(0, 0);lcd.print("Medidor Energia");
  lcd.setCursor(0, 1);lcd.print("UV= "+UV);
  lcd.setCursor(0, 2);lcd.print("Luz= "+String(luz)+"%");
  lcd.setCursor(0, 3);lcd.print("Watts= "+String(P,2));


  if (contador>5)
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("L1= "+estado1);
    lcd.setCursor(0, 1);lcd.print("L2= "+estado2);
    lcd.setCursor(0, 2);lcd.print("L3= "+estado3);
    lcd.setCursor(0, 3);lcd.print("L4= "+estado4);
    contador=0;
    delay(1500);
    lcd.clear();
  }
  contador++;

  

    //Encendido de relay desde APP
  if (Serial1.available() > 0)
  {
     dato1 = Serial1.read();
     if(dato1=='A')
     {
      digitalWrite(22,HIGH);
      estado1="apagado";
      r1=0;
      master=0;
     }
     if(dato1=='B')
     {
      digitalWrite(22,LOW);
      estado1="encendido";
      r1=1;
      master=1;
     }

     if(dato1=='C')
     {
      digitalWrite(23,HIGH);
      estado2="apagado";
      r2=0;
      master=0;
     }
     if(dato1=='D')
     {
      digitalWrite(22,LOW);
      estado2="encendido";
      r2=1;
      master=1;
     }
     
      if(dato1=='E')
     {
      digitalWrite(24,HIGH);
      estado3="apagado";
      r3=0;
      master=0;
     }
     if(dato1=='F')
     {
      digitalWrite(24,LOW);
      estado3="encendido";
      r3=1;
      master=1;
     }
     if(dato1=='G')
     {
      digitalWrite(25,HIGH);
      estado4="apagado";
      r4=0;
      master=0;
     }
     if(dato1=='H')
     {
      digitalWrite(25,LOW);
      estado4="encendido";
      r4=1;
      master=1;
     }
     if(dato1=='I')
     {
      digitalWrite(22,LOW);
      digitalWrite(23,LOW);
      digitalWrite(24,LOW);
      digitalWrite(25,LOW);
      estado1="encendido";
      estado2="encendido";
      estado3="encendido";
      estado4="encendido";
      r1=1;
      r2=1;
      r3=1;
      r4=1;
      master=1;
     }
     if(dato1=='J')
     {
      digitalWrite(22,HIGH);
      digitalWrite(23,HIGH);
      digitalWrite(24,HIGH);
      digitalWrite(25,HIGH);
      estado1="apagado";
      estado2="apagado";
      estado3="apagado";
      estado4="apagado";
      r1=0;
      r2=0;
      r3=0;
      r4=0;
      master=0;
     }
     dato1=0;
  }
  //Se activa cuando movimiento
  if(trabaja==1)
  {
    if((luz>25)&&(luz<=50)&&(master==0)) 
    {
      digitalWrite(22,LOW);
      digitalWrite(24,LOW);
      estado1="encendido";
      estado3="encendido";
      r1=1;
      r3=1;
    }
    if((luz<=25)&&(master==0)) 
    {
      digitalWrite(22,LOW);
      digitalWrite(23,LOW);
      digitalWrite(24,LOW);
      digitalWrite(25,LOW);
      estado1="encendido";
      estado2="encendido";
      estado3="encendido";
      estado4="encendido";
      r1=1;
      r2=1;
      r3=1;
      r4=1;
    }
  }
  //Se apagan las luces por falta de actividad en la oficina no hay control master
  //no hay personas
  if(trabaja==0)
  {
    digitalWrite(22,HIGH);
    digitalWrite(23,HIGH);
    digitalWrite(24,HIGH);
    digitalWrite(25,HIGH);
    estado1="apagado";
    estado2="apagado";
    estado3="apagado";
    estado4="apagado";
    r1=0;
    r2=0;
    r3=0;
    r4=0;
  }
  /////////////////////////////////////////////////////////////////////
  
  //envio de datos a Arduino ESP32 
  if ((contador2>3600)||(validador==0))
  {
    Serial.println(String(UV)+"@"+String(luz)+"@"+String(P)+"@"+String(r1)+"@"+String(r2)+"@"+String(r3)+"@"+String(r4)+"@"); 
    Serial1.println(String(UV)+"@"+String(luz)+"@"+String(P)+"@"+String(r1)+"@"+String(r2)+"@"+String(r3)+"@"+String(r4)+"@");
    Serial2.println(String(UV)+"@"+String(luz)+"@"+String(P)+"@"+String(r1)+"@"+String(r2)+"@"+String(r3)+"@"+String(r4)+"@");
    delay(500);
    Serial2.println(String(UV)+"@"+String(luz)+"@"+String(P)+"@"+String(r1)+"@"+String(r2)+"@"+String(r3)+"@"+String(r4)+"@");
    contador2=0;
    validador=1;
  }
  contador2++;
  delay(1000);
}

 
