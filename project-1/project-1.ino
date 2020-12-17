/*Kushal's IOT project
Date:17-12-2020
*/
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <BlynkSimpleEsp8266_SSL.h>
#include <SimpleTimer.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHT.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include<ArduinoJson.h>


// Set the LCD address to 0x27 for a 16 chars and 2 line display
#define DHTPIN D8   
#define DHTTYPE DHT11 

SimpleTimer timer;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);
const long utcOffsetInSeconds = 5*60*60+30*60;

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

String apis = "http://api.openweathermap.org/data/2.5/find?q=Titagarh&units=metric&appid=b8b163e201dd3b3afa723b7f001375cd";
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "paHIyAXiq3hQG2xsLfJwrIOWNyWGKVIa";

// Your WiFi credentials.
// Set password to "" for open networks.
String ssid = "realme 7i";
String pass = "Kushal1234";

float h{0.0},t{0.0};
short l;

short hr{0},mi{0};
String da("");
short mt;
short co{0};

WiFiClient client;

String thingSpeakAddress= "http://api.thingspeak.com/update?";
String writeAPIKey;
String tsfield1Name;
int hcount{0};
int windsp;
short clp;


void apiservice(void){
HTTPClient http;
StaticJsonDocument<5000> doc;
http.begin(apis);
int r = http.GET();
Serial.println(r);
//Serial.print(http.getString());
String payload = http.getString();
DeserializationError err = deserializeJson(doc,payload.c_str());
Serial.println(err.c_str());
if(!err){
  Serial.println("...");
  windsp = doc["list"][0]["wind"]["speed"];
  clp = doc["list"][0]["clouds"]["all"];
}

}

void gettime(){
digitalWrite(D0,0);  
timeClient.update();
int chr = timeClient.getHours();
int cmi = timeClient.getMinutes();
String cda = daysOfTheWeek[timeClient.getDay()];
if(co==0){cloud(1,t);co++;}
else if(co==1){cloud(2,h);co++;}
else if(co==2){cloud(3,l);co++;}
else if(co==3){cloud(4,mt);co=0;mt=0;} 



if(chr!=hr || cmi!=mi || cda!= da){
apiservice();
lcd.clear();
String hrp;
if(cmi<10){
if(hr<10){hrp = "0"+String(chr)+":0"+String(cmi);}
else{hrp = String(chr)+":0"+String(cmi);}}
else{
if(hr<10){hrp = "0"+String(chr)+":"+String(cmi);}
else{hrp = String(chr)+":"+String(cmi);}}


lcd.setCursor(0,1);
lcd.print(hrp);
lcd.print(" ");
lcd.print(cda);
String x = " "+String(windsp)+"m/h";
lcd.print(x);

lcd.setCursor(0,0);
String st = String(int(t)) + "oC";
lcd.print(st);
lcd.print(" ");

String n = String(int(h))+"%"+" "+String(l)+"x"+" "+String(clp)+"%";
lcd.print(n);


hr = chr;
mi = cmi;
da = cda;
}
digitalWrite(D0,1); 
}
void schedule(void){







}
void cloud(int fi,int value){
 String request_string;
     HTTPClient http;
    //if (client.connect("api.thingspeak.com",80)) {
      request_string = thingSpeakAddress;
      request_string += "key=";
      request_string += "NCCNFMK49VBY0SHF";
      request_string += "&";
      request_string += "field";
      request_string += fi;
      request_string += "=";
      request_string += value;
      Serial.println(request_string);
      http.begin(request_string);
      int x = http.GET();
      Serial.println(x);
      if(http.getString().length()==0){
      Serial.println("Error");
      digitalWrite(D7,HIGH);
      }
      else{
        digitalWrite(D7,LOW);
      }
      http.end();

    //}
}
void wificheck(void){
  int co{0};
  while (!WiFi.status() == WL_CONNECTED){
    Serial.print(".");
    lcd.clear();
    lcd.print("Wifi Connecting..");
    delay(300);
    if(co++>20){ESP.restart();}
  }
}



void ReadTEMP(void){
   h = dht.readHumidity();
   t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  t-=4;
  Blynk.virtualWrite(V0,t);
  Blynk.virtualWrite(V1,h);
}

void ReadFire(void){
  bool p = digitalRead(D6);
  if(p){
    Blynk.notify("Fire Detected");
    lcd.clear();
    lcd.print("!!ALART!!");
  }
  
}

void ReadMotion(void){
  short rd = digitalRead(D5);
  if(rd==1){
    Blynk.notify("Motion Detected");
    mt = 1;
  }
}

void Readlight(void){
  l = map(analogRead(A0),0,1024,0,100);
  Blynk.virtualWrite(V2,l);
}



void setup()
{
  Serial.println(ssid);
  Serial.println(pass);
  pinMode(D7,OUTPUT);
  lcd.begin();
  timeClient.begin();
  lcd.backlight();
  lcd.setCursor(1,0);
  
  
  String star = String(ssid)+" "+"..";
  lcd.print(star);
  lcd.setCursor(0,1);
  lcd.print("Conn. Server");
  
  pinMode(A0,INPUT);
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  pinMode(D0,OUTPUT);
  digitalWrite(D0,HIGH);
  Serial.begin(9600);
  
  Blynk.begin(auth,ssid.c_str(),pass.c_str());
  dht.begin();
  lcd.clear();
   
timer.setInterval(100L, ReadMotion);
timer.setInterval(100L,ReadFire);
timer.setInterval(30000L,ReadTEMP);
timer.setInterval(1000L,Readlight); 
timer.setInterval(40000L,gettime); 
//timer.setInterval(1000L,apiservice);
}

void loop()
{
  Blynk.run();
  timer.run();
}
