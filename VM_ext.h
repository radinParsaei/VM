#ifndef CALL_EXT
#define CALL_EXT(name) res |= VM_ext_run_ ## name (prog, arg, vm);
#define EXTENSIONS inline bool VM_ext_run(char prog, Value arg, VM* vm) {\
              bool res = false;
#define END_EXTS return res;\
              }
#endif
