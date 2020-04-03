//                                                              MQTT PROTOCOL EXAMPLE
#include <ESP8266WiFi.h>          
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>

WiFiClient espClient;
const char* ssid = "JARVIS";
const char* password = "111111111";

#include "EmonLib.h"                   // Include Emon Library
EnergyMonitor emon1;                   // Create an instance

#include <PubSubClient.h>     // MQTT module
PubSubClient client(espClient);
const char* mqtt_server = "mqtt.eclipse.org";
#define CLIENT_ID "currentSensor"

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

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    client.connect(CLIENT_ID);
  }
}

void setup()
{ 
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  emon1.current(A0, 111.1);             // Current: input pin, calibration.
}
void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  double Irms = emon1.calcIrms(1480);  // Calculate Irms only
  char* data;
  (String(Irms)+" A").toCharArray(data, 30);
  client.publish("sensorData", data);
  delay(2000);
}
