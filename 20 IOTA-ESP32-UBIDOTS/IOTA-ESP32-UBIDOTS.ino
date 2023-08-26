long randomNumber;

#include "UbidotsEsp32Mqtt.h"

//OTA
#define COMPDATE __DATE__ __TIME__
#define MODEBUTTON 0                                        // Button pin on the esp for selecting modes. D3 for the Wemos!


#include <IOTAppStory.h>                                    // IotAppStory.com library
IOTAppStory IAS(COMPDATE, MODEBUTTON);                      // Initialize IOTAppStory

// ================================================ DHT11 =========================================
#include <SimpleDHT.h>
int pinDHT11 = 21;
SimpleDHT11 dht11(pinDHT11);
float h;
float t;

// ================================================ EXAMPLE VARS =========================================
// used in this example to blink (LEDpin) every (blinkTime) miliseconds
unsigned long blinkEntry;
String deviceName = "pituIOTA";
String chipId;

// We want to be able to edit these example variables below from the wifi config manager
// Currently only char arrays are supported. (Keep in mind that html form fields always return Strings)
// Use functions like atoi() and atof() to transform the char array to integers or floats
// Use IAS.dPinConv() to convert Dpin numbers to integers (D6 > 14)

char* LEDpin    = "2";                                      // The value given here is the default value and can be overwritten by values saved in configuration mode
char* blinkTime = "1000";

// ================================================ UBIDOTS =========================================
const char *UBIDOTS_TOKEN = "BBFF-Uwy4vl7oGuHe5EKmHsUgknC7AWiY2x";  // Put here your Ubidots TOKEN

const char *WIFI_SSID = "NETLIFE-RIVERA";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "0909039109";      // Put here your Wi-Fi password
const char *DEVICE_LABEL = "Dispositivo";   // Put here your Device label to which data  will be published
const char *VARIABLE_LABEL = "Temperatura"; // Put here your Variable label to which data  will be published
const char *VARIABLE_LABEL2 = "Humedad"; // Put here your Variable label to which data  will be published
const int PUBLISH_FREQUENCY = 5000; // Update rate in milliseconds

unsigned long timer;

Ubidots ubidots(UBIDOTS_TOKEN);
int cont=0;

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
// ================================================ SETUP ================================================
void setup() 
{
  // ================================================ SETUP ================================================
  Serial.begin(115200);
  // ubidots.setDebug(true);  // uncomment this to make debug messages available
  ubidots.connectToWifi(WIFI_SSID, WIFI_PASS);
  ubidots.setCallback(callback);
  ubidots.setup();
  ubidots.reconnect();

  timer = millis();

  // ================================================ IOTA ================================================
  // creat a unique deviceName for classroom situations (deviceName-123)
  chipId      = String(ESP_GETCHIPID);
  chipId      = "-" + chipId.substring(chipId.length() - 3);
  deviceName += chipId;

  IAS.preSetDeviceName(deviceName);                         // preset deviceName this is also your MDNS responder: http://iasblink.local


  IAS.addField(LEDpin, "ledpin", 2, 'P');                   // These fields are added to the config wifimanager and saved to eeprom. Updated values are returned to the original variable.
  IAS.addField(blinkTime, "Blinktime(mS)", 5, 'N');         // reference to org variable | field label value | max char return


  // You can configure callback functions that can give feedback to the app user about the current state of the application.
  // In this example we use serial print to demonstrate the call backs. But you could use leds etc.
  IAS.onModeButtonShortPress([]() {
    Serial.println(F(" If mode button is released, I will enter in firmware update mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });

  IAS.onModeButtonLongPress([]() {
    Serial.println(F(" If mode button is released, I will enter in configuration mode."));
    Serial.println(F("*-------------------------------------------------------------------------*"));
  });

  IAS.onFirmwareUpdateProgress([](int written, int total) {
    Serial.print(".");
  });
  /*
    IAS.onFirstBoot([]() {
    IAS.eraseEEPROM('P');                   // Optional! What to do with EEPROM on First boot of the app? 'F' Fully erase | 'P' Partial erase
    });
  */

  IAS.begin();                              // Run IOTAppStory
  IAS.setCallHomeInterval(60);              // Call home interval in seconds(disabled by default), 0 = off, use 60s only for development. Please change it to at least 2 hours in production


  //-------- Your Setup starts from here ---------------


  pinMode(IAS.dPinConv(LEDpin), OUTPUT);
}



// ================================================ LOOP =================================================
void loop() 
{
  IAS.loop();   // this routine handles the calling home functionality,
  // reaction of the MODEBUTTON pin. If short press (<4 sec): update of sketch, long press (>7 sec): Configuration
  // reconnecting WiFi when the connection is lost,
  // and setting the internal clock (ESP8266 for BearSSL)


// ================================================ DHT11 =================================================
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  
  Serial.print((int)temperature); Serial.println(" *C,  "); 
  Serial.print((int)humidity); Serial.println(" H,  ");

  t = temperature;
  h = humidity;



// ================================================ RANDOM =================================================


  randomNumber = random(1,100);
  //Escribimos el numero aleatorio por el puerto serie
  Serial.print("Aleatorio = ");
  Serial.println(randomNumber);
  Serial.println();
  

//================================================ RANDOM =================================================

if(cont>=30)
  {
        
    if (!ubidots.connected())
    {
      ubidots.reconnect();
    }
    
    //float value = analogRead(analogPin);
    ubidots.add(VARIABLE_LABEL, temperature ); // Insert your variable Labels and the value to be sent
    ubidots.add(VARIABLE_LABEL2, humidity ); // Insert your variable Labels and the value to be sent
    ubidots.publish(DEVICE_LABEL);
    timer = millis();
    
    ubidots.loop();
  }
  if(cont>31) cont=0; 
  
  cont++;
//================================================ delay =================================================

  delay(1000);
}
