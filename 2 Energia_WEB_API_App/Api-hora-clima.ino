#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h>
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
  GetTime();
  clima();
  delay(10000);
}
void GetTime()
{
  //{"abbreviation":"-05","client_ip":"157.100.92.112","datetime":"2022-05-31T20:28:35.577230-05:00","day_of_week":2,"day_of_year":151,"dst":false,"dst_from":null,"dst_offset":0,"dst_until":null,"raw_offset":-18000,"timezone":"America/Guayaquil","unixtime":1654046915,"utc_datetime":"2022-06-01T01:28:35.577230+00:00","utc_offset":"-05:00","week_number":22}


  WiFiClient client;
  HTTPClient http;
  String fecha = "";
  String hora = "";
  String dia_semana = "";
  String dia_anio = "";
  String zona = "";
  String semana = "";
  
  http.begin(client,"http://worldtimeapi.org/api/timezone/America/Guayaquil");
  int httpCode = http.GET();
  if(httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY)
  {
    String payload = http.getString();
    
    int beginS = payload.indexOf("datetime");//posicion donde escuentra el string
    int endS = payload.indexOf("day_of_week");//posicion donde escuentra el string
    Serial.println(payload);
    Serial.println();
    
    fecha = payload.substring(beginS +11,endS-25);//recorto el string
    Serial.print("Fecha: ");
    Serial.println(fecha);
    hora = payload.substring(beginS +22,endS-16);//recorto el string
    Serial.print("Hora: ");
    Serial.println(hora);

    beginS = payload.indexOf("day_of_week");//posicion donde escuentra el string
    endS = payload.indexOf("day_of_year");//posicion donde escuentra el string  

    dia_semana = payload.substring(beginS +13,endS-2);//recorto el string
    Serial.print("Dia de la semana: ");
    Serial.println(dia_semana);

    beginS = payload.indexOf("day_of_year");//posicion donde escuentra el string
    endS = payload.indexOf("dst");//posicion donde escuentra el string  

    dia_anio = payload.substring(beginS +13,endS-2);//recorto el string
    Serial.print("Dia de anio: ");
    Serial.println(dia_anio);

    beginS = payload.indexOf("timezone");//posicion donde escuentra el string
    endS = payload.indexOf("unixtime");//posicion donde escuentra el string  

    zona = payload.substring(beginS +11,endS-3);//recorto el string
    Serial.print("Zona: ");
    Serial.println(zona);

    beginS = payload.indexOf("week_number");//posicion donde escuentra el string
    endS = payload.indexOf("week_number");//posicion donde escuentra el string  

    semana = payload.substring(beginS +13,endS+15);//recorto el string
    Serial.print("No semana: ");
    Serial.println(semana);
    
  }
}

/////////////////////////////////////////////////////////////////////////////////
void clima()
{
    //{"lat":-2.225,"lon":-79.9471,"timezone":"America/Guayaquil","timezone_offset":-18000,
    //"daily":[{"dt":1654016400,"sunrise":1653995860,"sunset":1654039059,"moonrise":1653999000,"moonset":1654043100,"moon_phase":0.04,"temp":
    //{"day":28.3,"min":19.85,"max":31.42,"night":22.99,"eve":25.93,"morn":20.58},
    //"feels_like":{"day":29.78,"night":23.49,"eve":26.48,"morn":20.99},"pressure":1013,"humidity":59,"dew_point":19.41,"wind_speed":3.84,"wind_deg":231,"wind_gust":7.08,
    //"weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],
    //"clouds":95,"pop":0.08,"uvi":9.66},{"dt":1654102800,"sunrise":1654082270,"sunset":1654125467,"moonrise":1654088460,"moonset":1654132500,"moon_phase":0.07,
    //"temp":{"day":29.59,"min":20.1,"max":31.5,"night":21.26,"eve":25.61,"morn":20.26},"feels_like":{"day":30.6,"night":21.56,"eve":25.92,"morn":20.59},"pressure":1011,
    //"humidity":51,"dew_point":18.26,"wind_speed":4.69,"wind_deg":241,"wind_gust":7.74,"weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],
    //"clouds":100,"pop":0.11,"uvi":5.94},{"dt":1654189200,"sunrise":1654168680,"sunset":1654211875,"moonrise":1654177860,"moonset":1654221960,"moon_phase":0.1,"temp":{"day":29.6,"min":20.47,"max":31.01,"night":22.01,"eve":25.79,"morn":20.49},"feels_like":{"day":31.07,"night":22.36,"eve":26.15,"morn":20.79},"pressure":1011,"humidity":54,"dew_point":19.06,"wind_speed":2.94,"wind_deg":225,"wind_gust":5.28,"weather":[{"id":804,"main":"Clouds","description":"overcast clouds","icon":"04d"}],"clouds":100,"pop":0.15,"uvi":10.28},{"dt":1654275600,"sunrise":1654255091,"sunset":1654298283,"moonrise":1654267260,"moonset":1654311360,"moon_phase":0.13,"temp":{"day":30.29,"min":21.16,"max":30.96,"night":22.05,"eve":24.2,"morn":21.16},"feels_like":{"day":32.19,"night":22.5,"eve":24.61,"morn":21.5},"pressure":1012,"humidity":54,"dew_point":19.88,"wind_speed":2.84,"wind_deg":216,"wind_gust":5.44,"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10d"}],"clouds":95,"pop":0.56,"rain":0.55,"uvi":10.94},{"dt":1654362000,"sunrise":1654341502,"sunset":1654384692,"moonrise":1654356540,"moonset":1654400640,"moon_phase":0.16,"temp":{"day":31.97,"min":20.94,"max":31.97,"night":21.9,"eve":22.82,"morn":20.96},"feels_like":{"day":34.07,"night":22.52,"eve":23.51,"morn":21.38},"pressure":1011,"humidity":49,"dew_point":19.91,"wind_speed":2.17,"wind_deg":204,"wind_gust":4.65,"weather":[{"id":501,"main":"Rain","description":"moderate rain","icon":"10d"}],"clouds":75,"pop":0.83,"rain":6.38,"uvi":2.16},{"dt":1654448400,"sunrise":1654427913,"sunset":1654471101,"moonrise":1654445700,"moonset":1654489860,"moon_phase":0.19,"temp":{"day":30.28,"min":20.97,"max":30.28,"night":22.3,"eve":24.7,"morn":21.2},"feels_like":{"day":32.18,"night":22.75,"eve":25.13,"morn":21.86},"pressure":1010,"humidity":54,"dew_point":19.95,"wind_speed":3.23,"wind_deg":252,"wind_gust":6.38,"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10d"}],"clouds":92,"pop":0.32,"rain":0.5,"uvi":0.14},{"dt":1654534800,"sunrise":1654514325,"sunset":1654557510,"moonrise":1654534740,"moonset":0,"moon_phase":0.22,"temp":{"day":28.27,"min":21.76,"max":28.53,"night":22.85,"eve":23.77,"morn":21.89},"feels_like":{"day":30.23,"night":23.38,"eve":24.32,"morn":22.33},"pressure":1011,"humidity":63,"dew_point":20.43,"wind_speed":2.28,"wind_deg":221,"wind_gust":4.49,"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10d"}],"clouds":98,"pop":0.35,"rain":0.3,"uvi":1},{"dt":1654621200,"sunrise":1654600736,"sunset":1654643920,"moonrise":1654623720,"moonset":1654578900,"moon_phase":0.25,"temp":{"day":28.86,"min":21.64,"max":28.86,"night":21.93,"eve":23.45,"morn":21.78},"feels_like":{"day":31.1,"night":22.37,"eve":23.89,"morn":22.29},"pressure":1010,"humidity":62,"dew_point":20.66,"wind_speed":3.64,"wind_deg":235,"wind_gust":6.55,"weather":[{"id":500,"main":"Rain","description":"light rain","icon":"10d"}],"clouds":97,"pop":0.34,"rain":0.34,"uvi":1}]}

    String latitud = "";
    String longitud = "";
    String zona2 = "";
    String fase_lunar = "";
    String temp_dia = "";
    String temp_min = "";
    String temp_max = "";
    String temp_noche = "";
    String temp_tarde = "";
    String temp_manana = "";
    String presion = "";
    String humedad = "";
    String velocidad_viento = "";
    String direccion_viento = "";
    String rafagas = "";
    String main = "";
    String uvi = "";  
    std::unique_ptr<BearSSL::WiFiClientSecure>client(new BearSSL::WiFiClientSecure);

    //client->setFingerprint(fingerprint);
    // Or, if you happy to ignore the SSL certificate, then use the following line instead:
     client->setInsecure();

    HTTPClient https;

    Serial.print("[HTTPS] begin...\n");
    //https://api.openweathermap.org/data/2.5/onecall?lat=-2.2249894&lon=-79.9470905&exclude=current,hourly,daily&appid=b31202feae8372ee7629d14cda746975&units=metric
    //if (https.begin(*client, "https://jigsaw.w3.org/HTTP/connection.html")) {  // HTTPS
    //pendiente
    //https://openweathermap.org/api/air-pollution
    if (https.begin(*client, "https://api.openweathermap.org/data/2.5/onecall?lat=-2.2249894&lon=-79.9470905&exclude=current,hourly&appid=b31202feae8372ee7629d14cda746975&units=metric")) 
    {  // HTTPS  

      Serial.print("[HTTPS] GET...\n");
      // start connection and send HTTP header
      int httpCode = https.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = https.getString();
          Serial.println(payload);
          Serial.println();
          int beginS = payload.indexOf("lat");//posicion donde escuentra el string
          int endS = payload.indexOf("lon");//posicion donde escuentra el string  
          latitud = payload.substring(beginS +5,endS-2);//recorto el string
          Serial.print("Latitud: ");
          Serial.println(latitud);

          beginS = payload.indexOf("lon");//posicion donde escuentra el string
          endS = payload.indexOf("timezone");//posicion donde escuentra el string  
          longitud = payload.substring(beginS +5,endS-2);//recorto el string
          Serial.print("Longitud: ");
          Serial.println(longitud);

          beginS = payload.indexOf("timezone");//posicion donde escuentra el string
          endS = payload.indexOf("timezone_offset");//posicion donde escuentra el string  
          zona2 = payload.substring(beginS +11,endS-3);//recorto el string
          Serial.print("Zona: ");
          Serial.println(zona2);
          
          beginS = payload.indexOf("moon_phase");//posicion donde escuentra el string
          endS = payload.indexOf("temp");//posicion donde escuentra el string  
          fase_lunar = payload.substring(beginS +1,endS-3);//recorto el string
          Serial.print("Fase_lunar: ");
          Serial.println(fase_lunar);

          beginS = payload.indexOf("day");//posicion donde escuentra el string
          endS = payload.indexOf("min");//posicion donde escuentra el string  
          temp_dia = payload.substring(beginS +5,endS-2);//recorto el string
          Serial.print("Temp_del_dia: ");
          Serial.println(temp_dia);

          beginS = payload.indexOf("min");//posicion donde escuentra el string
          endS = payload.indexOf("max");//posicion donde escuentra el string  
          temp_min = payload.substring(beginS +5,endS-2);//recorto el string
          Serial.print("Temp_min: ");
          Serial.println(temp_min);

          beginS = payload.indexOf("max");//posicion donde escuentra el string
          endS = payload.indexOf("night");//posicion donde escuentra el string  
          temp_max = payload.substring(beginS +5,endS-2);//recorto el string
          Serial.print("Temp_max: ");
          Serial.println(temp_max);

          beginS = payload.indexOf("night");//posicion donde escuentra el string
          endS = payload.indexOf("eve");//posicion donde escuentra el string  
          temp_noche = payload.substring(beginS +7,endS-2);//recorto el string
          Serial.print("Temp_noche: ");
          Serial.println(temp_noche);
          
          beginS = payload.indexOf("eve");//posicion donde escuentra el string
          endS = payload.indexOf("morn");//posicion donde escuentra el string  
          temp_tarde = payload.substring(beginS +5,endS-2);//recorto el string
          Serial.print("Temp_tarde: ");
          Serial.println(temp_tarde);
         
           beginS = payload.indexOf("morn");//posicion donde escuentra el string
          endS = payload.indexOf("feels");//posicion donde escuentra el string  
          temp_manana = payload.substring(beginS +6,endS-3);//recorto el string
          Serial.print("Temp_manana: ");
          Serial.println(temp_manana);
          
           beginS = payload.indexOf("pressure");//posicion donde escuentra el string
          endS = payload.indexOf("humidity");//posicion donde escuentra el string  
          presion = payload.substring(beginS +10,endS-2);//recorto el string
          Serial.print("Presion: ");
          Serial.println(presion);
          
           beginS = payload.indexOf("humidity");//posicion donde escuentra el string
          endS = payload.indexOf("dew_point");//posicion donde escuentra el string  
          humedad = payload.substring(beginS +10,endS-2);//recorto el string
          Serial.print("Humedad: ");
          Serial.println(humedad);
       
           beginS = payload.indexOf("wind_speed");//posicion donde escuentra el string
          endS = payload.indexOf("wind_deg");//posicion donde escuentra el string  
          velocidad_viento = payload.substring(beginS +12,endS-2);//recorto el string
          Serial.print("Velocidad de viento: ");
          Serial.println(velocidad_viento);
          
           beginS = payload.indexOf("wind_deg");//posicion donde escuentra el string
          endS = payload.indexOf("wind_gust");//posicion donde escuentra el string  
          direccion_viento = payload.substring(beginS +10,endS-2);//recorto el string
          Serial.print("Direccion de viento: ");
          Serial.println(direccion_viento);
          
           beginS = payload.indexOf("wind_gust");//posicion donde escuentra el string
          endS = payload.indexOf("weather");//posicion donde escuentra el string  
          rafagas = payload.substring(beginS +11,endS-2);//recorto el string
          Serial.print("Rafagas de viento: ");
          Serial.println(rafagas);
          
           beginS = payload.indexOf("main");//posicion donde escuentra el string
          endS = payload.indexOf("description");//posicion donde escuentra el string  
          main = payload.substring(beginS +7,endS-3);//recorto el string
          Serial.print("Clima: ");
          Serial.println(main);
         
           beginS = payload.indexOf("uvi");//posicion donde escuentra el string
          endS = payload.indexOf("dt");//posicion donde escuentra el string  
          uvi = payload.substring(beginS +5,endS-4);//recorto el string
          Serial.print("UVI: ");
          Serial.println(uvi);
        }
      } else {
        Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
      }
      https.end();
    } else {
      Serial.printf("[HTTPS] Unable to connect\n");
    }
}
