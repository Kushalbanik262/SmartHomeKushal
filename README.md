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

![IMG_20201219_012312](https://user-images.githubusercontent.com/59107993/102655885-0aaf6280-4199-11eb-8800-92832c8b9e9f.jpg)


Esp8266 is used here as main wifi module.It's peripherals are connected with other hardwares that are mentioned above.
The Intrigations that the IOT device is doing 
1. IOT weather station :- we all know the room temp,humidity and surrounding paramerts are very importent concern to the people. So here the informations of Temp,Humidity and Light are being taken from room but also the information of cloud pecentage in the area,wind speed are newly added things.We have done such things based on
on -------->

()-> DHT11,LDR sensor for Temp,Humidity and Light monitoring but the other two parts are accumulated by api service from weather api.Dht11 is a good sensor for temp and humidity resolution.Now the ldr I have used based on commonly used selenium and created a voltage divider rule with 10k resistance.The Adc is taken of 10 bit (0-1023) is mapped to (0-100) for percentage usage.
The API service is based on my location weather forcast.There are many parameters but for my requirements I am taking only those two which I have mentioned earlier.
The link for the site - https://openweathermap.org/api .The data is taken as JSON format Deserialized from there and taken accordingly.The get request is done during scheduling and all the datas are extracted. See code for more about it.

2.IOT smart room :- The second and one of the most important part is  smart home with security. The sensors like Motion sensor,Fire sensor are used here. The motion sensor is calibrated to get the readings after certain time intervals. Also the scheduling is done according to that in a syncronized way.The realtime datas are very sent to phone using blynk server at that moment.The attached part is a relay which is very necessary for controlling light and fan.
The static scheduling which can be based on NTP server time or something else can be done (The place is vacant in code).






