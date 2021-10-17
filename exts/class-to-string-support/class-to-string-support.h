//VM class-to-string-support
#ifdef NAMED_MEMORY_AND_FUNCTION
inline bool VM_ext_run_class_to_string_support(char opcode, Value arg, VM* vm) {
    if (opcode == opcode_PRINT && vm->getStack()[vm->getStack().size() - 1].getType() == Ptr) {
        Value instance = vm->getStack()[vm->getStack().size() - 1];
        if (vm->getMemoryPointer()->size() > instance.getLong() && vm->getMemoryPointer()->at(instance.getLong()).getType() == VALUE_TYPE_TEXT) {
            Value tmp = vm->getMemoryPointer()->at(instance.getLong());
            tmp = Value("#C") + tmp + "toString";
            if (functionNames.find(tmp.getString()) != functionNames.end()) {
                vm->pop();
                vm->push(tmp);
                vm->run1(opcode_CALLFN);
            } else {
                Value::print(tmp);
                Value::print(" Not Found\n");
            }
        }
    }
    return false;
}
#endif
