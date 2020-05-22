#include "VM.h"

VM vm;

void setup() {
  Serial.begin(9600);
  vm.run1(REC);
  vm.run1(PUT, "Thread Message");
  vm.run1(PRINT);
  vm.run1(PRINTLN);
  vm.run1(END);
  vm.run1(THREAD);
  vm.run1(PUT, "Hello World!");
  vm.run1(PRINT);
  vm.run1(PRINTLN);
}

void loop() {}
