//#include <UIPEthernet.h>
#include "PubSubClient.h"
#include "EthernetENC.h"
#include <OneWire.h>
#include <DallasTemperature.h>

#define CLIENT_ID "UnoMQTT"
#define INTERVAL 1000 // 3 sec delay between publishing
#define ONE_WIRE_BUS 2
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int lichtstatus;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ipfixed[] = {192, 168, 0, 2};
EthernetClient ethClient;
PubSubClient mqttClient;

long previousMillis;

void setup()
{
  // Inicjacja czujnika
  sensors.begin();
  // setup serial communication
    Serial.begin(57600);
  Ethernet.begin(mac, ipfixed);

  // setup mqtt client
  mqttClient.setClient(ethClient);
  mqttClient.setServer("192.168.0.164", 1883);
  previousMillis = millis();
  Serial.println("now");
}

void loop()
{
  if (millis() - previousMillis > INTERVAL)
  {
    sensors.requestTemperatures();
    sendData(sensors.getTempCByIndex(0));
    previousMillis = millis();
  }
}

void sendData(float temp)
{
  char msgBuffer[20];
  if (mqttClient.connect(CLIENT_ID))
  {
    mqttClient.publish("iot/temperature", dtostrf(temp, 6, 2, msgBuffer));
    Serial.println("connected and published");
  }
}
