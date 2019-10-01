#include <FastLED.h>



CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100


CRGBPalette16 currentPalette;
TBlendType    currentBlending;
int currentColorIncrement = 3;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;



long lastPaleta = 0;
long ms_update = 1000 / UPDATES_PER_SECOND;
void modoPaleta()
{
  long nowLoop = millis();
  if (!(nowLoop - lastPaleta > ms_update)) {
    return;
  }
  lastPaleta = nowLoop;
  currentColorIncrement = 3;
  if (prefs.cor.valor == -1) {
    ChangePalettePeriodically();
  } else {

    switch (prefs.cor.valor) {
      default:
      case 0:
        currentPalette = RainbowColors_p;
        currentBlending = LINEARBLEND;
        break;
      case 1:
        currentPalette = RainbowStripeColors_p;
        currentBlending = NOBLEND;
        break;
      case 2:
        currentPalette = RainbowStripeColors_p;
        currentBlending = LINEARBLEND;
        break;
      case 3:
        SetupPurpleAndGreenPalette();
        currentBlending = LINEARBLEND;
        break;
      case 4:
        SetupTotallyRandomPalette();
        currentBlending = LINEARBLEND;
        break;
      case 5:
        SetupBlackAndWhiteStripedPalette();
        currentBlending = NOBLEND;
        break;
      case 6:
        SetupBlackAndWhiteStripedPalette();
        currentBlending = LINEARBLEND;
        break;
      case 7:
        currentPalette = CloudColors_p;
        currentBlending = LINEARBLEND;
        break;
      case 8:
        currentPalette = PartyColors_p;
        currentBlending = LINEARBLEND;
        break;
      case 9:
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = NOBLEND;
        break;
      case 10:
        currentPalette = myRedWhiteBluePalette_p;
        currentBlending = LINEARBLEND;
        break;
      case 11:
        SetupNatalPalette();
        currentColorIncrement = 12;
        currentBlending = LINEARBLEND;
        break;
      case 12:
        SetupNatalPalette();
        currentColorIncrement = 12;
        currentBlending = NOBLEND;
        break;
    }
  }
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */

  FillLEDsFromPaletteColors( startIndex);

  //    FastLED.show();
  //    FastLED.delay(1000 / UPDATES_PER_SECOND);
}




void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;

  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += currentColorIncrement;
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      currentPalette = RainbowColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 10)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 15)  {
      currentPalette = RainbowStripeColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 20)  {
      SetupPurpleAndGreenPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 25)  {
      SetupTotallyRandomPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 30)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = NOBLEND;
    }
    if ( secondHand == 35)  {
      SetupBlackAndWhiteStripedPalette();
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 40)  {
      currentPalette = CloudColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 45)  {
      currentPalette = PartyColors_p;
      currentBlending = LINEARBLEND;
    }
    if ( secondHand == 50)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = NOBLEND;
    }
    if ( secondHand == 55)  {
      currentPalette = myRedWhiteBluePalette_p;
      currentBlending = LINEARBLEND;
    }
  }
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for ( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}
void SetupNatalPalette()
{
  CRGB red = CRGB( 255, 0, 0);
  CRGB green  = CRGB( 0, 255, 0);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     red,  black,  green,  black,
                     red, black, green,  black,
                     red, black, green,  black,
                     red, black, green,  black );
}

// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

void LEDsetup() {
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalSMD5050 );
  FastLED.setBrightness(  BRIGHTNESS );

  currentPalette = RainbowColors_p;
  currentBlending = LINEARBLEND;
}
void LEDloop() {
  switch (prefs.modoOperacao) {
    case MODO_PALETA:
      modoPaleta();
      break;
    case MODO_TEMP_COR:
      int r, g, b;
      temp2rgb(prefs.cor.valor, &r, &g, &b);
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(r, g, b);
      }
      break;
    case MODO_COR:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CRGB(prefs.cor.rgb.r, prefs.cor.rgb.g, prefs.cor.rgb.b);
      }
      break;
    case MODO_COR_HSV:
      for (int i = 0; i < NUM_LEDS; i++) {
        leds[i] = CHSV(prefs.cor.hsv.h, prefs.cor.hsv.s, prefs.cor.hsv.v);
      }
      break;
      break;
  }

  FastLED.show();

}
