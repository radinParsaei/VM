//VM named-memory-and-function
#define NAMED_MEMORY_AND_FUNCTION
#include <map>

#if __cplusplus >= 201703
static std::map<TEXT, long> variableNames;
static std::map<TEXT, long> functionNames;
#else
static std::map<TEXT, long> variableNames;
static std::map<TEXT, long> functionNames;
#endif

inline bool VM_ext_run_vm_named_memory_and_function(char opcode, Value arg, VM* vm) {
    if (!(vm->getStack().size() > 0)) return false;
    if (opcode == opcode_MEMGET || opcode == opcode_MEMSET || opcode == opcode_MEMDEL || opcode == opcode_MEMINS) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.getType() == VALUE_TYPE_TEXT) {
            TEXT tmp2 = tmp.toString();
            if (variableNames.find(tmp2) == variableNames.end()) {
                return false;
            }
            vm->set(vm->getStack().size() - 1, variableNames[tmp2]);
        }
    } else if (opcode == opcode_CALLFN || opcode == opcode_MKFN) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.getType() == VALUE_TYPE_TEXT) {
            TEXT tmp2 = tmp.toString();
            if (functionNames.find(tmp2) == functionNames.end()) {
                return false;
            }
            vm->set(vm->getStack().size() - 1, functionNames[tmp2]);
        }
    } else if (opcode == opcode_DLCALL) {
        Value tmp = vm->getStack()[vm->getStack().size() - 1];
        if (tmp.startsWith("nm")) { //nm -> name memory
            vm->pop();
            tmp.substring(2);
            char tmp1 = tmp.find(":").getLong();
            while (tmp.find(":", (int)++tmp1).getLong() != -1);
            tmp1--;
            Value tmp2 = tmp;
            tmp2.substring((int)(tmp1 + 1));
            tmp.substring(0, (int)tmp1);
            tmp2.toNum();
            variableNames.insert(std::make_pair(tmp.toString(), tmp2.getLong()));
            return true;
        } else if (tmp.startsWith("nf")) { //nf -> name function
            vm->pop();
            tmp.substring(2);
            char tmp1 = tmp.find(":").getLong();
            while (tmp.find(":", (int)++tmp1).getLong() != -1);
            tmp1--;
            Value tmp2 = tmp;
            tmp2.substring((int)(tmp1 + 1));
            tmp.substring(0, (int)tmp1);
            tmp2.toNum();
            functionNames.insert(std::make_pair(tmp.toString(), tmp2.getLong()));
            return true;
        } else if (tmp == "gpm") { //gpm -> get pointer of memory name
            vm->pop();
            Value tmp = vm->getStack()[vm->getStack().size() - 1];
            if (tmp.getType() == VALUE_TYPE_TEXT) {
                TEXT tmp2 = tmp.toString();
                if (variableNames.find(tmp2) == variableNames.end()) {
                    return false;
                }
                vm->set(vm->getStack().size() - 1, variableNames[tmp2]);
            }
            return true;
        } else if (tmp == "gpf") { //gpf -> get pointer of function name
            vm->pop();
            Value tmp = vm->getStack()[vm->getStack().size() - 1];
            if (tmp.getType() == VALUE_TYPE_TEXT) {
                TEXT tmp2 = tmp.toString();
                if (functionNames.find(tmp2) == functionNames.end()) {
                    return false;
                }
                vm->set(vm->getStack().size() - 1, functionNames[tmp2]);
            }
            return true;
        }
    }
    return false;
}
