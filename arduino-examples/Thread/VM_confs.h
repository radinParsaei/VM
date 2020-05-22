#define STD_INCLUDED
#define USE_UTILS
#define DYNAMIC_LIBS_NOT_AVAILABLE
#define THREADING PROTOTHREADING
#include <Arduino.h>
#include <ArduinoSTL.h>
#include <vector>

namespace std {
  inline ohserialstream cerr(Serial);
}

