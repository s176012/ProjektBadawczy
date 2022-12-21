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
 
ADXL345 accelerometer;
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int lichtstatus;
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
byte ipfixed[] = {172,20,98,225};
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

  // Inicjalizacja ADXL345
  Serial.println("Inicjalizacja ADXL345");
  if (!accelerometer.begin())
  {
    Serial.println("Nie odnaleziono ADXL345, sprawdz podlaczenie!");
    delay(500);
  }
 
  accelerometer.setRange(ADXL345_RANGE_16G);
 
}

void loop()
{

  // Odczyt wartosci znormalizowanych m/s2
  Vector norm = accelerometer.readNormalize();
 
  if (millis() - previousMillis > INTERVAL)
  {
    sensors.requestTemperatures();
    sendData(sensors.getTempCByIndex(0), norm.XAxis, norm.YAxis, norm.ZAxis);
    previousMillis = millis();
  }
}

void sendData(float temp, float x, float y, float z)
{
  char msgBuffer_x[20];
  char msgBuffer_y[20];
  char msgBuffer_z[20];
  char msgBuffer_xyz[70];
  memset(msgBuffer_xyz, 0, sizeof(msgBuffer_xyz));
  
  char msgBuffer_temp[20];
  
  dtostrf(x, 1, 2, msgBuffer_x);
  dtostrf(y, 1, 2, msgBuffer_y);
  dtostrf(z, 1, 2, msgBuffer_z);
  
  int k =0;
  for(int i=0;i<20;i++){
    if(msgBuffer_x[i]){
      msgBuffer_xyz[i] = msgBuffer_x[i];
    }else{
      k = i;
      msgBuffer_xyz[k] = '|';
      k++;
      break;
    }
  }
  for(int i=k;i<20+k;i++){
    if(msgBuffer_y[i-k]){
      msgBuffer_xyz[i] = msgBuffer_y[i-k];
    }else{
      k = i;
      msgBuffer_xyz[k] = '|';
      k++;
      break;
    }
  }
  for(int i=k;i<20+k;i++){
    if(msgBuffer_z[i-k]){
      msgBuffer_xyz[i] = msgBuffer_z[i-k];
    }else{
      k = i;
      break;
    }
  }
  
  if (mqttClient.connect(CLIENT_ID))
  {
    //mqttClient.publish("iot/temperature", dtostrf(temp, 6, 2, msgBuffer_temp));
    mqttClient.publish("iot/acc", msgBuffer_xyz);
    Serial.println("connected and published");
  }else{
    Serial.println("Can't connect");
  }
}
