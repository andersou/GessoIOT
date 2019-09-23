
#include <WiFi.h>
#include <PubSubClient.h>


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0, lastReconnect = 0;
char msg[50];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  String pl = String((char*)payload);
  
  String top = String(topic);
  if(top.equalsIgnoreCase("temperatura_cor")){
    tempCor = pl.toInt();
    modo = MODO_TEMP_COR;
  }else if(top.equalsIgnoreCase("cor_rgb")){
    tempCor = pl.toInt();
    modo = MODO_TEMP_COR;
  }
  if(top.equalsIgnoreCase("modo_paleta")){
    modo = MODO_PALETA;
  }
  

}

void reconnect() {
  Serial.print("Tentando conexão com servidor MQTT...");
  String clientId = MQTT_CLIENT;
  // Attempt to connect
  if (client.connect(clientId.c_str())) {
    Serial.println("Conectou");
    // Once connected, publish an announcement...
    client.publish("outTopic", "hello world");
    // ... and resubscribe
    client.subscribe("temperatura_cor");
    client.subscribe("modo_paleta");
  } else {
    Serial.print("erro, rc=");
    Serial.print(client.state());
  }

}

void MQTTsetup() {
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void MQTTloop() {

  if (!client.connected() && (millis() - lastReconnect >  3000)) {
    lastReconnect = millis();
    reconnect();
  }

  if (!client.connected()) {
    // se nao conectou
    return;
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) {
    lastMsg = now;
    ++value;
    snprintf (msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
