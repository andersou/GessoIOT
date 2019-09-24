
#include <stdint.h>
#include "config.h"

enum modoOperacao_e
{
  MODO_TEMP_COR,
  MODO_COR,
  MODO_PALETA,
  MODO_FESTA,
  MODO_NATAL,
  TOTAL_MODOS
};
struct gessoIOTPrefs_t
{
  enum modoOperacao_e modoOperacao;
  union {
    uint32_t valor; // salvo aqui tambem a cor em 32bits 0x00RRGGBB, temp de cor e nr da paleta
    struct
    {
      uint8_t b;
      uint8_t g;
      uint8_t r;
      uint8_t trash;
    } rgb;
  } cor;
} prefs;

void setup()
{

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
