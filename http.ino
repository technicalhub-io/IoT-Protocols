
//                                                              HTTP PROTOCOL EXAMPLE
#include <FS.h>    
//for to connect to wifi.
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>          
#include <DNSServer.h>
#include <WiFiManager.h>          //https://github.com/tzapu/WiFiManager

WiFiClient espclient;
const char* ssid = "JARVIS";
const char* password = "111111111";

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance


HTTPClient http;
const char* http_server = "sriki007.pythonanywhere.com";

void setup_wifi()
{
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void upload_http(float data)
{    
    //for http server.
    if (espclient.connect(http_server,80))
     {      
     espclient.print(String("GET /upload_pc_data?power="+String(data * 230)+"&"+"current="+String(data)) + " HTTP/1.1\r\n" +"Host: " + http_server + "\r\n" + "Connection: close\r\n\r\n");
     }
     else
     {
      Serial.println("There was a problem in connecting to server... pls check once.");
     }
     espclient.stop();
     delay(10000);//for data uploading.
} 

void setup()
{ 
  Serial.begin(9600);
  setup_wifi();
  emon1.current(A0, 111.1);             // Current: input pin, calibration.
}
void loop()
{
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  upload_http(Irms);
  delay(2000);
}
