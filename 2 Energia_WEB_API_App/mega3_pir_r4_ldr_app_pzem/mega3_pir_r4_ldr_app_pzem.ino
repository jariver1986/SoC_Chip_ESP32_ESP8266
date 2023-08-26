#include <LiquidCrystal_I2C.h>  //Libreria para control de LCD.
#include <Wire.h>               //Libreria complemento para control de protocolo i2C.
#include <String.h>             //Libreria para manejo de cadena de caracteres.
#include <PZEM004Tv30.h>
#include <Wire.h> 

PZEM004Tv30 pzem(&Serial3);
LiquidCrystal_I2C lcd(0x27,20,4); // LCD
String UV = "0"; 
int luz = "0"; 
int luz2 = "0"; 
int luz3 = "0"; 
String estado1 = "apagado"; 
String estado2 = "apagado";
String estado3 = "apagado"; 
String estado4 = "apagado"; 
int dato=0;
int dato1=0;
int contador2=0;
int muestra=0;

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
  Serial3.begin(9600);
  lcd.init();                       //Inicio de pantalla LCD
  lcd.backlight();                  //Enciende iluminacion de fondo de pantalla LCD
  lcd.setCursor(0, 0);lcd.print("        UCSG        ");
  lcd.setCursor(0, 1);lcd.print("      Proyecto      ");
  lcd.setCursor(0, 2);lcd.print("   Medidor Energia  ");
  lcd.setCursor(0, 3);lcd.print("Por: Carlos Poveda  ");
  delay(2000);
  lcd.clear();
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
  int movimiento=analogRead(A3); // PIR1
  int movimiento2=analogRead(A4);// PIR2
  int movimiento3=analogRead(A5);// PIR3
  //Muestra en LCD los estados de los sensores de movimiento
  if (((movimiento>50)||(movimiento2>50)||(movimiento3>50))&&(trabaja!=2))
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("Sensores Movimiento");
    lcd.setCursor(0, 1);lcd.print("PIR1= "+String(movimiento));
    lcd.setCursor(0, 2);lcd.print("PIR2= "+String(movimiento2));
    lcd.setCursor(0, 3);lcd.print("PIR3= "+String(movimiento3));
    delay(1500);//Bajar nuemro para mayor Velocidad
    lcd.clear();
    trabaja=1;
  }

  contador3++;
  //Para activar la inactividad variable contador3 = inactividad mayor a 5(15 segundos aprox.) setea variable "trabaja=0"
  if((contador3>5)&&(trabaja!=2))  
  {
    trabaja=0;
    contador3=0;
  }
  
  //Lectura de sensor de UV
  UV = readSensor();
  
  //Lectura de Sensor de Luz1
  luz=(1000-analogRead(A2))/10;
  //lum1=luz*10;
  if(luz<0) luz=0; if(luz>100) luz=100;

  //Lectura de Sensor de Luz2
  luz2=(1000-analogRead(A6))/10;
  if(luz2<0) luz2=0; if(luz2>100) luz2=100;

  //Lectura de Sensor de Luz3
  luz3=(1000-analogRead(A7))/10;
  if(luz3<0) luz3=0; if(luz3>100) luz3=100;
  
  
  //Medido de energia
  float voltage = pzem.voltage();
  float current = pzem.current();
  float power = pzem.power();
  float energy = pzem.energy();
  float frequency = pzem.frequency();
  float pf = pzem.pf();
  

  //Presentacion de datos recopilados en pantalla LCD
  if(muestra<3)
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("Sensores de Luz");
    lcd.setCursor(0, 1);lcd.print("Lum1 = "+String(luz)+"");
    lcd.setCursor(0, 2);lcd.print("Lum2 = "+String(luz2)+"");
    lcd.setCursor(0, 3);lcd.print("Lum3 = "+String(luz3)+"");
  }
  if(muestra>=3 && muestra<6)
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("Medidor Energia");
    lcd.setCursor(0, 1);lcd.print("Ciclo = "+String(contador3) + "   T= "+String(trabaja));
    lcd.setCursor(0, 2);lcd.print("Volt[v]= "+String(voltage,2));
    lcd.setCursor(0, 3);lcd.print("I[A]  = "+String(current,2));
  }
  if(muestra>=6 && muestra<9)
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("Fp = "+String(pf));
    lcd.setCursor(0, 1);lcd.print("Pot[W]= "+String(power,2));
    lcd.setCursor(0, 2);lcd.print("E[J]  = "+String(energy,2));
    lcd.setCursor(0, 3);lcd.print("Fr[Hz]= "+String(frequency));
  }
  //Presenta estados de luminarias encendidas o apagadas
  if(muestra>=9 && muestra<12)
  {
    lcd.clear();
    lcd.setCursor(0, 0);lcd.print("Lamp1 = "+estado1);
    lcd.setCursor(0, 1);lcd.print("Lamp2 = "+estado2);
    lcd.setCursor(0, 2);lcd.print("Lamp3 = "+estado3);
    lcd.setCursor(0, 3);lcd.print("Lamp4 = "+estado4);
    
  }
  if(muestra>=12){muestra=0;}
  muestra++;
  

    //Encendido de relay desde APP android recepcion de comandos
  if (Serial1.available() > 0)
  {
     dato1 = Serial1.read();
     if(dato1=='2')
     {
      digitalWrite(22,HIGH);
      estado1="apagado";
      r1=0;
      trabaja=0;
      
     }
     if(dato1=='1')
     {
      digitalWrite(22,LOW);
      estado1="encendido";
      r1=1;
      trabaja=2;
     }

     if(dato1=='4')
     {
      digitalWrite(23,HIGH);
      estado2="apagado";
      r2=0;
      trabaja=0;
     }
     if(dato1=='3')
     {
      digitalWrite(23,LOW);
      estado2="encendido";
      r2=1;
      trabaja=2;
     }
     
      if(dato1=='6')
     {
      digitalWrite(24,HIGH);
      estado3="apagado";
      r3=0;
      trabaja=0;
     }
     if(dato1=='5')
     {
      digitalWrite(24,LOW);
      estado3="encendido";
      r3=1;
      trabaja=2;
     }
     if(dato1=='8')
     {
      digitalWrite(25,HIGH);
      estado4="apagado";
      r4=0;
      trabaja=0;
     }
     if(dato1=='7')
     {
      digitalWrite(25,LOW);
      estado4="encendido";
      r4=1;
      trabaja=2;
     }
     if(dato1=='9')
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
      trabaja=2;
     }
     if(dato1=='A')
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
      trabaja=0;
     }
     dato1=0;
  }

  
  //Se activa cuando movimiento y los sensores de luz estan entre 25 y 50 culquiera de ellos
  //Estas variables deben ser cambiadas tanto luz, luz2 , luz3
  if(trabaja==1)
  {
    if(((luz>25)||(luz2>25)||(luz3>25))&&((luz<=50)||(luz2<=50)||(luz3<=50))) 
    {
      digitalWrite(22,LOW);
      digitalWrite(24,LOW);
      estado1="encendido";
      estado3="encendido";
      r1=1;
      r3=1;
      Serial1.println("A@#;_");//comando para presentar mensaje en APP
    }
    if(((luz<=25)||(luz2<=25)||(luz3<=25))) 
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
      Serial1.println("B@#;_");//comando para presentar mensaje en APP
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
    Serial1.println("a@#;_");//comando para presentar mensaje en APP
  }
  /////////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////////
  //envio de datos a Arduino ESP32 contador2=10 es cada 10segundos se recomienda cada 60
  if ((contador2>300)||(validador==0))
  {
    Serial.println(String(UV)+"@"+String(luz)+"@"+String(voltage)+"@"+String(current)+"@"+String(power)+"@"+String(energy)+"@"+String(frequency)+"@"+String(pf)+"@"); 
    Serial2.println(String(UV)+"@"+String(luz)+"@"+String(voltage)+"@"+String(current)+"@"+String(power)+"@"+String(energy)+"@"+String(frequency)+"@"+String(pf)+"@"); 
    delay(500);//Bajar nuemro para mayor Velocidad
    Serial2.println(String(UV)+"@"+String(luz)+"@"+String(voltage)+"@"+String(current)+"@"+String(power)+"@"+String(energy)+"@"+String(frequency)+"@"+String(pf)+"@"); 
    contador2=0;
    validador=1;
  }
  contador2++;
  delay(1000);//Bajar nuemro para mayor Velocidad
}

 
