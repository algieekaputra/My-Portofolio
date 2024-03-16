//Tech Trends Shameer
#define BLYNK_TEMPLATE_ID "TMPL6BaD_zBbC"
#define BLYNK_TEMPLATE_NAME "Hydro Thermometer"
#define BLYNK_AUTH_TOKEN "vXSbP-kWUfSNd-sbeOfZZFtpBEv2rNSx"
#define BLYNK_PRINT Serial
#define LEDB 2
#define LEDR 4
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "algianya";  // type your wifi name
char pass[] = "4131772002";  // type your wifi password


BlynkTimer timer;

#define DHTPIN 27 
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) 
  {
    Blynk.virtualWrite(V1, "Pengambilan Data Gagal, Periksa Sensor");
    Serial.println("Failed to read from DHT sensor!");
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, HIGH);
    delay(100);
    digitalWrite(LEDR, LOW);
    delay(100);
  }
  else {
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
    Blynk.virtualWrite(V1, "Pengambilan Data Sukses");
    digitalWrite(LEDR, LOW);
    digitalWrite(LEDB, HIGH);
    Blynk.virtualWrite(V5, t);
    Blynk.virtualWrite(V6, h);
    Serial.print("Temperature : ");
    Serial.print(t);
    Serial.print("    Humidity : ");
    Serial.println(h);
  }
}
void setup()
{   
  //DEBUG, PENTING!!!
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  dht.begin();
  timer.setInterval(100L, sendSensor);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDB, OUTPUT);
 
  }
void loop()
{
  Blynk.run();
  timer.run();
 }
