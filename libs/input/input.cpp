#include "input.h"
#if TARGET == Emscripten
#include <emscripten.h>
#endif

#ifdef WASM_INPUT
#define CUSTOM_INPUT WASM_INPUT
#endif

LIB_FUNCTION(input) {
  TEXT data;
#if TARGET != Emscripten && !defined(USE_ARDUINO_STRING) && !defined(INPUT_ARDUINO) && !defined(CUSTOM_INPUT)
  std::getline(std::cin, data);
#elif defined(CUSTOM_INPUT)
  data = CUSTOM_INPUT;
#elif defined(USE_ARDUINO_STRING) || defined(INPUT_ARDUINO)
  data = "";
  while (Serial.available() == 0);
  for (byte i = 0; i < 3; i++) {
    while (Serial.available() > 0) {
      data += (char) Serial.read();
      long long t = millis() + 50;
      while (t > millis() && Serial.available() == 0); 
    }
    long long t = millis() + 50;
    while (t > millis() && Serial.available() == 0); 
  }
#else
  int x = EM_ASM_INT({
    var str = prompt("Input:");
    if (str == null) return 0;
    var buffer = Module._malloc(str.length + 1);
  	Module.stringToUTF8(str, buffer, str.length + 1);
  	return buffer;
  });
  if (x == 0) data = "";
  else data = (char*) x;
#endif
  vm->push(Value(data));
}
