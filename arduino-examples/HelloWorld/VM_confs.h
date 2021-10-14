#define STD_INCLUDED
#define DYNAMIC_LIBS_NOT_AVAILABLE
#define RUN_PROTO_THREADS_AT_EXIT
#ifndef ESP32
#define THREADING PROTOTHREADING
#endif
#include <Arduino.h>
#if !defined(ESP8266) && !defined(ESP32)
#include <ArduinoSTL.h>
#endif
#include <vector>
#define STATIC_BUILD_MODE

#define USE_ARDUINO_STRING

#define Emscripten 1
#define ARDUINO 2

#define TARGET ARDUINO

#ifdef ESP8266
inline void exit(int) {
  while(1);
}
#endif
