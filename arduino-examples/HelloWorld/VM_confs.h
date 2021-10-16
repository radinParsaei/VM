#define STD_INCLUDED
#define DYNAMIC_LIBS_NOT_AVAILABLE
#define RUN_PROTO_THREADS_AT_EXIT
#ifndef ESP32
#define THREADING PROTOTHREADING
#endif
#include <Arduino.h>
#if !defined(ESP8266) && !defined(ESP32) && defined(__AVR__)
//#include <ArduinoSTL.h>
#include <vector>
#endif
#define STATIC_BUILD_MODE

#if !defined(ESP32) && !defined(ESP8266) && defined(__AVR__)
#define USE_ARDUINO_STRING
#endif

#define Emscripten 1
#define ARDUINO 2

#define TARGET ARDUINO

#ifdef ESP8266
inline void exit(int) {
  while(1);
}
#endif
