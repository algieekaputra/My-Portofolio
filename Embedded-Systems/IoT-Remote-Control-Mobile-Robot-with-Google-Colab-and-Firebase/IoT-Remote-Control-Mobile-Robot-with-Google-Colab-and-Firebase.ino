#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>

//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "PAY ME 100k FOR HOTSPOT" //EIRRG H118 // PAY ME 100k FOR HOTSPOT
#define WIFI_PASSWORD "swglord16" // modaldong // swglord16

// Insert Firebase project API Key
#define API_KEY "AIzaSyCy56yyIjPnEG5Q-m03eizK06sJq3hogMA"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://esp32-integration-test-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool boolValue;
int intValue;
int speedValue;
bool signupOK = false;

int enA = 25;
int enB = 16;
int in1 = 26;
int in2 = 27;
int in3 = 5;
int in4 = 17;

int trigpin = 15; 
int echopin = 32; 
int waktu; 
int distance;

void(* resetFunc) (void) = 0; // create a standard reset function

void maju(int kecepatan)
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2,LOW);
  analogWrite(enA, kecepatan);
  digitalWrite(in3, HIGH);
  digitalWrite(in4,LOW);
  analogWrite(enB, kecepatan);
  
}

void mundur(int kecepatan_mundur)
{
  digitalWrite(in1, LOW);
  digitalWrite(in2,HIGH);
  analogWrite(enA, kecepatan_mundur);
  digitalWrite(in3, LOW);
  digitalWrite(in4,HIGH);
  analogWrite(enB, kecepatan_mundur);
  
}

void berhenti()
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  analogWrite(enA, 0);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  analogWrite(enB, 0);
}

void hadap_kiri(float kecepatan_hadapkiri)
{
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, kecepatan_hadapkiri);
  digitalWrite(in3, HIGH); //kanan
  digitalWrite(in4, LOW);
  analogWrite(enB, kecepatan_hadapkiri);
}

void hadap_kanan(float kecepatan_hadapkanan)
{
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, kecepatan_hadapkanan);
  digitalWrite(in3, LOW); //kanan
  digitalWrite(in4, HIGH);
  analogWrite(enB, kecepatan_hadapkanan);
}

void setup() {
  Serial.begin(115200);
  pinMode (trigpin, OUTPUT); 
  pinMode (echopin, INPUT); 
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("ok");
    signupOK = true;
  }
  else {
   // resetFunc();
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {

  //Pembacaan speed
  if (Firebase.ready() && signupOK) { 
   // sendDataPrevMillis = millis();
       if (Firebase.RTDB.getInt(&fbdo, "/OUTPUT/speed")) {
      if (fbdo.dataType() == "int") {
        speedValue = fbdo.intData();
      }
    }
    else {
      Serial.println(fbdo.errorReason());
    }
    //Pembacaan mode
    if (Firebase.RTDB.getInt(&fbdo, "/OUTPUT/command")) {
      if (fbdo.dataType() == "int") {
        intValue = fbdo.intData();        
          if(intValue == 1)
            {
              maju(speedValue);
              Serial.println("Maju");
              Serial.println(speedValue);
            }
            else if(intValue == 2)
            {
              hadap_kanan(120);
              delay(100);
              berhenti();
                //    delay(10000);
              Serial.println("Kanan");
              Serial.println(speedValue);
             }
            else if(intValue == 3)
            {
             hadap_kiri(120);
             delay(100);
             berhenti();
            // delay(10000);
             Serial.println("Kiri");
             Serial.println(speedValue);
            }
            else if(intValue == 4)
            {
              mundur((speedValue) - 10);
              Serial.println("berhenti");
              Serial.println(speedValue);
            }
            else
            {
              berhenti();
              Serial.println("Berhenti");
              Serial.println(speedValue);
            }
            delay(30);      
        if(millis() >= 12000) //tiap bbrp detik harus di reset, karena lost connection rawan terjadi setelah running beberapa detik
        {
          resetFunc();  // reset the Arduino via software function
        }
      }
    }
    else {
 //     resetFunc();
      Serial.println(fbdo.errorReason());
    }
  }
}
