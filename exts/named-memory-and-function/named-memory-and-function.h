//VM named-memory-and-function
#include <map>
inline std::map<std::string, long> variableNames;

inline bool VM_ext_run_vm_named_memory_and_function(char opcode, Value arg, VM* vm) {
    if (opcode == MEMGET || opcode == MEMSET || opcode == MEMDEL || opcode == MEMINS) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        std::cout << tmp << std::endl;
        if (tmp.getType() == VALUE_TYPE_TEXT) {
            std::string tmp2 = tmp.toString();
            if (variableNames.find(tmp2) == variableNames.end()) {
                return false;
            }
            vm->pop();
            vm->push(variableNames[tmp2]);
        }
    } else if (opcode == DLCALL) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.startsWith("nmf")) { //nmf -> named memory and function
            tmp.substring(3);
            char tmp1 = tmp.find(":").getLong();
            Value tmp2 = tmp;
            tmp2.substring((int)(tmp1 + 1));
            tmp.substring(0, (int)tmp1);
            tmp2.toNum();
            variableNames.insert(std::make_pair(tmp.toString(), tmp2.getLong()));
            return true;
        }
    }
    return false;
}
