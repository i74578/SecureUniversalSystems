# SecureUniversalSystems

This is the source for the [secureuniversal.systems website](https://secureuniversal.systems)
## MainCode

The MainCode contains the C code we have used on our ESP32-module. The code contains all the pins for the ESP32 and how they work together, as well as all our functions and libraries we use to control the different sensors and motor. The MainCode is a result of all the things in the "Unused" folder that we have consolidated into a single program. 

## Unused

Contains all the individual files we used to test each element of our project. For instance, we have a file for only testing the distance sensor and so on. Here we also have all the methods we never implemented in the end. Like master/slave idea we considered, where we would use two ESP8266 units communicating with each other, instead of a single ESP32. 
In summary this folder contains everything we have had on Github in one way or another, whether it made it into the final product or not. Therefore the code in "Unused" isn't commented and described as well as the code in "MainCode". 

## WebsiteBackend

This contains all the code for our website and backend. It is split into two folders, "www" containing everything for the website, "MQTTSubscriber" containing everything for MQTT communications with the microcontroller.

