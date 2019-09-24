
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
/*
/CLIENT/modo ou /modo -> 0= MODO_TEMP_COR, MODO_COR, MODO_PALETA, MODO_FESTA, MODO_NATAL
/temperatura_de_cor   -> temperatura em kelvin
/cor                  -> R,G,B
/paleta               -> 0 ... 9

*/
String topicos[] = {"/modo", "/temperatura_de_cor", "/cor", "/paleta", ""};
long lastMsg = 0, lastReconnect = 0;
char msg[50];
int value = 0;

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Mensagem recebida [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  String pl = String((char *)payload);

  String top = String(topic);
  if (top.endsWith("temperatura_de_cor"))
  {
    prefs.cor.valor = pl.toInt();
    prefs.modoOperacao = MODO_TEMP_COR;
  }
  else if (top.endsWith("cor"))
  {
    prefs.cor.valor = pl.toInt();
    prefs.modoOperacao = MODO_COR;
  }
  else if (top.endsWith("paleta"))
  {
    prefs.cor.valor = pl.toInt();
    prefs.modoOperacao = MODO_PALETA;
  }
  else if (top.endsWith("modo"))
  {
    int modo = pl.toInt();
    if (modo > 0 && modo < TOTAL_MODOS)
      prefs.modoOperacao = MODO_PALETA;
  }

  client.publish(MQTT_CLIENT "/modo", String(prefs.modoOperacao));
}

void reconnect()
{
  Serial.print("Tentando conexão com servidor MQTT...");
  String clientId = MQTT_CLIENT;
  // Attempt to connect
  if (client.connect(clientId.c_str()))
  {
    char buff[100];
    String s = topicos[0];
    int i = 0;
    while (!s.isEmpty())
    {
      client.subscribe(s.c_str());
      s = String("/") + String(MQTT_CLIENT) + s;
      client.subscribe(s.c_str());
      s = topicos[++i];
    }
  }
  else
  {
    Serial.print("erro, rc=");
    Serial.print(client.state());
  }
}

void MQTTsetup()
{
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void MQTTloop()
{
  long nowLoop = millis();
  if (!client.connected() && (nowLoop - lastReconnect > 3000))
  {
    lastReconnect = nowLoop;
    reconnect();
  }

  if (!client.connected())
  {
    // se nao conectou
    return;
  }
  client.loop();

  if (nowLoop - lastMsg > 2000)
  {
    lastMsg = nowLoop;
    ++value;
    snprintf(msg, 50, "hello world #%ld", value);
    Serial.print("Publish message: ");
    Serial.println(msg);
    client.publish("outTopic", msg);
  }
}
