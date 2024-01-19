# SecureUniversalSystems

This is the source for the [secureuniversal.systems website](https://secureuniversal.systems)
## MainCode

The MainCode contains the C-code we have used on our ESP32-module. The code contains all the pins for the ESP and how they work together. It also contains all our functions and libraries we use to control the different sensors and motor. The MainCode is a result of all the things in the "Unused" folder that we have consolidated and actually used in our project. 

## Unused

Contains all the individual files we used to try each element of our project. For instance we have a file for only testing the distance sensor and so on. Here we also have all the methods we never implemented in the end. Like master/slave idea we had where we used two ESP8266 instead. But since we could use and ESP32 instead we switched up this idea.
So all in all this file contains everything we have pushed into github some of the parts we used in the MainCode some never made the final project. Therefore the code in "Unused" isn't commented and described as well as the code in "MainCode". 

## WebsiteBackend

This contains all the all the code for our website and backend. It is split into two folders one containing all the files and code for our website, this is in the "www" folder. 
The other folder "MQTTSubsriber" folder contains the python file we have used for using MQTT.  

