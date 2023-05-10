#ifndef Vesc_h
#define Vesc_h
#include <VescUart.h>
#include <SoftwareSerial.h>

class Vesc {
private:
  VescUart vesc;
  SoftwareSerial vescSerial = SoftwareSerial(D7, D8);
  float minVoltage;  // the minimum allowed voltage of the battery
  float maxVoltage;  // the maximum allowed voltage of the battery
public:
  float prevRpm;  // the previous rpm record;
  float rpm;        // Mock to be removed once in real conditions
  float dutyCycle;  // Mock to be removed once in real conditions
  /**
    Constructor
    param minVoltage : the minimum allowed voltage of the battery
    param maxVoltage : the maximum allowed voltage of the battery
  **/
  Vesc(float minVoltage, float maxVoltage);

  /**
    Refreshes the vesc data
  **/
  void refresh();

  /**
    Whether the board is stopped or running
    returns : true if stopped, false otherwise
  **/
  bool isStopped();

  /**
    Whether the board is breaking or not
    returns : true if breaking, false otherwise
  **/
  bool isBreaking();

  /**
    Computes the ratio between the max voltage and the current one. 
    Does not perform any estimation of the capacity remaining
    returns : the remaining voltage percentage, between 0 and 100
  **/
  int getVoltagePercent();
};

#endif
