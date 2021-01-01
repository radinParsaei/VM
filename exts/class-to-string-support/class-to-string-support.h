//VM class-to-string-support
#ifdef NAMED_MEMORY_AND_FUNCTION
inline bool VM_ext_run_class_to_string_support(char opcode, Value arg, VM* vm) {
    if (opcode == PRINT && vm->getStack()[vm->getStack().size() - 1].getType() == Ptr) {
        Value instance = vm->getStack()[vm->getStack().size() - 1];
        if (vm->getMemoryPointer()->size() > instance.getLong() && vm->getMemoryPointer()->at(instance.getLong()).getType() == VALUE_TYPE_TEXT) {
            std::string tmp = vm->getMemoryPointer()->at(instance.getLong()).getString();
            tmp = "#C" + tmp + "toString";
            if (functionNames.find(tmp) != functionNames.end()) {
                vm->pop();
                vm->push(tmp);
                vm->run1(CALLFN);
            } else {
                std::cout << tmp << " Not Found\n";
            }
        }
    }
    return false;
}
#endif
