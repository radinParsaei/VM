#define STD_INCLUDED
#define DYNAMIC_LIBS_NOT_AVAILABLE
#define THREADING PROTOTHREADING
#include <Arduino.h>
#ifndef ESP8266
#include <ArduinoSTL.h>
#endif
#include <vector>

#ifdef ESP8266
inline void exit(int) {
  while(1);
}
#endif
