
#include "config.h"


void setup() {
  
  Serial.begin(115200);
  Serial.println("Booting");
  OTAsetup();
  LEDsetup();
  MQTTsetup();
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  OTAloop();
  LEDloop();
  MQTTloop();
}
