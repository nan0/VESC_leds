#ifndef Strip_h
#define Strip_h

#include <Adafruit_NeoPixel.h>
#include "Arduino.h"
#include "AnimationType.h"

class Strip {

private:
  Adafruit_NeoPixel leds;

  int slotNumber;

  int color;  //The color of the strip

  /*
    The currently active led. From 1 to 17.
    0 is the left handside led, it marks the begining of the animation
    9 is the right handside led, it marks the middle of the animation
    17 is the left handside led, it marks the end of the animation
  */
  int led = 0;

  /**
    Finds the addresses slot on which the EEPROMData can be right
    returns the slot as a char
  **/
  char getSlot(int EEPROMData);

public:
  /**
    Construcor
    param pin : the strip pin
    param slotNumber : the slot of 10 bytes on on which the strip will persist data (from 1 to 50)
  **/
  Strip(int pin, int slotNumber);

  /**
    Powers On/off the led Strip and persists it
    param on : true if on, false if off
  **/
  void powerOn(bool on);

  /**
    Gets the persisted power status
    returns : true if on, false, if off
  **/
  bool isPoweredOn();

  /**
    Sets and persists the brightness
    param level : 0 to 255 
  **/
  void setBrightness(int level);


  /**
    Gets the persisted brightness
    returns the brightness level (0 to 255) 
  **/
  int getBrightness();

  /**
    Sets the strip color
    param r - red
    param g - green
    param b - blue
  **/
  void setColor(int r, int g, int b);

  /**
    Displays a percentage ratio
    param perentage : the percentage to display
  **/
  void displayPercentage(int percentage);

  /**
    Triggers one turn of accordion animation for both the Strip
  **/
  void animateAccordion();

  /**
    Triggers one turn of linear animation for both the Strip
  **/
  void animateLinear();

  /**
    Turns on the lights without any animation
  **/
  void fixedLights();
};

#endif
