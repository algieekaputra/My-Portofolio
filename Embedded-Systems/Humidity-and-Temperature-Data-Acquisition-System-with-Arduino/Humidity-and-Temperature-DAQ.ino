#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <DHT.h>

LiquidCrystal_I2C lcd(0x27 ,16,2);
#define DHTPIN 2
#define DHTTYPE DHT22
#define led 12 
DHT dht(DHTPIN, DHTTYPE);
float h;
float t;

void ledblink()
{
  digitalWrite(led, HIGH);
  delay(500);
  digitalWrite(led, LOW);
  delay(500);
}
void memulai()
{
  Serial.println("Memulai Pengambilan Data..."); 
  dht.begin();
  lcd.init();                      
  lcd.backlight();
  lcd.setCursor(2,0);
  lcd.print("Humidity DAQ");
  lcd.setCursor(3,1);
  lcd.print("Kelompok 1");
  delay(4000);
  lcd.clear();
}
void gagalmengambil()
{
    Serial.println("GAGAL MENGAMBIL DATA");
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("  SISTEM GAGAL ");
    lcd.setCursor(0,1);
    lcd.print(" MENGAMBIL DATA ");
    ledblink();
}
void pengambilandata()
{
    lcd.setCursor(0,0);
    lcd.print("  Hum : ");
    lcd.print(h);
    lcd.print("%  ");
    lcd.setCursor(0,1);
    if(h < 75)
    {
      lcd.print("     SEHAT      ");
    }
    else
    {
      lcd.print("  TIDAK SEHAT  ");
    }--
    Serial.print(",");
    Serial.print(h);
    Serial.print(",");
    Serial.print(t);
    Serial.print("\n");
    delay(100);
    digitalWrite(led, HIGH);
}
void setup() {
  pinMode(led, OUTPUT);
  Serial.begin(9600);
  memulai();
}
void loop() {
  h = dht.readHumidity();
  t = dht.readTemperature();
  if (isnan(h)) {
    gagalmengambil();
  } 
  else {
    pengambilandata();
  }
}
