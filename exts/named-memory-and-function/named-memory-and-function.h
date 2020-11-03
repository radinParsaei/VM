//VM named-memory-and-function
#include <map>
inline std::map<std::string, long> variableNames;
inline std::map<std::string, long> functionNames;

inline bool VM_ext_run_vm_named_memory_and_function(char opcode, Value arg, VM* vm) {
    if (opcode == MEMGET || opcode == MEMSET || opcode == MEMDEL || opcode == MEMINS) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.getType() == VALUE_TYPE_TEXT) {
            std::string tmp2 = tmp.toString();
            if (variableNames.find(tmp2) == variableNames.end()) {
                return false;
            }
            vm->set(vm->getStack().size() - 1, variableNames[tmp2]);
        }
    } else if (opcode == CALLFN || opcode == MKFN) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.getType() == VALUE_TYPE_TEXT) {
            std::string tmp2 = tmp.toString();
            if (functionNames.find(tmp2) == functionNames.end()) {
                return false;
            }
            vm->set(vm->getStack().size() - 1, functionNames[tmp2]);
        }
    } else if (opcode == DLCALL) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.startsWith("nm")) { //nm -> name memory
            tmp.substring(2);
            char tmp1 = tmp.find(":").getLong();
            Value tmp2 = tmp;
            tmp2.substring((int)(tmp1 + 1));
            tmp.substring(0, (int)tmp1);
            tmp2.toNum();
            variableNames.insert(std::make_pair(tmp.toString(), tmp2.getLong()));
            return true;
        } else if (tmp.startsWith("nf")) { //nf -> name function
            tmp.substring(2);
            char tmp1 = tmp.find(":").getLong();
            Value tmp2 = tmp;
            tmp2.substring((int)(tmp1 + 1));
            tmp.substring(0, (int)tmp1);
            tmp2.toNum();
            functionNames.insert(std::make_pair(tmp.toString(), tmp2.getLong()));
            return true;
        }
    }
    return false;
}
