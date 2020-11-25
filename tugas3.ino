#include <WiFi.h>
#include <MQTT.h>

const char ssid[] = "yanginiSSID";
const char pass[] = "inipasswordnya";
int led = 23;
int ldr = 39;
int lux;
char value[20];
char batas[20];

WiFiClient net;
MQTTClient client;


void connect() {
  Serial.print("checking wifi...");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }

  Serial.print("\nconnecting...");
  while (!client.connect("ESP32s", "77baee1b", "35b96c9aaadbce2f")) {//parameter 1 : tipe board , parameter 2 : username token pada topic yang dibuat , parameter 3 : password token pada topic yang dibuat
    Serial.print(".");
    delay(1000);
  }

  Serial.println("\nconnected!");

  client.subscribe("/lux");
  client.subscribe("/batasIntensitasCahaya");
}

void pengukurCahaya(){
  int readLight = analogRead(ldr);
  lux = (0.009768*readLight)+10;
  itoa(lux,value,10);
  delay(500);
  client.publish("/lux",value);
}

void messageReceived(String &topic, String &payload) {
  if(topic == "/batasIntensitasCahaya"){
    strcpy(batas,payload.c_str());
  }
  if(topic == "/lux"){
    if(payload >= batas ){
      digitalWrite(led, HIGH);
      client.publish("/led", "0");
    }
    else if(payload <=  batas){
      digitalWrite(led, LOW);
      client.publish("/led", "1");
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, pass);
  
  pinMode(led, OUTPUT);
 
  client.begin("broker.shiftr.io", net);
  client.onMessage(messageReceived);
  connect();
}

void loop() {
  pengukurCahaya();
  
  client.loop();
  delay(10);  
  if (!client.connected()) {
    connect();
  }
}
