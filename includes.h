
// wifi and mqtt stuff

#include <WiFiManager.h>
#include <ESP8266WiFi.h>
// must increase max packet size to > 500
#include <PubSubClient.h>
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ESP8266WiFi.h>          //https://github.com/esp8266/Arduino
#include <ArduinoJson.h>

//needed for library
#include <DNSServer.h>
#include <ESP8266WebServer.h>

#include <ArduinoOTA.h>

// sensor libs

#include "DHT.h"
#include <WEMOS_Matrix_LED.h>

   
