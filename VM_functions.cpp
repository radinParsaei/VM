#include "VM_functions.h"

value pop(){
  value v = stack[stack.size() - 1];
  stack.pop_back();
  return v;
}

void DLCALL_task(){
  void* lib;
  dlfunc fn;
  lib = dlopen(VM::val2str(pop()).c_str(), RTLD_LAZY);
  if(!lib){
    std::cerr << "CANNOT OPEN LIBRARY\n";
    std::cerr << dlerror();
    return;
  }
  dlerror();//clear errors
  std::string fnName = VM::val2str(pop());
  std::ostringstream stream;
  stream << "_Z" << fnName.size() << fnName << "St6vectorISt7variantIJdNSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEEEESaIS7_EE";
  fn = ((dlfunc)dlsym(lib, stream.str().c_str()));
  stack = fn(stack);
  dlclose(lib);
}

void RUN_task(){
  VM vm;
  std::vector<value> prog;
  int ps = std::get<double>(pop());
  for(int i = 0; i < ps; i++){
    prog.insert(prog.begin(), pop());
  }
  vm.setStack(stack);
  vm.autoKill = true;
  vm.run(prog);
  stack = vm.getStack();
}

void POP_task(){
  stack.pop_back();
}

void LOGSTCK_task(){
  std::cout << "[";
  for(int i = 0; i < stack.size(); i++){
    std::cout << VM::val2str(stack[i]) << ((i + 1) == stack.size()? "":", ");
  }
  std::cout << "]";
}

void PRINTLN_task(){
  std::cout << std::endl;
}

void PRINT_task(){
  std::cout << VM::val2str(pop());
}

void REPEAT_task(){
  VM vm;
  int count = VM::toNUM(pop());
  std::vector<value> prog;
  int ps = std::get<double>(pop());
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

void EXIT_task(){
  exit(VM::toNUM(pop()));
}

void ADD_task(){
  stack.push_back(VM::add2val(pop(), pop()));
}

void SUB_task(){
  stack.push_back(VM::sub2val(pop(), pop()));
}

void MUL_task(){
  stack.push_back(VM::mul2val(pop(), pop()));
}

void DIV_task(){
  stack.push_back(VM::div2val(pop(), pop()));
}

void MOD_task(){
  stack.push_back(VM::mod2val(pop(), pop()));
}

void PUT_task(int data){
  stack.push_back(data);
}

void PUT_task(const char* data){
  stack.push_back(std::string(data));
}
