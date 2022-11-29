//#include <UIPEthernet.h>
#include "PubSubClient.h"
#include "EthernetENC.h"

#define CLIENT_ID "UnoMQTT"
#define INTERVAL 1000 // 3 sec delay between publishing

int lichtstatus;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ipfixed[] = {192, 168, 0, 2};
EthernetClient ethClient;
PubSubClient mqttClient;

long previousMillis;

void setup()
{
  // setup serial communication
  //  Serial.begin(57600);
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
    sendData();
    previousMillis = millis();
  }
}

void sendData()
{
  if (mqttClient.connect(CLIENT_ID))
  {
    mqttClient.publish("hal/temp", "dupa");
  }
}
