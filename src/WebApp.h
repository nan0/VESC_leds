#ifndef WebApp_h
#define WebApp_h

#include <ESP8266WebServer.h>
#include "Arduino.h"

class WebApp {
public:
  const char* getHtml();
};

#endif
