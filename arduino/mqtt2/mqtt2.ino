//#include <UIPEthernet.h>
#include "PubSubClient.h"
#include "EthernetENC.h"
#include <OneWire.h>
#include <DallasTemperature.h>
#include <Wire.h>
#include <ADXL345.h>
#include <string.h>

#define CLIENT_ID "UnoMQTT"
#define INTERVAL 1000 // 3 sec delay between publishing
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
float last_send_temp=9999;
int lichtstatus;
byte mac[] = {0xDE, 0xAD, 0xC0, 0xDE, 0xDE, 0xAD};
byte ipfixed[] = {172,20,98,226};
EthernetClient ethClient;
PubSubClient mqttClient;

long previousMillis;

void setup()
{ 
  // Inicjacja czujnika
  sensors.begin();
  // setup serial communication
    Serial.begin(9600);
  Ethernet.begin(mac, ipfixed);

  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer("172.20.98.222", 1883);
  previousMillis = millis();

 
}

void loop()
{
  if (millis() - previousMillis > INTERVAL)
  {
    sensors.requestTemperatures();
    float temp = sensors.getTempCByIndex(0);
    if(temp != last_send_temp){
      last_send_temp = temp;
      Serial.println(temp);
      sendData(temp);
    }else{
     Serial.println("not sending");      
    }
    previousMillis = millis();
  }
}

void sendData(float temp)
{
  char msgBuffer_temp[20];

  if (mqttClient.connect(CLIENT_ID))
  {
    mqttClient.publish("iot/temperature2", dtostrf(temp, 6, 2, msgBuffer_temp));
    Serial.println("connected and published");
  }else{
    Serial.println("Can't connect");
  }
}
