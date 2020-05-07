#include "VM_functions.h"

Value pop(){
  Value v = stack[stack.size() - 1];
  stack.pop_back();
  return v;
}

extern "C" void TOTXT_task(){
  stack.push_back(pop().toString());
}

extern "C" void TONUM_task(){
  stack.push_back(VM::toNUM(pop()));
}

extern "C" void CANNUM_task(){
  if(stack.size() == 0) return;
  Value v = stack[stack.size() - 1];
  if(!v.getType()){
    stack.push_back(1);
  } else {
    int64_t i = 0;
    for(; v.getString()[i] != 0; i++){
      if(!isdigit(v.getString()[i])){
        stack.push_back(0);
        return;
      }
    }
    stack.push_back(1);
  }
}

extern "C" void ISNUM_task(){
  if(stack.size() == 0) return;
  stack.push_back(!stack[stack.size() - 1].getType());
}

extern "C" void MEMPUT_task(){
  mem->push_back(pop());
}

extern "C" void MEMINS_task(){
  mem->insert(mem->begin() + pop().getLong(), pop());
}

extern "C" void MEMDEL_task(){
  mem->erase(mem->begin() + pop().getLong());
}

extern "C" void MEMSIZE_task(){
  stack.push_back((int)mem->size());
}

extern "C" void MEMSET_task(){
  int n = pop().getLong();
  if(mem->size() < (n + 1))mem->resize(n + 1);
  mem->at(n) = pop();
}

extern "C" void MEMGET_task(){
  stack.push_back(mem->at(pop().getLong()));
}

extern "C" void THREAD_task(){
  std::vector<Value> prog;
  int ps = pop().getLong();
  for(; ps > 0; ps--){
    prog.insert(prog.begin(), pop());
  }
  std::thread t([=]{
    VM vm;
    vm.attachMem(mem);
    vm.run(prog);
  });
  t.detach();
}

extern "C" void DLCALL_task(){
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
  HINSTANCE hinstLib;
  dlfunc fn;
  std::string s = pop().getString();
  hinstLib = LoadLibraryA(s.c_str());
  if(hinstLib != NULL){
    fn = (dlfunc) GetProcAddress(hinstLib, (LPCSTR)pop().getString().c_str());
    if (NULL != fn){
      stack = fn(stack);
    } else {
      std::cerr << "SYMBOL NOT FOUND" << GetLastError();
    }
    FreeLibrary(hinstLib);
  } else {
    std::cerr << "CANNOT OPEN LIBRARY\n";
    std::cerr << GetLastError();
  }
#else
  void* lib;
  dlfunc fn;
  lib = dlopen(pop().toString(), RTLD_LAZY);
  if(!lib){
    std::cerr << "CANNOT OPEN LIBRARY\n";
    std::cerr << dlerror();
    return;
  }
  dlerror();//clear errors
  fn = ((dlfunc)dlsym(lib, pop().toString()));
  stack = fn(stack);
  dlclose(lib);
#endif
}

extern "C" void RUN_task(){
  VM vm;
  vm.attachMem(mem);
  std::vector<Value> prog;
  int ps = pop().getLong();
  for(int i = 0; i < ps; i++){
    prog.insert(prog.begin(), pop());
  }
  vm.setStack(stack);
  vm.autoKill = true;
  vm.run(prog);
  stack = vm.getStack();
}

extern "C" void POP_task(){
  stack.pop_back();
}

extern "C" void LOGSTCK_task(){
  std::cout << "[";
  for(int i = 0; i < stack.size(); i++){
    std::cout << stack[i].toString() << ((i + 1) == stack.size()? "":", ");
  }
  std::cout << "]";
}

extern "C" void PRINTLN_task(){
  std::cout << std::endl;
}

extern "C" void PRINT_task(){
  std::cout << pop().toString();
}

extern "C" void REPEAT_task(){
  VM vm;
  vm.attachMem(mem);
  int count = VM::toNUM(pop()).toLong();
  std::vector<Value> prog;
  int ps = pop().getLong();
  for(int i = 0; i < ps; i++){
    prog.insert(prog.begin(), pop());
  }
  vm.setStack(stack);
  for(; count > 0; count--){
    vm.autoKill = true;
    vm.run(prog);
    stack = vm.getStack();
  }
}

extern "C" void WTRUN_task(){
  if(stack.size() < 2)return;
  if(!stack[stack.size() - 1].getType()){
    bool tos = stack[stack.size() - 1].getLong();
    if(tos){
      stack.pop_back();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--){
        prog.insert(prog.begin(), pop());
      }
      while(tos){
        VM vm;
        vm.attachMem(mem);
        vm.setStack(stack);
        vm.autoKill = true;
        vm.run(prog);
        stack = vm.getStack();
        tos = stack[stack.size() - 1].getLong();
        if(tos)stack.pop_back();
      }
    }
  }
}


extern "C" void IFTRUN_task(){
  if(stack.size() < 2)return;
  if(!stack[stack.size() - 1].getType()){
    bool tos = stack[stack.size() - 1].getLong();
    if(tos){
      stack.pop_back();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--){
        prog.insert(prog.begin(), pop());
      }
      VM vm;
      vm.attachMem(mem);
      vm.setStack(stack);
      vm.autoKill = true;
      vm.run(prog);
      stack = vm.getStack();
    }
  }
}

extern "C" void IFFRUN_task(){
  if(stack.size() < 2)return;
  if(!stack[stack.size() - 1].getType()){
    bool tos = stack[stack.size() - 1].getLong();
    if(!tos){
      stack.pop_back();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--){
        prog.insert(prog.begin(), pop());
      }
      VM vm;
      vm.attachMem(mem);
      vm.setStack(stack);
      vm.autoKill = true;
      vm.run(prog);
      stack = vm.getStack();
    }
  }
}

extern "C" void WFRUN_task(){
  if(stack.size() < 2)return;
  if(!stack[stack.size() - 1].getType()){
    bool tos = stack[stack.size() - 1].getLong();
    if(!tos){
      stack.pop_back();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--){
        prog.insert(prog.begin(), pop());
      }
      while(!tos){
        VM vm;
        vm.attachMem(mem);
        vm.setStack(stack);
        vm.autoKill = true;
        vm.run(prog);
        stack = vm.getStack();
        tos = stack[stack.size() - 1].getLong();
        if(!tos)stack.pop_back();
      }
    }
  }
}

extern "C" void EXIT_task(){
  exit(VM::toNUM(pop()).toLong());
}

extern "C" void ADD_task(){
  stack.push_back(VM::add2val(pop(), pop()));
}

extern "C" void SUB_task(){
  stack.push_back(VM::sub2val(pop(), pop()));
}

extern "C" void MUL_task(){
  stack.push_back(VM::mul2val(pop(), pop()));
}

extern "C" void DIV_task(){
  stack.push_back(VM::div2val(pop(), pop()));
}

extern "C" void MOD_task(){
  stack.push_back(VM::mod2val(pop(), pop()));
}

extern "C" void EQ_task(){
  stack.push_back(VM::isEQ(pop(), pop()));
}

extern "C" void FEQ_task(){
  stack.push_back(VM::isFEQ(pop(), pop()));
}

extern "C" void GT_task(){
  stack.push_back(VM::isGT(pop(), pop()));
}

extern "C" void GE_task(){
  stack.push_back(VM::isGE(pop(), pop()));
}

extern "C" void LT_task(){
  stack.push_back(VM::isLT(pop(), pop()));
}

extern "C" void LE_task(){
  stack.push_back(VM::isLE(pop(), pop()));
}

extern "C" void LAND_task(){
  stack.push_back(VM::LAND2val(pop(), pop()));
}

extern "C" void AND_task(){
  stack.push_back(VM::AND2val(pop(), pop()));
}

extern "C" void LOR_task(){
  stack.push_back(VM::LOR2val(pop(), pop()));
}

extern "C" void OR_task(){
  stack.push_back(VM::OR2val(pop(), pop()));
}

extern "C" void NOT_task(){
  stack.push_back(VM::NOTval(pop()));
}

extern "C" void LNOT_task(){
  stack.push_back(VM::LNOTval(pop()));
}

extern "C" void LSHIFT_task(){
  stack.push_back(VM::LSHIFT2val(pop(), pop()));
}

extern "C" void RSHIFT_task(){
  stack.push_back(VM::RSHIFT2val(pop(), pop()));
}

extern "C" void XOR_task(){
  stack.push_back(VM::XOR2val(pop(), pop()));
}

extern "C" void NEG_task(){
  stack.push_back(VM::NEGval(pop()));
}

extern "C" void PUT_taski(double data){
  stack.push_back(data);
}

extern "C" void PUT_tasks(const char* data){
  stack.push_back(data);
}
