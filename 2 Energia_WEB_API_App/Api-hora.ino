#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#ifndef STASSID
#define STASSID "NETLIFE-RIVERA"
#define STAPSK  "0909039109"
#endif

const char* ssid     = STASSID;
const char* password = STAPSK;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print(".");}

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  String Time = GetTime();
  Serial.println(Time);
  Serial.print("Fecha: ");
  for(int i=0; i<10;i++)
  {
    Serial.print(Time[i]);
  }
  Serial.println();
  Serial.print("Hora: ");
  for(int i=11; i<=18;i++)
  {
    Serial.print(Time[i]);
  }
  Serial.println();
  delay(60000);
}
String GetTime()
{
  WiFiClient client;
  HTTPClient http;
  String timeS = "";

  http.begin(client,"http://worldtimeapi.org/api/timezone/America/Guayaquil");
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
  {
    String payload = http.getString();
    int beginS = payload.indexOf("datetime");//posicion donde escuentra el string
    int endS = payload.indexOf("day_of_week");//posicion donde escuentra el string
    Serial.println(payload);
    Serial.println();
    timeS = payload.substring(beginS +11,endS-3);//recorto el string
    Serial.println(timeS);
    Serial.println();
  }
  return timeS;
  
}
