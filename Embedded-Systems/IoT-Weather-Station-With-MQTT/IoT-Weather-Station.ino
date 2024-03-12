//rain sensor -> max = kering, min = basah
//level sensor -> 0 - 750, 0 = kosong , 750 = full
//OLED -> SCL = D22, SDA = D21 //Address OLED 0x3C
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFi.h>
#include <PubSubClient.h>     // Library untuk mengkoneksikan ESP32 ke MQTT broker            /
#include <ArduinoJson.h>
#include <DHT.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//pin pin sensor
#define rain 34
#define ldr 33
#define DHTPIN 27 
#define DHTTYPE DHT22  
DHT dht(DHTPIN, DHTTYPE);
#define led 2
#define buzz 4
//variabel variabel
int thres_rain = 45;
int thres_light = 1500;

char* ssid = "algianya";
char* password = "4131772002";

// MQTT Setting
char *mqttServer = "192.168.1.15";
int mqttPort = 1883;
String myClientID = "Edspert_device_01";
String mqttPassword = "1234";
String mqttUser = "Device_01";
String tempTopic = "edspert/data";

WiFiClient wifiClient;                // Membuat objek wifiClient
PubSubClient mqttClient(wifiClient);

StaticJsonDocument<200> doc;
String jsonString,jsonPayload;


void init_display()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 10); //0,10
  display.println("IoT Weather Station");
  display.setCursor(15, 50); //0,10
  display.println("By Algi Ekaputra");
  display.display();
  delay(3000);
  display.clearDisplay();
}
void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
  Serial.begin(115200);
  connect_to_AP(ssid, password);
  init_to_broker(mqttServer,mqttPort); // setup koneksi ke broker
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) // Address 0x3D for 128x64
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
//  delay(2000);
  pinMode(rain, INPUT);
  pinMode(ldr, INPUT);
  dht.begin();
  pinMode(led, OUTPUT);
  pinMode(buzz, OUTPUT);
  init_display();
}

void loop() {
  int rain_val = map(analogRead(rain), 0, 4095, 100, 0);
  int light = analogRead(ldr);
  int temp = dht.readTemperature();
  int hum = dht.readHumidity();
  
  if (!mqttClient.connected()){
      connect_to_broker(myClientID, mqttUser, mqttPassword);   // Try to connect with broker
    }
    else
    {
      send_and_display(rain_val, light, temp, hum);
    } 
}
void connect_to_AP(char* mySSID, char* myPASS){
  WiFi.mode(WIFI_STA);
  WiFi.begin(mySSID, myPASS);   // Mencoba connect ke Wifi
  // Melakukan pengecekan terhadap status koneksi ke WI-Fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  } 
  Serial.println("");
  Serial.print("Device telah terhubung ke: ");
  Serial.println(mySSID);
  Serial.print("IP Device: ");
  Serial.println(WiFi.localIP());
}

void init_to_broker(char* myMqttServer, int myMqttPort) {
  mqttClient.setServer(myMqttServer, myMqttPort); // Mengatur detail broker target  yang digunakan
  // jika kita ingin menerima pesan untuk langganan yang dibuat oleh klien sebagai subscriber
  // mqttClient.setCallback(callback);           
}

// untuk melakukan koneksi ulang (reconnect) ke broker MQTT 
// jika klien kehilangan koneksi dengan broker 
// atau gagal melakukan koneksi saat pertama kali menjalankan program.
// ESP32 Reconnect to broker
void connect_to_broker(String clientName, String userMQTT , String passwordMQTT) {
  if (!mqttClient.connected()){
    Serial.println("Connecting to MQTT Broker...");
    String clientId = "ESP32Client-" + clientName;
    mqttClient.connect(clientId.c_str(), userMQTT.c_str(), passwordMQTT.c_str());
    while (!mqttClient.connected()) {
        Serial.print("failed, rc=");
        Serial.println(mqttClient.state());
        Serial.println("Reconnecting to MQTT Broker..");
        delay(1000);  
    }
    Serial.print(clientId);
    Serial.println(" Device to Broker Connected.");
  }
  else{  }
}

// Publish untuk Tipe data JSon dengan inputan Payload Float
void mqtt_publish(String yourTopic, float yourPayload){
  
  // Tambahkan data "temp" ke objek JSON
  doc["data"] = yourPayload;

  // Serializasikan objek JSON ke dalam string JSON
  serializeJson(doc, jsonString);
  
  // char yourPayloadString[8];
  // dtostrf(yourPayload, 1, 2, yourPayloadString);
  // mqttClient.publish(yourTopic.c_str(), yourPayloadString);
  mqttClient.publish(yourTopic.c_str(), jsonString.c_str());
  Serial.print(yourTopic);
  Serial.print(" ==> ");
  Serial.println(yourPayload);
  // Serial.println(jsonString);
  jsonString = "";    // Riset Payload JsonString
}

// Publish untuk Tipe data JSon dengan inputan Payload JSON
void mqtt_publish_json(String yourTopic, String yourPayload){
   
  // char yourPayloadString[8];
  // dtostrf(yourPayload, 1, 2, yourPayloadString);
  // mqttClient.publish(yourTopic.c_str(), yourPayloadString);
  mqttClient.publish(yourTopic.c_str(), yourPayload.c_str());
  Serial.print(yourTopic);
  Serial.print(" ==> ");
  Serial.println(yourPayload);
  // Serial.println(jsonString);
  // yourPayload = "";    // Riset Payload JsonString
}

void send_and_display(int a, int b, int c, int d)
{
      doc["rain"] = a;
      doc["light"] = b;
      doc["temp"] = c;
      doc["hum"] = d;
      digitalWrite(buzz, LOW);
      display.setTextSize(1);
      display.setTextColor(WHITE);
      
      display.setCursor(0, 0); //0,10
      display.println("Rain(%) : ");
      
      display.setCursor(55, 0); 
      display.println(a); //rain
    
      display.setCursor(0, 10); 
      display.println("Light   : ");
      
      display.setCursor(55, 10); 
      display.println(b); //light 
    
      display.setCursor(0, 20); 
      display.println("Temp(C) : ");
      
      display.setCursor(55, 20); 
      display.println(c); //temp
    
      display.setCursor(0, 30); 
      display.println("Hum(%)  : ");
      
      display.setCursor(55, 30); 
      display.println(d); 
      
      display.setCursor(40, 40); 
      display.println("Status : ");
      
      display.setTextSize(2);
      display.setCursor(40, 50);
      if(a >= thres_rain)
      {
        doc["stat"] = "HUJAN";
        display.println("HUJAN");
        digitalWrite(led, HIGH);
      }
      else if(b < thres_light)
      {
        doc["stat"] = "MENDUNG/GELAP";
        display.setTextSize(1);
        display.setCursor(30, 50);
        display.println("MENDUNG/GELAP");
        digitalWrite(led, LOW);
      }
      else
      {
        doc["stat"] = "CERAH";
        display.println("CERAH");
        digitalWrite(led, LOW);
        digitalWrite(buzz, LOW);
      }
      display.display();
      display.clearDisplay();
      delay(1000); 
      // Serializasikan objek JSON ke dalam string JSON
      jsonPayload = "";    // Riset Payload jsonPayload
      serializeJson(doc, jsonPayload);
      // Serial.println(jsonPayload);
      mqtt_publish_json(tempTopic,jsonPayload);
}
