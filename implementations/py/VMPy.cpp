#include <VM.h>

VM vm;
std::vector<Value>* mem = new std::vector<Value>();

extern "C" void init() {
  vm.attachMem(mem);
}

extern "C" void run(char opcode) {
  vm.run1(opcode);
}

extern "C" void runWithString(char opcode, const char* data) {
  vm.run1(opcode, data);
}

extern "C" void runWithNumber(char opcode, const char* data) {
  vm.run1(opcode, NUMBER(data));
}
