#include "VM.h"

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

using namespace std;

int main(int argc, char const **argv) {
  vector<Value> prog;
  VM *vm = new VM();
  vector<Value> mem;
  vm->attachMem(&mem);
#ifdef USE_READLINE
  while (true) {
    char* line = readline("> ");
    if (!line) exit(0);
    add_history(line);
    for(Value val : VM::assemble(Utils::stringDuplicate(line))){
#else
  string line;
  while(getline(cin, line)){
    for(Value val : VM::assemble(Utils::stringDuplicate(line.c_str()))){
#endif
      prog.push_back(val);
    }
    vm->autoKill = true;
    vm->run(prog);
    prog.clear();
  }
  return 0;
}
