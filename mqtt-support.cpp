

void localMQTTCallback(char* topic, byte* payload, unsigned int length) {
  // Relay actions
  if (!strcmp(topic, actionSTopic.c_str()) || !strcmp(topic, groupActionSTopic.c_str()))
  {
    if ((char)payload[0] == '1' || ! strncasecmp_P((char *)payload, "on", length))
    {
      desiredRelayState = 1;
    }
    else if ((char)payload[0] == '0' || ! strncasecmp_P((char *)payload, "off", length))
    {
      desiredRelayState = 0;
    }
    else if ((char)payload[0] == 'X' || ! strncasecmp_P((char *)payload, "toggle", length))
    {
      desiredRelayState = !desiredRelayState;
    }
    else if ((char)payload[0] == 'S' || ! strncasecmp_P((char *)payload, "status", length))
    {
      sendStatus = true;
    }

  } else if (!strcmp(topic, pingSTopic.c_str()))
  // Ping action
  {
    sendPong = true;
  } else {
    mqttCallbackHandle(topic, payload, length) {

  }
}

void mqttSetup(void) {
	// create specific topics
	mqttCreateTopics();

  // after wifi and parameters are configured, create publish topics
  eventTopic = String(F("event/")) + custom_unit_id.getValue() + String(F("/switch"));
  groupEventTopic = String(F("event/")) + custom_group_id.getValue() + String(F("/switch"));
  statusTopic = String(F("status/")) + custom_unit_id.getValue() + String(F("/relay"));
  pongStatusTopic = String(F("pong/")) + custom_unit_id.getValue() + String(F("/status"));
  pongMetaTopic = String(F("pong/")) + custom_unit_id.getValue() + String(F("/meta"));
  // and subscribe topic
  actionSTopic = String(F("action/")) + custom_unit_id.getValue() + String(F("/relay"));
  groupActionSTopic = String(F("action/")) + custom_group_id.getValue() + String(F("/relay"));
  pingSTopic = String(F("ping/nodes"));

  client.setServer(custom_mqtt_server.getValue(), atoi(custom_mqtt_port.getValue()));
  client.setCallback(localMQTTCallback);

}

void mqttPublish(void) {
    // Relay state is updated via the interrupt *OR* the MQTT callback.
  if (relayState != desiredRelayState) {
    Serial.print(F("Chg state to "));
    Serial.println(desiredRelayState);

    digitalWrite(RELAY_PIN, desiredRelayState);
    relayState = desiredRelayState;
    sendStatus = true;
  }


  if (sendPong)
  {
    Serial.print(F("MQTT pub: "));
    String meta = getDeviceMeta(CONFIG_VERSION);
    Serial.print(meta);
    Serial.print(F(" to "));
    Serial.println(pongMetaTopic);
    client.publish(pongMetaTopic.c_str(), meta.c_str());
    sendPong = false;
  }

  // publish event if touched
  if (sendEvent) {
    const char* payload = (relayState == 0) ? "0" : "1";
    Serial.print(F("MQTT pub: "));
    Serial.print(payload);
    Serial.print(F(" to "));
    if (sendGroupEventTopic) {
      Serial.println(groupEventTopic);
      client.publish(groupEventTopic.c_str(), payload);
    } else {
      Serial.println(eventTopic);
      client.publish(eventTopic.c_str(), payload);
    }
    sendEvent = false;
  }

  // publish state when requested to do so
  if (sendStatus) {
    const char* payload = (relayState == 0) ? "0" : "1";
    Serial.print(F("MQTT pub: "));
    Serial.print(payload);
    Serial.print(F(" to "));
    Serial.println(statusTopic);
    client.publish(statusTopic.c_str(), payload);
    sendStatus = false;
  }
}

//
// Connect to MQTT broker
// Subscribe to topics, flash LED etc
//
void checkMQTTConnection(void) {
  Serial.print(F("MQTT conn? "));
  if (client.connected()) Serial.println(F("OK"));
  else {
    if (WiFi.status() == WL_CONNECTED) {
      //Wifi connected, attempt to connect to server
      Serial.print(F("new connection: "));
      if (client.connect(custom_unit_id.getValue(), custom_mqtt_user.getValue(), custom_mqtt_pass.getValue())) {
        Serial.println(F("connected"));
          client.subscribe(pingSTopic->c_str());
          client.subscribe(actionSTopic->c_str());
          client.subscribe(groupActionSTopic->c_str());
        for (int i = 0; i < noSubscribedTopics; i++) 
        {
          client.subscribe(subscribedTopics[i]->c_str());
        }
        client.publish(pongStatusTopic.c_str(), "connected");
      } else {
        Serial.print(F("failed, rc="));
        Serial.println(client.state());
      }
    }
    else {
      //Wifi isn't connected, so no point in trying now.
      Serial.println(F(" Not connected to WiFI AP, abandoned connect."));
    }
  }
  //Set the status LED to ON if we are connected to the MQTT server
  if (client.connected())
    digitalWrite(LED_PIN, LOW);
  else
    digitalWrite(LED_PIN, HIGH);
}
