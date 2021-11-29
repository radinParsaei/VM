//VM string-functions
#ifdef NAMED_MEMORY_AND_FUNCTION
inline bool VM_ext_run_string_functions(char opcode, Value arg, VM* vm) {
    if (opcode == DLCALL && vm->getStack()[vm->getStack().size() - 1] == "chkstr:" && vm->getStack()[vm->getStack().size() - 2].getType() == VALUE_TYPE_TEXT) {
        vm->pop();
        Value text = vm->pop();
        Value function = vm->pop();
        if (function == "toLower" || function == "toLowerCase") {
          vm->push(text.toLower());
          vm->run1(PUT, 5);
          vm->run1(SKIP);
          return true;
        } else if (function == "toUpper" || function == "toUpperCase") {
          vm->push(text.toUpper());
          vm->run1(PUT, 5);
          vm->run1(SKIP);
          return true;
        // } else if (function == "charAt") {
        //   vm->push(text[]);
        //   vm->run1(PUT, 5);
        //   vm->run1(SKIP);
        //   return true;
        }
    }
    return false;
}
#endif
