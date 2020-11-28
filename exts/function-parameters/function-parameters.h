//VM function parameter holder
#include <map>
#include <vector>
inline std::map<int, std::vector<long>> parameters;

inline bool VM_ext_run_vm_function_pointers(char opcode, Value arg, VM* vm) {
    if (opcode == MKFN) {
        std::vector<Value> vmStack = vm->getStack();
        if (vmStack[vmStack.size() - 1].getType() == VALUE_TYPE_TEXT) {
            Value function = vmStack[vmStack.size() - 1] + " ";
            int i = 0;
            while (isdigit(function.charAt(i))) i++;
            Value functionCode = function.substring(0, i);
            vm->set(vmStack.size() - 1, functionCode.toNum());
            std::vector<long> parametersVector;
            parametersVector.reserve(50); //reserve memory for 50 params (not limited but make vector acts faster)
            parameters.insert(std::make_pair(functionCode.getLong(), parametersVector));
            i = 0;
            while (i != function.getString().size() - 1) {
                if (!isdigit(function.charAt(i))) i++;
                else {
                    function.substring(i);
                    i = 0;
                    while (i != function.getString().size() - 1 && isdigit(function.charAt(i))) i++;
                    Value tmp = function;
                    tmp.substring(0, i);
                    parameters[functionCode.getLong()].push_back(tmp.toNum().getLong());
                }
            }
        }
    } else if (opcode == DLCALL) {
        std::vector<Value> vmStack = vm->getStack();
        if (vmStack.size() > 0 && vmStack[vmStack.size() - 1].getType() == VALUE_TYPE_TEXT) {
            Value tmp = vmStack[vmStack.size() - 1];
            if (tmp.startsWith("fp:")) {
                tmp.substring(3);
                if (tmp.startsWith("l:")) { //length
                    tmp.substring(2);
                    vm->push((long)parameters[tmp.toNum().getLong()].size());
                    return true;
                } else if (tmp.startsWith("g:")) { //get
                    tmp.substring(2);
                    int tmp2 = tmp.substring(0, tmp.find(":") + 1).toNum().getLong();
                    vm->push((long)parameters[tmp2][tmp.substring(1).toNum().getLong()]);
                    return true;
                } else if (tmp.startsWith("s:")) { //set
                    tmp.substring(2);
                    vm->pop();
                    for (long l : parameters[tmp.toNum().getLong()]) {
                        if (vm->getMemoryPointer()->size() < (l + 1)) vm->getMemoryPointer()->resize(l + 1);
                        vm->getMemoryPointer()->at(l) = vm->pop();
                    }
                    return true;
                } else if (tmp.startsWith("c:")) { //call
                    tmp.substring(2);
                    vm->pop();
                    for (long l : parameters[tmp.toNum().getLong()]) {
                        if (vm->getMemoryPointer()->size() < (l + 1)) vm->getMemoryPointer()->resize(l + 1);
                        vm->getMemoryPointer()->at(l) = vm->pop();
                    }
                    vm->run1(PUT, tmp.toNum());
                    vm->run1(CALLFN);
                    return true;
                }
            }
        }
    }
    return false;
}
