
#include <stdint.h>
#include "config.h"
#include <Preferences.h>
Preferences prefsMan;

enum modoOperacao_e
{
  MODO_TEMP_COR,
  MODO_COR,
  MODO_COR_HSV,
  MODO_PALETA,
  MODO_FESTA,
  TOTAL_MODOS
};
struct gessoIOTPrefs_t
{
  enum modoOperacao_e modoOperacao;
  union {
    int32_t valor; // salvo aqui tambem a cor em 32bits 0x00RRGGBB, temp de cor e nr da paleta
    struct
    {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t trash;
    } rgb;
    struct
    {
      uint16_t h;
      uint8_t s;
      uint8_t v;
    } hsv;
  } cor;
} prefs;



void setup()
{
  //carregar configuracoes
  prefsMan.begin("prefs");
  size_t prefsLen = prefsMan.getBytesLength("prefs");
  //se nao existe crio a config default
  if (prefsLen != sizeof(gessoIOTPrefs_t)) {
    prefsMan.clear();
    prefsMan.putBytes("prefs", &prefs, sizeof(gessoIOTPrefs_t));

  }
  prefsMan.getBytes("prefs", &prefs,  sizeof(gessoIOTPrefs_t));
  prefsMan.end();
  
  Serial.begin(115200);
  Serial.println("Booting");
  OTAsetup();
  LEDsetup();
  MQTTsetup();
  pinMode(LED_BUILTIN, OUTPUT);
}
void loop()
{
  OTAloop();
  LEDloop();
  MQTTloop();
}
