#include "input.h"
#if TARGET == Emscripten
#include <emscripten.h>
#endif

LIB_FUNCTION(input) {
  TEXT data;
#if TARGET != Emscripten && !defined(USE_ARDUINO_STRING) && !defined(ARDUINO_SAM_DUE)
  std::getline(std::cin, data);
#elif defined(WASM_INPUT)
  data = WASM_INPUT;
#elif defined(USE_ARDUINO_STRING)
  data = Serial.read();
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
