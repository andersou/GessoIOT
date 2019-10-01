
#include <WiFi.h>
#include <PubSubClient.h>

WiFiClient espClient;
PubSubClient client(espClient);
/*
  /CLIENT/modo ou /modo -> 0= MODO_TEMP_COR, MODO_COR, MODO_PALETA, MODO_FESTA
  /temperatura_de_cor   -> temperatura em kelvin
  /cor                  -> rgb(R,G,B), ou H,S,V
  /paleta               -> -1 ... 10 ->
       -1:
         troca periodicamente
       0:
        RainbowColors_p LINEARBLEND
       1:
        RainbowStripeColors_p NOBLEND
       2:
        RainbowStripeColors_p LINEARBLEND
       3:
        PurpleAndGreenPalette LINEARBLEND
       4:
        TotallyRandomPalette LINEARBLEND
       5:
        BlackAndWhiteStripedPalette NOBLEND
       6:
        BlackAndWhiteStripedPalette LINEARBLEND
       7:
        CloudColors_p LINEARBLEND
       8:
        PartyColors_p LINEARBLEND
       9:
        RedWhiteBluePalette_p NOBLEND;
       10:
        RedWhiteBluePalette_p LINEARBLEND;
       11:
        Natal LINEARBLEND;
       12:
        Natal NOBLEND;

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
    prefs.modoOperacao = MODO_COR;
    uint8_t pos;
    if (pl.indexOf("rgb") != -1) {
      prefs.cor.rgb.r = pl.substring(4).toInt();
      pos = pl.indexOf(",") + 1;
      prefs.cor.rgb.g = pl.substring(pos).toInt();

      pos = pl.indexOf(",", pos) + 1;
      prefs.cor.rgb.b = pl.substring(pos).toInt();

    } else if ((pos=pl.indexOf(",")) != -1) //cor em hsv
    {
      prefs.modoOperacao = MODO_COR_HSV;
      prefs.cor.hsv.h = map(pl.toInt(),0,360,0,255);
      pos=pl.indexOf(",") + 1;
      prefs.cor.hsv.s =  map(pl.substring(pos).toInt(),0,100,0,255);
      pos=pl.indexOf(",",pos) + 1;
      prefs.cor.hsv.v =  map(pl.substring(pos).toInt(),0,100,0,255);
      
    } else {
      prefs.cor.valor = pl.toInt();
    }
    
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
  //salvo prefs
  prefsMan.begin("prefs");
  prefsMan.putBytes("prefs", &prefs, sizeof(gessoIOTPrefs_t));
  prefsMan.end();
}

void reconnect()
{
  Serial.print("Tentando conexão com servidor MQTT...");
  String clientId = MQTT_CLIENT;
  // Attempt to connect
  if (client.connect(clientId.c_str()))
  {
    String s = topicos[0];
    int i = 0;
    while (!s.isEmpty())
    {
      client.subscribe(s.c_str());
      s = topicos[++i];
    }
    client.subscribe("/"MQTT_CLIENT"/#");
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
