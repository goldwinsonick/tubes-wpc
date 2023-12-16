#define sensorPowerPin 13
#define sensorReadPin 34
#define waterThreshold 420 
#define buzzerPin 23

// Wifi Setups
#include <WiFi.h>
#define WIFI_SSID "WALEMOS"
#define WIFI_PASS "WALEMOS123"
#define WIFI_TIMEOUT 10000 // ms
void setupWifi(){
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting...");
  int startTime = millis();
  while(WiFi.status() != WL_CONNECTED and (millis()-startTime) < WIFI_TIMEOUT){
    Serial.print(".");
    delay(500);
  }
  if(WiFi.status() == WL_CONNECTED){
    Serial.println("\nConnected to WiFi: ");
    Serial.println(WiFi.localIP());
    Serial.println();
  }else{
    Serial.println("Can't connect");
  }
}

//Firebase and Firebase Configs
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"
#define API_KEY "AIzaSyBuzmdZ8ksbPnLysv3nCy5trnQ1rvJe3qQ"
#define DATABASE_URL "https://walemos-wpc-default-rtdb.firebaseio.com/"
FirebaseData fbdo; // Firebase Data Object
FirebaseAuth auth; // Firebase Auth Object
FirebaseConfig config;
unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
void setupFirebase(){
  // Configs
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.println("signUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

bool getWater(){
  digitalWrite(sensorPowerPin, HIGH);
  bool flag;
  int outSensor = analogRead(sensorReadPin);
  Serial.println(outSensor);
  if(outSensor > waterThreshold){
    flag = true;
  }else{
    flag = false;
  }
  digitalWrite(sensorPowerPin, LOW);
  return flag;
}


void setup() {
  Serial.begin(9600);
  pinMode(buzzerPin, OUTPUT);
  analogWrite(buzzerPin, 0);
  pinMode(sensorPowerPin, OUTPUT);
  pinMode(sensorReadPin, INPUT);
  setupWifi();
  setupFirebase();
}
bool waterStatus = false;

FirebaseJson dataJson;
void sendData(){
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();

    // Store data to Realtime Database
    dataJson.set("Ts/.sv", "timestamp");
    if(waterStatus){
      if(Firebase.RTDB.setJSON(&fbdo, "/data/lastOn", &dataJson)){
        Serial.println("json1 sent");
      }else{
        Serial.println("FAILED: " + fbdo.errorReason());
      }
    }else{
      if(Firebase.RTDB.setJSON(&fbdo, "/data/lastOff", &dataJson)){
        Serial.println("json2 sent");
      }else{
        Serial.println("FAILED: " + fbdo.errorReason());
      }
    }
  }
}

void loop() {
  // Water Sensor
  waterStatus = getWater();
  Serial.println(waterStatus);
  if(waterStatus){
    analogWrite(buzzerPin, 50);
  }else{
    analogWrite(buzzerPin, 0);
  }

  // Update Firebase
  sendData();

  delay(500);
}
