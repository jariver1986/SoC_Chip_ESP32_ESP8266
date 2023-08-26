#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <UbidotsMicroESP8266.h>

// Datos de conexión a WiFi
const char* WIFI_SSID = "TU_SSID_WIFI";
const char* WIFI_PASSWORD = "TU_CONTRASEÑA_WIFI";

// Datos de conexión a Ubidots
const char* UBIDOTS_TOKEN = "TU_TOKEN_UBIDOTS";
const char* UBIDOTS_VARIABLE_ID = "TU_ID_VARIABLE_UBIDOTS";

// Pin al que está conectado el sensor de pH (A0 en este caso)
const int pH_SENSOR_PIN = A0;

// Objeto para conexión WiFi
WiFiClient client;

// Objeto para conexión con Ubidots
Ubidots ubidots(UBIDOTS_TOKEN, client);

void setup() {
  Serial.begin(9600);
  connectWiFi();
}

void loop() {
  float pHValue = readPH();
  Serial.print("Valor de pH: ");
  Serial.println(pHValue);

  // Envía los datos de pH a Ubidots
  ubidots.add(UBIDOTS_VARIABLE_ID, pHValue);
  ubidots.sendAll();
  delay(5000); // Envía los datos cada 5 segundos (ajusta este valor según tus necesidades)
}

void connectWiFi() {
  Serial.print("Conectando a WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado a WiFi");
}

float readPH() {
  int sensorValue = analogRead(pH_SENSOR_PIN);
  // Realiza aquí la conversión del valor analógico a pH, dependiendo de las especificaciones del sensor PH4502C
  // La conversión puede variar según el sensor y el circuito utilizado, así que asegúrate de consultar la hoja de datos del sensor y realizar los cálculos adecuados.

  // Por ejemplo, si el sensor de pH produce un valor analógico entre 0 y 1023 y tiene una sensibilidad de 5.0 pH por cada 1000 unidades ADC,
  // la fórmula para convertir el valor sería:
  // float pHValue = map(sensorValue, 0, 1023, 0, 1400) / 100.0; // 0 a 14 pH

  // Devolver el valor de pH
  return pHValue;
}
