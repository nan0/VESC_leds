#include <cstdio>
#include <sys/stdio.h>
#include "HardwareSerial.h"
#include "Vesc.h"

Vesc::Vesc(float minVoltage, float maxVoltage) {
  this->minVoltage = minVoltage;
  this->maxVoltage = maxVoltage;

  /** Setup Serial port to display data */
  Serial.begin(9600);

  /** Setup SoftwareSerial port */
  vescSerial.begin(19200);

  /** Define which ports to use as UART */
  vesc.setSerialPort(&vescSerial);
}

void Vesc::refresh() {
  //  prevRpm = rpm;
  if (!vesc.getVescValues()) {
    printf("Error while acquiring vesc data");
  }
}

// TODO : remove mock
bool Vesc::isStopped() {
  // return !valuesAcquired || vesc.data.rpm == 0;
  return rpm == 0;
}

bool Vesc::isBreaking() {
  return prevRpm > rpm;
}

int Vesc::getVoltagePercent() {
  const float currentVoltage = vesc.data.inpVoltage;
  if (currentVoltage == 0 || currentVoltage <= minVoltage) {
    return 0;
  }

  if (currentVoltage >= maxVoltage) {
    return 100;
  }

  return (int)((currentVoltage - minVoltage) * 100 / (maxVoltage - minVoltage));
}
