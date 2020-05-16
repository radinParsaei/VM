#include "VM.h"

using namespace std;

int main(int argc, char const **argv) {
  vector<Value> prog;
  string line;
  VM *vm = new VM();
  vector<Value> mem;
  vm->attachMem(&mem);
  while(getline(cin, line)){
    for(Value val : VM::assemble(Utils::stringDuplicate(line.c_str()))){
      prog.push_back(val);
    }
    vm->autoKill = true;
    vm->run(prog);
    prog.clear();
  }
  return 0;
}
