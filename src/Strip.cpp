#include <cstdio>
#include "HardwareSerial.h"
#include <EEPROM.h>
#include "Strip.h"
#include "EEPROMData.h"

#define NB_PIXELS 10  // The number of pixels per strip

Strip::Strip(int pin, int slotNumber) {

  if (slotNumber < 1 || slotNumber > 50) {
    Serial.println("slotNumber must be between 1 and 50");
    return;
  }

  this->slotNumber = slotNumber;

  EEPROM.begin(512);

  leds = Adafruit_NeoPixel(NB_PIXELS, pin, NEO_GRB + NEO_KHZ800);
  leds.begin();
}

//TODO refactor power so both strips are managed as one
void Strip::powerOn(bool on) {
  for (int i = 0; i < NB_PIXELS; i++) {
    if (!on) {
      leds.setPixelColor(i, leds.Color(0, 0, 0));
    } else {
      leds.setPixelColor(i, color);
    }
    leds.show();
  }
  const int address = this->getSlot(EEPROMData::POWER);
  EEPROM.write(address, (char)on);
  EEPROM.commit();
}

bool Strip::isPoweredOn() {
  const int address = Strip::getSlot(EEPROMData::POWER);
  return (bool)EEPROM.read(address);
}

void Strip::animateAccordion() {
  if (!this->isPoweredOn()) {
    return;
  }
  int offset = 0;
  if (led < 10) {
    offset = 0;
  } else {
    offset = 2 * (NB_PIXELS - led) - 2;
  }
  const int ledLeft = led + offset;
  const int ledRight = NB_PIXELS - led - offset - 1;

  leds.clear();
  leds.setPixelColor(ledLeft, color);
  leds.setPixelColor(ledRight, color);
  leds.show();

  if (led == 17) {
    led = 0;
  } else {
    led++;
  }
}

void Strip::animateLinear() {
  if (!this->isPoweredOn()) {
    return;
  }
  int offset = 0;

  if (led > 9) {
    offset = 2 * (NB_PIXELS - led) - 2;
  }

  leds.clear();
  leds.setPixelColor(led + offset, color);
  leds.show();

  if (led == 17) {
    led = 0;
  } else {
    led++;
  }
}

void Strip::fixedLights() {
  if (!this->isPoweredOn()) {
    return;
  }
  for (int i = 0; i < NB_PIXELS; i++) {
    leds.setPixelColor(i, color);
  }
  leds.show();
}

char Strip::getSlot(int EEPROMData) {
  return this->slotNumber * 10 + EEPROMData;
}


// TODO: change color, or else share code with fixed() function
void Strip::displayPercentage(int percentage) {
  if (!this->isPoweredOn()) {
    return;
  }
  leds.clear();
  if (percentage == 0) {
    return;
  }

  const int nbLedsToDiplay = percentage * NB_PIXELS / 100;

  for (int i = 0; i < nbLedsToDiplay; i++) {
    leds.setPixelColor(i, color);
  }
  leds.show();
}

void Strip::setColor(int r, int g, int b) {
  this->color = this->leds.Color(g, r, b);
}
