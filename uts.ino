#include "DHT.h"
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define DHTPIN 23
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
int t;
int lux;
int led = 22,led2 = 3,potPin=39,pin,pin2,pin3;
WidgetLED ledTemp(V7);
WidgetLED ledLight(V6);
BlynkTimer timer;

BLYNK_CONNECTED(){
  Blynk.syncAll();
}

BLYNK_WRITE(V4){
  pin = param.asInt();
}
BLYNK_WRITE(V5){
  pin2 = param.asInt();
}
BLYNK_WRITE(V8){
  pin3 = param.asInt();
}

void temp(){
  if (isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  Blynk.virtualWrite(V1, t);
  Blynk.virtualWrite(V3, t);
}

char auth[] = "Cc4iuJoaPPUrfeS8KLjHE7nTmzXY7gVA";
char ssid[] = "OKE";
char pass[] = "bergizitingg1";
void setup() {
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass, IPAddress(192,168,1,8), 8080);
  pinMode(led,OUTPUT);
  pinMode(led2,OUTPUT);
  dht.begin();

  timer.setInterval(2000L, temp);
}


void loop() {
  Blynk.run();
  timer.run();
  if(pin3 == 1){
  int readLight = analogRead(potPin);
  lux = (0.009768*readLight)+10;
  Blynk.virtualWrite(V0, lux);
  Blynk.virtualWrite(V2, lux);
  Serial.print(lux);
  t = dht.readTemperature();
 
 if(t >= pin2){
  digitalWrite(led,HIGH);
  ledTemp.on();
 }else{
  digitalWrite(led,LOW);
  ledTemp.off();
 }
 if(lux >= pin){
  digitalWrite(led2,HIGH);
  ledLight.on();
 }else{
  digitalWrite(led2,LOW);
  ledLight.off();
 }
 }
 else{
  Serial.println("system off");
 }
 }
