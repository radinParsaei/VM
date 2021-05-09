#include "input.h"
#if TARGET == Emscripten
#include <emscripten.h>
#endif

LIB_FUNCTION(input) {
  std::string data;
#if TARGET != Emscripten
  std::getline(std::cin, data);
#elif defined(WASM_INPUT)
  data = WASM_INPUT;
#else
  int x = EM_ASM_INT({
    var str = prompt("Input:");
    var buffer = Module._malloc(str.length + 1);
  	Module.stringToUTF8(str, buffer, str.length + 1);
  	return buffer;
  });
  data = (char*) x;
#endif
  vm->push(Value(data));
}
