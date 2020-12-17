<<<<<<< HEAD
/*Kushal's IOT project
project created on 10-12-2020
A simple project for Home automation system which is a intregation of
Weather station and room security system.

Thanks to the library creators for making my job easy.The definations of every segment is mentioned.
My github repo link-https://github.com/Kushalbanik262/SmartHomeKushal.git
Hardware used ESP8266(nodemcu). 
Opensource project development using opensource libs.
Feel free to contribute for more firmware efficiency.
*/

#define BLYNK_PRINT Serial 
#include <ESP8266WiFi.h> //Main lib for ESP8266 
#include <ESP8266HTTPClient.h> //For Http/Https api services
#include <BlynkSimpleEsp8266_SSL.h> //Support lib of blynk
#include <SimpleTimer.h> //Some scheduling is needed for task sequencing
#include <NTPClient.h> //NTP server 
#include <WiFiUdp.h> //supporting wifi lib
#include "DHT.h" //Temp sensor protocol converter(DHT11/DHT22/DHT21) 
#include <Wire.h> //For spi communication some functions helps for I2c interface
#include <LiquidCrystal_I2C.h>//Lib for I2c and lcd interfacing
#include<ArduinoJson.h> //The lib for arduino json manupulation



#define DHTPIN D8 //Enter your pin except D0   
#define DHTTYPE DHT11 //Your DHT sensor type

SimpleTimer timer; //Object of simpleTimer class;
LiquidCrystal_I2C lcd(0x27, 16, 2);//My lcd is (16x2) here also Set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);//Init of DHT sensor
const long utcOffsetInSeconds = 5*60*60+30*60; //The NTP server timezone (Calculate it according to your location's Degrees and Mins)

char daysOfTheWeek[7][12] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; //Weekdays
=======
/*Kushal's IOT project*/
#define BLYNK_PRINT Serial
#include<Arduino.h>
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
>>>>>>> new

// Define NTP Client to get time
WiFiUDP ntpUDP; //Ntp supporter object of WIFIUDP class;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);//constructor initialization

String apis = "http://api.openweathermap.org/data/2.5/find?q={*YOUR_CITY_NAME*}&units=metric&appid={YOUR_API_KEY}";//The weather api service for more go through .md file
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "****";//Your blynk authentication Token

// Your WiFi credentials.
// Set password to "" for open networks.
String ssid = "****";//your SSID
String pass = "****";//your PASSWORD

float h{0.0},t{0.0}; //Initalizing h(humidity) and t(temp) .
short l;// Light luminicance

short hr{0},mi{0}; //Initialization of HRS and MINS
String da(""); //Date initialization
short mt; //Motion control parameter
short co{0}; //cloud schedular

WiFiClient client;//WIFI client for specific web port handaling 

String thingSpeakAddress= "http://api.thingspeak.com/update?"; //Thingspeak cloud api url
String writeAPIKey; //APIKEY 
String tsfield1Name; //Field name of Thingspeak
int windsp;//Windspeed from api service
short clp; //cloud percentage of your location%


void apiservice(void){//API service where we get the json of windspeed and cloud percentage
HTTPClient http;
StaticJsonDocument<5000> doc; //stringlength*8 should me min template value
http.begin(apis);
int r = http.GET(); 
Serial.println(r);
//Serial.print(http.getString());
String payload = http.getString();
DeserializationError err = deserializeJson(doc,payload.c_str()); //Deserialization
Serial.println(err.c_str());
if(!err){
  Serial.println("...");
  windsp = doc["list"][0]["wind"]["speed"]; //Fetching windspeed
  clp = doc["list"][0]["clouds"]["all"]; //Fetching cloudpercentage
}

}

void gettime(){ //The main LCD display function which updates in every minutes
digitalWrite(D0,0);  
timeClient.update();
int chr = timeClient.getHours();//NTP hours
int cmi = timeClient.getMinutes();//NTP Minutes
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
void schedule(void){ //This is schedule (static scheduling is not done contribute if have idea~~)



}
void cloud(int fi,int value){ //Uploads data to the thingspeak cloud
 String request_string;
     HTTPClient http;//http/https service object 
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
      if(http.getString().length()==0){//Show error if thingspeak get method is not valid
      Serial.println("Error");
      digitalWrite(D7,HIGH);
      }
      else{
        digitalWrite(D7,LOW);
      }
      http.end();

    //}
}
void wificheck(void){ //Function to check if the wifi is available or not
  int co{0};
  while (!WiFi.status() == WL_CONNECTED){
    Serial.print(".");
    lcd.clear();
    lcd.print("Wifi Connecting..");
    delay(300);
    if(co++>20){ESP.restart();}//if exceeds 6 sec by searching
  }
}



void ReadTEMP(void){//Reading the temp and hum in global variables
   h = dht.readHumidity();
   t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  t-=4;
  Blynk.virtualWrite(V0,t); //Blynk virtual writing to pins(V0,V1)
  Blynk.virtualWrite(V1,h);
}

void ReadFire(void){//Reading from the fire sensor and giving the result to LCD
  bool p = digitalRead(D6);
  if(p){
    Blynk.notify("Fire Detected");
    lcd.clear();
    lcd.print("!!ALART!!");
  }
  
}

void ReadMotion(void){//Reading the motion from the PIR sensors
  short rd = digitalRead(D5);
  if(rd==1){
    Blynk.notify("Motion Detected");//Blynk Notification
    mt = 1;
  }
}

void Readlight(void){//Reading the light lumi. from LDR sensor
  l = map(analogRead(A0),0,1024,0,100);
  Blynk.virtualWrite(V2,l);
}



void setup()//setup code
{
  Serial.println(ssid);
  Serial.println(pass);
  pinMode(D7,OUTPUT);
  lcd.begin();//Starting lcd
  timeClient.begin();//timer begin
  lcd.backlight();
  lcd.setCursor(1,0);//setting cursor
  
  
  String star = String(ssid)+" "+"..";
  lcd.print(star);
  lcd.setCursor(0,1);
  lcd.print("Conn. Server");
                    //Configure pins as io as you need
  pinMode(A0,INPUT);
  pinMode(D5,INPUT);
  pinMode(D6,INPUT);
  pinMode(D0,OUTPUT);
  digitalWrite(D0,HIGH);
  Serial.begin(9600);
  
  Blynk.begin(auth,ssid.c_str(),pass.c_str());//starting blynk
  dht.begin();//staring dht sensor
  lcd.clear();

//Scheduling is done based on the timings that you have created and blynk servers; 

timer.setInterval(100L, ReadMotion);//For Motion
timer.setInterval(100L,ReadFire);//For fire sensing
timer.setInterval(30000L,ReadTEMP);//For Temp sensing
timer.setInterval(1000L,Readlight); //For light sensing
timer.setInterval(40000L,gettime); //For getting time and printing
}

void loop()//The main loop .Don't add any other parameters only work through scheduling for better performance
{
  Blynk.run();//Run blynk
  timer.run();//Run timer object
}
