#define sensorPowerPin 13
#define sensorReadPin 12
#define threshold 420 
#define buzzerPin 23
// #include <WiFi.h>
// #define WIFI_SSID ""
// #define WIFI_PASS ""
// #define WIFI_TIMEOUT 10000 // ms
// void setupWifi(){
//   WiFi.begin(WIFI_SSID, WIFI_PASS);
//   Serial.print("Connecting...");
//   startTime = millis()
//   while(WiFi.status() != WL_CONNECTED and (millis()-startTime) < WIFI_TIMEOUT){
//     Serial.print(".");
//     delay(500);
//   }
//   if(WiFi.status() == WL_CONNECTED){
//     Serial.println("\nConnected to WiFi: ");
//     Serial.println(WiFi.localIP());
//     Serial.println();
//   }else{
//     Serial.println("Can't connect");
//   }
// }
// void setupWifiManager(){
//   // 
// }
// void setupFirebase(){
//   // 
// }

bool getWater(){
  digitalWrite(sensorPowerPin, HIGH);
  bool flag;
  int outSensor = analogRead(sensorReadPin);
  Serial.println(outSensor);
  if(outSensor > threshold){
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
}
bool waterStatus = false;
void loop() {
  waterStatus = getWater();
  Serial.println(waterStatus);
  if(waterStatus){
    analogWrite(buzzerPin, 50);
  }else{
    analogWrite(buzzerPin, 0);
  }
  delay(500);
}
