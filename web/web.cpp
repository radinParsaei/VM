#include "VM.h"
#include <fstream>
#include "VM_binaries.h"

using namespace std;
using namespace VM_BINARIES;

VM vm;
vector<Value> mem;

int main(){
  vm.attachMem(&mem);
  vm.autoKill = false;
}

#include <stdio.h>
#include <emscripten/emscripten.h>

#ifdef __cplusplus
extern "C" {
#endif

void EMSCRIPTEN_KEEPALIVE run(int opcode) {
  vm.run1(opcode, null);
}

void EMSCRIPTEN_KEEPALIVE runWithString(int opcode, char* string) {
  vm.run1(opcode, string);
}

void EMSCRIPTEN_KEEPALIVE runWithNumber(int opcode, char* number) {
  vm.run1(opcode, NUMBER(number));
}

void EMSCRIPTEN_KEEPALIVE runWithBool(int opcode, bool data) {
  vm.run1(opcode, data);
}

const char* EMSCRIPTEN_KEEPALIVE disassemble(int opcode) {
  return vm.disassemble(opcode, null).toString().c_str();
}

const char* EMSCRIPTEN_KEEPALIVE disassembleWithString(int opcode, const char* string) {
  return vm.disassemble(opcode, string).toString().c_str();
}

const char* EMSCRIPTEN_KEEPALIVE disassembleWithNumber(int opcode, const char* number) {
  return vm.disassemble(opcode, NUMBER(number)).toString().c_str();
}

const char* EMSCRIPTEN_KEEPALIVE disassembleWithBool(int opcode, bool data) {
  return vm.disassemble(opcode, data).toString().c_str();
}
#ifdef __cplusplus
}
#endif
