#include "VM.h"

VM vm;

void setup() {
  Serial.begin(9600);
  std::vector<Value>* mem = new std::vector<Value>();
  vm.attachMem(mem);
  vm.run1(PUT, "Data");
  vm.run1(MEMPUT);
  vm.run1(PUT, 0);
  vm.run1(MEMGET);
  vm.run1(PRINT);
  vm.run1(PUT, "\n");
  vm.run1(PRINT);
}

void loop() {}
