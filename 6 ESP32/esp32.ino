//#include "secrets.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"
#include <SimpleDHT.h>
 
 
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

////////////////////////////////////////////////////////////////////////////
#define SECRET
#define THINGNAME "ESP32_DHT11_2"                         //change this

int pinDHT11 = 14;
SimpleDHT11 dht11(pinDHT11);
float h;
float t;
  
const char WIFI_SSID[] = "NETLIFE-RIVERA";               //change this
const char WIFI_PASSWORD[] = "0909039109";           //change this
const char AWS_IOT_ENDPOINT[] = "ajjqwn71x5ci5-ats.iot.us-east-1.amazonaws.com";       //change this
                                 
// Amazon Root CA 1 ok
static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";
 
// Device Certificate  ok                                             //change this
static const char AWS_CERT_CRT[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAKfP6se1NFoYv+rbTbLS3KSIABmtMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMjA4MjkwMzQ1
MzZaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDDRIsyGPO8UO4QPPy6
FyChtSXe5LsHdckw0/bb8yqomkfL8aQGuzfI3WA0W2gbpM7cPIrAGqzOoe5zE2O3
o+WqQnpGyzDmOJBn6CIuyGGv2oORoyCxClV4aox5FIfsOgh1DL92Z8y6o4LBlIYZ
BN9vCWHXVCw8zAImR/x2fbFp7W1sx5JJos5fk21wAzzhTk0aikpiw+mpJBL2NTO7
gLuA56UWpzcqOEPHuaaABJbAJsghgBEUQM0o2u7yXOc0OvWGHhWaeKZr0w4KCw7H
vDalvc4CvH9tMGYAQIs4jJZ2JC1lTOv9RwmayGZdWLSXW4QnjZyY6R/XN82V+fhZ
4sLXAgMBAAGjYDBeMB8GA1UdIwQYMBaAFCMDTRsmy3gkfjJrNR09hwdCv4RxMB0G
A1UdDgQWBBRytBQe6uNSX31iNn375YOlSsVK4zAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAhbnL5NTInluRuJF2IfGrAyBH
vvNnJ2Y0/+y7uMnqhBLMsIJd6YJ5sTFvscz1i5o1biKuxKv2f4hFLQKkx8pbr3KW
Hz+DiVO1FoxcKDUxx30a049JCzNdfzsjQnWA3s9tHMJBFXwtdVPxuh53vIjcZEIz
CPwRI14xh/hIKKlU84YmWadgDmRkhMhORWaQv+6AeD7skp0r6W8chc8b0XsxPtou
weL7gkaaTYLWBSg2TvcHcHF2TGCRO5O6Ydwx9hFd4etPy75pNw5v9kXrnwmZ2XPQ
32AXNG68b0XMqV7fT2Mt8itngzz3n2NPmBp3nsf4CAAAqWpT9y4IEgA+vZhhHA==
-----END CERTIFICATE-----
 
 
)KEY";
 
// Device Private Key                                               //change this
static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEpAIBAAKCAQEAw0SLMhjzvFDuEDz8uhcgobUl3uS7B3XJMNP22/MqqJpHy/Gk
Brs3yN1gNFtoG6TO3DyKwBqszqHucxNjt6PlqkJ6Rssw5jiQZ+giLshhr9qDkaMg
sQpVeGqMeRSH7DoIdQy/dmfMuqOCwZSGGQTfbwlh11QsPMwCJkf8dn2xae1tbMeS
SaLOX5NtcAM84U5NGopKYsPpqSQS9jUzu4C7gOelFqc3KjhDx7mmgASWwCbIIYAR
FEDNKNru8lznNDr1hh4Vmnima9MOCgsOx7w2pb3OArx/bTBmAECLOIyWdiQtZUzr
/UcJmshmXVi0l1uEJ42cmOkf1zfNlfn4WeLC1wIDAQABAoIBACr8PNQzb/xlqHKE
qnHgw3s1oHeXzKBQypBoKPC0cBCbnfCEakJdFmOucA42jQ2A01TiMv0lIiN+vDoy
64Gyn81CQx6wfjw/+XIWROgdjnui09HMola22qfedLv2zBExmUkDHGbZZNU1R4SL
yJVbYEBWRWacjfx4QITi1e3Tr6mMi1kZwcftYr/33ZyTZQeE7AvAQYvcAZlEC+ZN
bQXrzHKQn6GJslStOUIYCHLrUMqzxFDD0J2gwBicj38IDcmqq+YPyUzu1kQET7LP
Tiw5yJ5AuPuynNeSTy52i8KoyFkdAiJaKXBlsfUZZV+voJ6x0aJwL644ebH3uIcA
+kX9LjkCgYEA37KLZjwq0BO07uFK4/qdv3ZEWcjbWjwSbqP6lea1zXRvyY4+KMuX
n4hlS1tHF+8JPdLnqnfBaH4pHqhWww5DCNnYTa0J58+sKOvlc5bBqPp/AD7CfNqp
5gM9NFSQellQGSEw0bVSKklN+nqpS06NSTKK/BHr4nnsuLcXtoY0WGsCgYEA33cI
5H6fWzxeA4t0e0YVwgd+wKLAze9kD+rusyLfJu5fS7FpgIf66LfBnfhdichbgPbG
gxVZVg5+66QDrYyKkqRdNwxuCAD71YkzDo6Cw1PiERk17HWd7VWCfhovRGBySOhZ
AWk+Kyl1RGJ/tWcR5uoeSeQyxsCVi+NPnJKoSkUCgYEA3wgnEHGgxgE//oRFpPQ0
37ZNA6/NdDQ8tG9t4pM6ZHRZGH5n30jODdNyGyYQxf4gGrHzJPVtsn9HBDgW+Td5
ApQgo6GJJMTcA5jfFtYSlCXdBiOMyetCRgCov0opGrN9tvBuukeseljIFJrDvD+/
gmGQTS6ZHx8tNCXo0KUR/60CgYBPUxH1Cn3HBUbaO0a9+8L9fWuhNiT69V+vxtW1
AUv1pehuFiw9VAfdVF2GlgOeRWE2BmOxZIKx6bNkMnSbxqfqJi5wvBjS/oZm5q0Z
Hb2NWNmG+EwMBDRra09HNERTIWlwOApgVdubtrvJ6q0q18ujDLhzwm3D39k2Lxqz
mdvjgQKBgQDECvSQ/2Nx1jVhTBn0ldOIRcVOyLN1/kSVLIFJ99wM9BGPt3r9e//k
boRjSZno9cZ5ga9MkXJo6Y9UFLls+YjuOM1aCmjUg2a0VkkIObONp+jvlkcXHkfk
I9xwORTdHDoI9X8m1VgQ3tIwOIEcV9E0WvYS/Q/89tXO5jb7cy5rbw==
-----END RSA PRIVATE KEY----- 
 
)KEY";
////////////////////////////////////////////////////////////////////////////


 
 
WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
 
void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
  Serial.println("Connecting to Wi-Fi");
 
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
 
  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);
 
  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  client.setServer(AWS_IOT_ENDPOINT, 8883);
 
  // Create a message handler
  client.setCallback(messageHandler);
 
  Serial.println("Connecting to AWS IOT");
 
  while (!client.connect(THINGNAME))
  {
    Serial.print(".");
    delay(100);
  }
 
  if (!client.connected())
  {
    Serial.println("AWS IoT Timeout!");
    return;
  }
 
  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);
 
  Serial.println("AWS IoT Connected!");
}
 
void publishMessage()
{
  StaticJsonDocument<200> doc;
  doc["humidity"] = h;
  doc["temperature"] = t;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
 
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}
 
void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}
 
void setup()
{
  Serial.begin(115200);
  connectAWS();
 
}
 
void loop()
{
  byte temperature = 0;
  byte humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht11.read(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) {
    Serial.print("Read DHT11 failed, err="); Serial.print(SimpleDHTErrCode(err));
    Serial.print(","); Serial.println(SimpleDHTErrDuration(err)); delay(1000);
    return;
  }
  Serial.print("Sample OK: ");
  Serial.print((int)temperature); Serial.print(" *C, "); 
  Serial.print((int)humidity); Serial.println(" H");

  t = temperature;
  h = humidity;
 
 
  if (isnan(h) || isnan(t) )  // Check if any reads failed and exit early (to try again).
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
 
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.println(F("°C "));
 
  publishMessage();
  client.loop();
  delay(10000);
}
