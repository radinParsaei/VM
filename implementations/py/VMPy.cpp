#include <VM.h>

VM vm;
std::vector<Value>* mem = new std::vector<Value>();

extern "C" void init() {
  vm.attachMem(mem);
}

extern "C" void runWithString(char opcode, const char* data) {
  vm.run1(opcode, data);
}

extern "C" void runWithBoolean(char opcode, bool data) {
  vm.run1(opcode, data);
}

extern "C" void runWithNull(char opcode) {
  vm.run1(opcode, null);
}

extern "C" void runWithNumber(char opcode, const char* data) {
  vm.run1(opcode, NUMBER(data));
}
