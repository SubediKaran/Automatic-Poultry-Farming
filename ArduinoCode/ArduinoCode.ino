#include <Servo.h>
Servo servo;
const int trigPin = D4; 
const int echoPin = D5; 
long duration;
int distance;
#define L1 D0 
#define L2 D1
#define R1 D2
#define R2 D3
void motor1(){
  digitalWrite(L1, HIGH);
  digitalWrite(L2, LOW);
}
void motor2on(){
  digitalWrite(R1, HIGH);
  digitalWrite(R2, LOW);
}
void motor2off(){
  digitalWrite(R1, LOW);
  digitalWrite(R2, LOW);
}
const int LED2 = 12;
#include <DHT.h>
#include <ESP8266WiFi.h>
String apiKey = "254QOSOJI85QE8QQ";     //  Enter your Write API key from ThingSpeak
const char *ssid =  "minihostel_wlink";         // replace with your wifi ssid and wpa2 key
const char *pass =  "pubgmobile@@1234";
const char* server = "api.thingspeak.com";
#define DHTPIN 13     //pin where the dht11 is connected
DHT dht(DHTPIN, DHT11);
WiFiClient client;
void setup() {      
       Serial.begin(115200);
       //pinMode(LED1, OUTPUT);
       pinMode(LED2, OUTPUT);
       pinMode(A0, INPUT);
       pinMode(trigPin, OUTPUT); 
       pinMode(echoPin, INPUT); 
        pinMode(L1, OUTPUT);
        pinMode(L2, OUTPUT);
        pinMode(R1, OUTPUT);
        pinMode(R2, OUTPUT);
       servo.attach(15);
       servo.write(0);
       delay(10);
       dht.begin();
       Serial.println("Connecting to ");
       Serial.println(ssid);
       WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED) 
     {
            delay(500);
            Serial.print(".");
     }
      Serial.println("");
      Serial.println("WiFi connected");
 motor1();
}
void loop() {
      int sensorValue = analogRead(A0); 
      float voltage = sensorValue * (5.0 / 1023.0); 
      float h = dht.readHumidity();
      float t = dht.readTemperature();
if (t <=32) {
  motor2on();
  Serial.print(t);
  Serial.println("\n Fan Is On.");
  }
  else {
  motor2off();
  Serial.print(t);
  Serial.println("\n Fan is Off.");
  }  
      if (isnan(h) || isnan(t)) 
              {
                  Serial.println("Failed to read from DHT sensor!");
                  return;
              }
                 if (client.connect(server,80))   //   "184.106.153.149" or api.thingspeak.com
                    {                    
                         String postStr = apiKey;
                         postStr +="&field1=";
                         postStr += String(t);
                         postStr +="&field2=";
                         postStr += String(h);
                         postStr += "\r\n\r\n";
                         client.print("POST /update HTTP/1.1\n");
                         client.print("Host: api.thingspeak.com\n");
                         client.print("Connection: close\n");
                         client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
                         client.print("Content-Type: application/x-www-form-urlencoded\n");
                         client.print("Content-Length: ");
                         client.print(postStr.length());
                         client.print("\n\n");
                         client.print(postStr);
                         Serial.print("Temperature: ");
                         Serial.print(t);
                         Serial.print(" degrees Celcius, Humidity: ");
                         Serial.print(h);
                        Serial.println("%. Send to Thingspeak.");
                    }
          client.stop();
          Serial.println("Waiting..."); 
  // thingspeak needs minimum 15 sec delay between updates
  delay(1000);
  if (voltage <=3) {
  digitalWrite(LED2, LOW);
  Serial.print(voltage);
  Serial.println("\nRoom is Bright, LED is OFF");
  }
  else {
  digitalWrite(LED2, HIGH);
  Serial.print(voltage);
  Serial.println("\nLED is ON");
  }
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
duration = pulseIn(echoPin, HIGH);
distance= duration*0.034/2;
 if(distance < 5) {                                  
      servo.write(180); 
      Serial.println(" servo tap stop"); 
 }        
      else{     
      Serial.println("servo tap open");
      servo.write(0);  
 }
Serial.print("Distance: ");
Serial.println(distance);
}



