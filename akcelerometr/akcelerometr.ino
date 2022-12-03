#include <Wire.h>
#include <ADXL345.h>
 
ADXL345 accelerometer;
 
void showRange(void)
{
  Serial.print("Wybrany zakres pomiarowy: ");
 
  switch(accelerometer.getRange())
  {
    case ADXL345_RANGE_16G: Serial.println("+/- 16 g"); break;
    case ADXL345_RANGE_8G:  Serial.println("+/- 8 g");  break;
    case ADXL345_RANGE_4G:  Serial.println("+/- 4 g");  break;
    case ADXL345_RANGE_2G:  Serial.println("+/- 2 g");  break;
    default: Serial.println("Bledny zakres"); break;
  }
}
 
void showDataRate(void)
{
  Serial.print("Wybrana szybkosc transmisji: ");
 
  switch(accelerometer.getDataRate())
  {
    case ADXL345_DATARATE_3200HZ: Serial.println("3200 Hz"); break;
    case ADXL345_DATARATE_1600HZ: Serial.println("1600 Hz"); break;
    case ADXL345_DATARATE_800HZ:  Serial.println("800 Hz");  break;
    case ADXL345_DATARATE_400HZ:  Serial.println("400 Hz");  break;
    case ADXL345_DATARATE_200HZ:  Serial.println("200 Hz");  break;
    case ADXL345_DATARATE_100HZ:  Serial.println("100 Hz");  break;
    case ADXL345_DATARATE_50HZ:   Serial.println("50 Hz");   break;
    case ADXL345_DATARATE_25HZ:   Serial.println("25 Hz");   break;
    case ADXL345_DATARATE_12_5HZ: Serial.println("12.5 Hz"); break;
    case ADXL345_DATARATE_6_25HZ: Serial.println("6.25 Hz"); break;
    case ADXL345_DATARATE_3_13HZ: Serial.println("3.13 Hz"); break;
    case ADXL345_DATARATE_1_56HZ: Serial.println("1.56 Hz"); break;
    case ADXL345_DATARATE_0_78HZ: Serial.println("0.78 Hz"); break;
    case ADXL345_DATARATE_0_39HZ: Serial.println("0.39 Hz"); break;
    case ADXL345_DATARATE_0_20HZ: Serial.println("0.20 Hz"); break;
    case ADXL345_DATARATE_0_10HZ: Serial.println("0.10 Hz"); break;
    default: Serial.println("Bleda szybkosc transmisji"); break;
  }
}
 
void setup(void)
{
  Serial.begin(9600);
 
  // Inicjalizacja ADXL345
  Serial.println("Inicjalizacja ADXL345");
  if (!accelerometer.begin())
  {
    Serial.println("Nie odnaleziono ADXL345, sprawdz podlaczenie!");
    delay(500);
  }
 
  // Wybor zakresu pomiarowego
  // +/-  2G: ADXL345_RANGE_2G
  // +/-  4G: ADXL345_RANGE_4G
  // +/-  8G: ADXL345_RANGE_8G
  // +/- 16G: ADXL345_RANGE_16G
  accelerometer.setRange(ADXL345_RANGE_16G);
 
  // Wyswietlenie aktualnych parametrow
  showRange();
  showDataRate();
}
 
void loop(void)
{
  // Odczyt wartosci surowych
  Vector raw = accelerometer.readRaw();
 
  // Odczyt wartosci znormalizowanych
  Vector norm = accelerometer.readNormalize();
 
  // Wyswietlenie danych surowych
  Serial.print(" Xraw = ");
  Serial.print(raw.XAxis);
  Serial.print(" Yraw = ");
  Serial.print(raw.YAxis);
  Serial.print(" Zraw: ");
  Serial.print(raw.ZAxis);
 
  // Wyswietlenie danych znormalizowanych m/s^2
  Serial.print(" Xnorm = ");
  Serial.print(norm.XAxis);
  Serial.print(" Ynorm = ");
  Serial.print(norm.YAxis);
  Serial.print(" Znorm = ");
  Serial.print(norm.ZAxis);
 
  Serial.println();
 
  delay(200);
}
