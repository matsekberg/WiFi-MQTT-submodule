void setupMQTT(void) {
	// create specific topics
	createTopics();

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
  client.setCallback(handleMQTT);

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
