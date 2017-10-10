
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


#define LONG_PRESS_MS 1000
#define SHORT_PRESS_MS 100
#define CONFIG_WIFI_PRESS_MS 5000
#define CONFIG_TOUCHES_COUNT 3
#define MQTT_CHECK_MS 15000

#define OTA_PASS "UPDATE_PW"
#define OTA_PORT 8266

#define BUTTON_PIN  0  // GPIO0, pin 18, D3
#define LED_PIN     2  // GPIO2, pin 17, D4
#define RELAY_PIN   5  // GPIO5, pin 20, D1 (SCL)
   
WiFiClient espClient;
PubSubClient client(espClient);
unsigned long uptime = 0;


volatile int desiredRelayState = 0;
volatile int relayState = 0;
volatile unsigned long millisSinceChange = 0;
volatile int noOfConfigTouches = 0;

volatile boolean sendGroupEventTopic = false;
volatile boolean configWifi = false;
volatile boolean sendEvent = true;
boolean sendStatus = true;
boolean sendPong = false;
boolean sendSensors = false;

String eventTopic;         // published when the switch is touched
String groupEventTopic;    // published when the switch was long touched
String statusTopic;        // published when the relay changes state wo switch touch
String pongStatusTopic;    // publish node status topic
String pongMetaTopic;      // publish node meta topic
String pingSTopic;         // subscribe to nodes ping topic
String actionSTopic;       // subscribed to to change relay status
String groupActionSTopic;  // subscribed to to change relay status based on groupid


unsigned long lastMQTTCheck = -MQTT_CHECK_MS; //This will force an immediate check on init.
bool printedWifiToSerial = false;


// this method needs to be called from setup()
void mqttSetup(void);
void mqttLoop(void);
void mqttCheckConnection(void);


// these methods needs to be implemented
void mqttCallbackCreateTopics(void);
void mqttCallbackHandle(char* topic, byte* payload, unsigned int length);

// internal deps
String getDeviceMeta(const char* configVersion);

WiFiManagerParameter custom_mqtt_server = NULL;
WiFiManagerParameter custom_mqtt_port = NULL;
WiFiManagerParameter custom_mqtt_user = NULL;
WiFiManagerParameter custom_mqtt_pass = NULL;
WiFiManagerParameter custom_unit_id = NULL;
WiFiManagerParameter custom_group_id = NULL;

