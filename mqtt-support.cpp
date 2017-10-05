//
// Connect to MQTT broker
// Subscribe to topics, flash LED etc
//
void checkMQTTConnection() {
  Serial.print(F("MQTT conn? "));
  if (client.connected()) Serial.println(F("OK"));
  else {
    if (WiFi.status() == WL_CONNECTED) {
      //Wifi connected, attempt to connect to server
      Serial.print(F("new connection: "));
      if (client.connect(custom_unit_id.getValue(), custom_mqtt_user.getValue(), custom_mqtt_pass.getValue())) {
        Serial.println(F("connected"));
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
