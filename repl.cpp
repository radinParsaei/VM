#include "VM.h"

using namespace std;

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

char* opcodes[] = {
  "EXIT",
  "PUT",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "MOD",
  "PRINT",
  "DLCALL",
  "REC",
  "END",
  "RUN",
  "POP",
  "LOGSTCK",
  "PRINTLN",
  "REPEAT",
  "EQ",
  "FEQ",
  "GT",
  "GE",
  "LT",
  "LE",
  "LAND",
  "LOR",
  "AND",
  "OR",
  "NOT",
  "LNOT",
  "LSHIFT",
  "RSHIFT",
  "XOR",
  "NEG",
  "BREAK",
  "WTRUN",
  "WFRUN",
  "IFTRUN",
  "IFFRUN",
  "THREAD",
  "MEMSET",
  "MEMGET",
  "MEMSIZE",
  "MEMPUT",
  "MEMINS",
  "MEMDEL",
  "TONUM",
  "TOTXT",
  "ISNUM",
  "CANNUM",
  NULL
};

char* PUT_params[] = { "NUM", "TXT", NULL };

char* completion_generator(const char* text, int state) {
  static uint8_t i, len;
  char* name;
  if (!state) {
    i = 0;
    len = Utils::stringLength(text) - 1;
  }
  if (Utils::find(rl_line_buffer, "PUT ") != -1 && Utils::find(rl_line_buffer, "NUM") == -1 && Utils::find(rl_line_buffer, "TXT") == -1) {
    while ((name = PUT_params[i++])) {
      if (strncmp(name, Utils::toUpper(text), len) == 0) {
        return Utils::stringDuplicate(name);
      }
    }
  } else {
    while ((name = opcodes[i++])) {
      if (strncmp(name, Utils::toUpper(text), len) == 0) {
        return Utils::stringDuplicate(name);
      }
    }
  }
  return NULL;
}

char** completion(const char* text, int start, int end) {
  rl_attempted_completion_over = 1;
  return rl_completion_matches(text, completion_generator);
}

int main(int argc, char const **argv) {
  rl_attempted_completion_function = completion;
  vector<Value> prog;
  VM *vm = new VM();
  vector<Value> mem;
  vm->attachMem(&mem);
#ifdef USE_READLINE
  while (true) {
    char* line = readline("> ");
    if (!line) exit(0);
    add_history(line);
    for(Value val : VM::assemble(line)){
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
