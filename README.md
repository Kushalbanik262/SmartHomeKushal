# SmartHomeSystem
Smart Home Monitoring system using IOT.
This is based on a IOT device that is used to take the necessary parameters from outside and cultivate to the user.
This can be discussed over the segments of this device.
Let's come to the hardware parts of this--- 

1. Main part we use is the NodeMCU(esp8266). 
2. For electrical apllience control we use a single channel relay
3. Sensors like DHT11 (Humidity and Temparetue)
4. Fire sensor module
5. PIR motion sensor
6. LDR light sensor(Build on board not using module)
7. For output we use LCD with I2C converter(For less hazar and less pin manupulation).Only SCL and SDA.
8. Some notification leds
9. Jumper wires
10. Header pins
Total system overview------

![IMG_20201219_012312](https://user-images.githubusercontent.com/59107993/102655885-0aaf6280-4199-11eb-8800-92832c8b9e9f.jpg)

The circuit is very straight forward where i have manupulated pins in sequencial manner.Digital pins are mapped for digital sensors or digital outputs and only the adc pin is used for LDR.The pic of circuit diagram---

![Circuit Diagram](https://user-images.githubusercontent.com/59107993/102721616-e03ce100-4321-11eb-86da-4b8483fd8c0c.png)

The pin details are in pin_Diagram.xlsx file.

Esp8266 is used here as main wifi module.It's peripherals are connected with other hardwares that are mentioned above.
The Intrigations that the IOT device is doing 
1. IOT weather station :- we all know the room temp,humidity and surrounding paramerts are very importent concern to the people. So here the informations of Temp,Humidity and Light are being taken from room but also the information of cloud pecentage in the area,wind speed are newly added things.We have done such things based on
on -------->

()-> DHT11,LDR sensor for Temp,Humidity and Light monitoring but the other two parts are accumulated by api service from weather api.Dht11 is a good sensor for temp and humidity resolution.Now the ldr I have used based on commonly used selenium and created a voltage divider rule with 10k resistance.The Adc is taken of 10 bit (0-1023) is mapped to (0-100) for percentage usage.
The API service is based on my location weather forcast.There are many parameters but for my requirements I am taking only those two which I have mentioned earlier.
The link for the site - https://openweathermap.org/api .The data is taken as JSON format Deserialized from there and taken accordingly.The get request is done during scheduling and all the datas are extracted. See code for more about it.

2. IOT smart room :- The second and one of the most important part is  smart home with security. The sensors like Motion sensor,Fire sensor are used here. The motion sensor is calibrated to get the readings after certain time intervals. Also the scheduling is done according to that in a syncronized way.The realtime datas are very sent to phone using blynk server at that moment.The attached part is a relay which is very necessary for controlling light and fan.
The static scheduling which can be based on NTP server time or something else can be done.

()->The PIR module is used as motion detection sensor when this gets some motion or partial infrared then it triggers to a digital pin with high value.Then we got the value uploads to cloud and generates a notification to blynk. The fire sensor is also a sensor which detects the fire and then interrupt the fortware and send that to blynk server.Some scheudling timers are tuned to overcome some glittery outputs.

This is the device LCD and datas:-
![IMG_20201219_012119](https://user-images.githubusercontent.com/59107993/102694271-ecec0700-4245-11eb-947a-16439e033de9.jpg)



 Cloud - The cloud services that I am using here are 1.Thingspeak 2.Blynk 
1. Thingspeak:- I am using thingspeak cloud service as a sequenced data collector. That I can see a large timeseries data at one glance. There are 4 fields where I can see datas The first one is for Temp second one is for Humid the third one is for light luminicence and the forth one is for human motion tracking. Those are really helpful because just i have to do api GET method for that by a specific URL.My channel is in private mode for some security issues so the link is not available.But the pic is-
![Thng1](https://user-images.githubusercontent.com/59107993/102694441-0d689100-4247-11eb-8be3-505035188b35.PNG)

Data sampling of every specified fields are tuned based on priority like Temp and humidity is not taken that much fast but I take the light faster than those and the motion is fastest in that scenario.So the order is in right mode.

2. Blynk:-Blynk is the most unique opensource IOT development plaform .I have used Blynk here to integrate the IOT system with mobile app for realtime monitor and control.This app consist of a UI and a backend.I have done the backend manually rather than their backend app wise pin configuraion.That means I have used all virtual pins for my own control rather than blynk and that helped a lot .some glimpses of the app ---
![IMG_20201219_222403](https://user-images.githubusercontent.com/59107993/102697551-8cb48f80-425c-11eb-8702-d57e732c6a62.jpg)

The Blynk.run() function does all of them at once in c++ code but the code is manupulated and integrated with every other things those are mentioned here.


Observations:- Most importent technical thing that can be done to upgrade the project is data handling.Feel free to contribute or give such ideas to get datas from there and process them using data science or other machine learning models and algorithums. The serverside data managment will help for more smart manupulations like turning of lights automatically by understanding user's behaviour or deep learning based home security .As datas are importent today we need to manupulate data efficiently so i am ready to go with that in this specified device.




