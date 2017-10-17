#ifndef MQTT_SUPPORT

#include <PubSubClient.h>
#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include <ArduinoJson.h>


#define LONG_PRESS_MS 1000
#define SHORT_PRESS_MS 100
#define CONFIG_WIFI_PRESS_MS 5000
#define CONFIG_TOUCHES_COUNT 3
#define MQTT_CHECK_MS 15000

#define PL_OFF    "off"
#define PL_ON     "on"
#define PL_TOGGLE "toggle"
#define PL_STATUS "status"

#ifdef TH10
#define BUTTON_PIN  0  // GPIO0,  pin 15
#define LED_PIN    13  // GPIO13, pin 12
#define RELAY_PIN  12  // GPIO12, pin 6
#define LEDON  LOW
#define LEDOFF HIGH
#endif

#ifdef POW
#define BUTTON_PIN  0  // GPIO0,  pin 15
#define LED_PIN    15  // GPIO15, pin 13
#define RELAY_PIN  12  // GPIO12, pin 10
#define LEDON  HIGH
#define LEDOFF LOW

#define CF_PIN     14  // GPIO14, pin 9
#define CF1_PIN    13  // GPIO13, pin 12
#define SEL_PIN     5  // GPIO5,  pin 24
#endif

#ifdef S20
#define BUTTON_PIN  0  // GPIO0,  pin 15
#define LED_PIN    13  // GPIO13, pin 12
#define RELAY_PIN  12  // GPIO12, pin 6
#define LEDON  LOW
#define LEDOFF HIGH
#endif

#ifdef TOUCH
#define BUTTON_PIN  0  // GPIO0,  pin 15
#define LED_PIN    13  // GPIO13, pin 12
#define RELAY_PIN  12  // GPIO12, pin 6
#define LEDON  LOW
#define LEDOFF HIGH
#endif

#ifdef WEMOS
#define BUTTON_PIN  0  // GPIO0,  pin 15
#define LED_PIN    13  // GPIO13, pin 12
#define RELAY_PIN  12  // GPIO12, pin 6
#define LEDON  LOW
#define LEDOFF HIGH
#endif

//--WiFiClient espClient;
PubSubClient client(espClient);
unsigned long uptime = 0;
unsigned int connections = 0;

volatile int desiredRelayState = 0;
volatile int relayState = 0;
volatile unsigned long millisSinceChange = 0;
volatile int noOfConfigTouches = 0;

volatile boolean sendGroupEventTopic = false;
//--volatile boolean configWifi = false;
volatile boolean sendEvent = true;
boolean sendStatus = true;
boolean sendPong = false;

String eventTopic;         // published when the switch is touched
String groupEventTopic;    // published when the switch was long touched
String statusTopic;        // published when the relay changes state wo switch touch
String pongStatusTopic;    // publish node status topic
String pongMetaTopic;      // publish node meta topic
String pingSTopic;         // subscribe to nodes ping topic
String actionSTopic;       // subscribed to to change relay status
String groupActionSTopic;  // subscribed to to change relay status based on groupid


unsigned long lastMQTTCheck = -MQTT_CHECK_MS; //This will force an immediate check on init.


// this method needs to be called from setup()
void mqttSetup(void);
void mqttLoop(void);

void mqttCheckConnection(void);
void mqttUpdateLED(void);
void mqttPublishMessage(const char* topic, const char* payload);

// these methods needs to be implemented
void mqttCallbackCreateTopics(void);
void mqttCallbackHandle(char* topic, byte* payload, unsigned int length);

#define MQTT_SUPPORT
#endif
