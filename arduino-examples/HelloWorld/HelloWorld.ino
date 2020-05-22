#include "VM.h"

VM vm;

void setup() {
  Serial.begin(9600);
  vm.run({PUT, "Hello World", PRINT, PRINTLN});
}

void loop() {}
