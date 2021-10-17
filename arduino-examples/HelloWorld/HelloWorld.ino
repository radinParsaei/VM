#include "VM.h"

#undef min
#undef max

#ifdef ARDUINO_SAM_DUE
extern "C" int _gettimeofday() {
    return 0;
}
extern "C" void exit(int) {
  while(1);
}
extern "C" void _exit(int) {
  while(1);
}
extern "C" void _kill(int) {}
extern "C" void _getpid(int) {}
#endif

//uncomment these lines if target is STM32
//extern "C" int _gettimeofday() {
//    return 0;
//}

VM vm;

void setup() {
  Serial.begin(9600);
  vm.run({ opcode_PUT, "Hello World\n", opcode_PRINT });
}

void loop() {}
