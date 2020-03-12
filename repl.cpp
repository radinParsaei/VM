#include <string.h>
#include <iostream>
#include <vector>
#include <variant>
#include <fstream>
#include "VM.h"
#include "assembler_functions.h"

using namespace std;

int main(int argc, char const **argv) {
  vector<value> prog;
  string line;
  VM *vm = new VM();
  while(getline(cin, line)){
    for(value val : assemble(line)){
      prog.push_back(val);
    }
    vm->autoKill = true;
    vm->run(prog);
    prog.clear();
  }
  return 0;
}
