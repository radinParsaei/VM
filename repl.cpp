#include "VM.h"

using namespace std;

#ifdef USE_READLINE
#include <readline/readline.h>
#include <readline/history.h>

char* opcodes[] = {
  "EXIT",
  "PUT",
  "POP",
  "STCKGET",
  "STCKMOV",
  "STCKDEL",
  "ADD",
  "SUB",
  "MUL",
  "DIV",
  "MOD",
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
  "TOBOOL",
  "REC",
  "END",
  "RUN",
  "REPEAT",
  "BREAK",
  "IFTRUN",
  "IFFRUN",
  "WTRUN",
  "WFRUN",
  "THREAD",
  "SKIP",
  "IFSKIP",
  "MKFN",
  "CALLFN",
  "EXITFN",
  "DLCALL",
  "PRINT",
  NULL
};

char* PUT_params[] = { "NUM", "TXT", "BOOL", "NULL", NULL };

char* completion_generator(const char* text, int state) {
  if (Value(rl_line_buffer).trim().startsWith("PUT") && (Value(rl_line_buffer).find("TXT") != -1 || Value(rl_line_buffer).find("NUM") != -1)) return NULL;
  else if (Value(rl_line_buffer).trim().find("PUT") == -1 && Value(rl_line_buffer).find(" ") != -1) return NULL;
  static uint8_t i, len;
  char* name;
  if (!state) {
    i = 0;
    len = strlen(text);
  }
  if (Value(rl_line_buffer).trim().startsWith("PUT") && Value(rl_line_buffer).find("NUM") == -1 && Value(rl_line_buffer).find("TXT") == -1) {
    while ((name = PUT_params[i++])) {
      if (strncmp(name, Value(text).toUpper().toString().c_str(), len) == 0) {
        return strdup(name);
      }
    }
  } else {
    while ((name = opcodes[i++])) {
      if (strncmp(name, Value(text).toUpper().toString().c_str(), len) == 0) {
        return strdup(name);
      }
    }
  }
  return NULL;
}

char** completion(const char* text, int start, int end) {
  rl_attempted_completion_over = 1;
  return rl_completion_matches(text, completion_generator);
}
#endif

int main(int argc, char const **argv) {
  vector<Value> prog;
  VM *vm = new VM();
  vector<Value> mem;
  vm->attachMem(&mem);
#ifdef USE_READLINE
  rl_attempted_completion_function = completion;
  while (true) {
    char* line = readline("> ");
    if (!line) {
      delete vm;
      exit(0);
    }
    if (Value(line).trim() != "") add_history(line);
    for(Value val : VM::assemble(line)) {
#else
  string line;
  while(getline(cin, line)){
    for(Value val : VM::assemble(line)) {
#endif
      prog.push_back(val);
    }
    vm->autoKill = true;
    vm->run(prog);
    prog.clear();
  }
  delete vm;
  return 0;
}
