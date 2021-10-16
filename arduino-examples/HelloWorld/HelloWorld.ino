#include "VM.h"

#ifdef ARDUINO_SAM_DUE
extern "C" int _gettimeofday() {
    return 0;
}
#endif

VM vm;

void setup() {
  Serial.begin(9600);
  vm.run({ PUT, "Hello World\n", PRINT });
}

void loop() {}
