#include "VM.h"

VM* vm = new VM();

void setup() {
  Serial.begin(9600);
  vm->run1(REC);
  vm->run1(PUT, "Thread\n");
  vm->run1(PRINT);
  vm->run1(END);
  vm->run1(THREAD);
  vm->run1(PUT, "Hello\n");
  vm->run1(PRINT);
  delete vm;
  Serial.println();
}

void loop() {}
