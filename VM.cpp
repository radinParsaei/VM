#include "VM.h"

VM::VM(){
  running = true;
  recsize = 0;
}

std::vector<value> VM::getStack(){
  return stack;
}

void VM::setStack(std::vector<value> v){
  stack = v;
}

std::string strReplace(std::string str, std::string from, std::string to) {
  if(from.empty())
    return str;
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  return str;
}

bool VM::disassemble(int prog, value val, std::string end){
  switch (prog) {
    case EXIT:
      std::cout << "EXIT" << end;
      break;
    case PUT:
      std::cout << "PUT" << "\t";
      if(getValType(val) == TYPE_NUM){
        std::cout << "NUM" << std::get<double>(val);
      } else {
        std::cout << "TXT" << std::get<std::string>(val);
      }
      std::cout << end;
      return 1;
    case ADD:
      std::cout << "ADD" << end;
      break;
    case SUB:
      std::cout << "SUB" << end;
      break;
    case MUL:
      std::cout << "MUL" << end;
      break;
    case DIV:
      std::cout << "DIV" << end;
      break;
    case MOD:
      std::cout << "MOD" << end;
      break;
    case DLCALL:
      std::cout << "DLCALL" << end;
      break;
    case RUN:
      std::cout << "RUN" << end;
      break;
    case REC:
      std::cout << "REC" << end;
      break;
    case END:
      std::cout << "END" << end;
      break;
    case PRINT:
      std::cout << "PRINT" << end;
      break;
    case POP:
      std::cout << "POP" << end;
      break;
    case LOGSTCK:
      std::cout << "LOGSTCK" << end;
      break;
    case PRINTLN:
      std::cout << "PRINTLN" << end;
      break;
    case REPEAT:
      std::cout << "REPEAT" << end;
      break;
    case EQ:
      std::cout << "EQ" << end;
      break;
    case FEQ:
      std::cout << "FEQ" << end;
      break;
    case GT:
      std::cout << "GT" << end;
      break;
    default:
      std::cout << "???" << end;
      break;
    return 0;
  }
}

void VM::printStack(){
  std::cout << "[";
  for(int i = 0; i < stack.size(); i++){
    std::cout << val2str(stack[i]) << ((i + 1) == stack.size()? "":", ");
  }
  std::cout << "]";
}

void VM::run(std::vector<value> prog, bool forceRun, int pc) {
  if(forceRun)running = true;
  for (; pc < prog.size(); pc++) {//fetch
    if(running)pc += run1(std::get<double>(prog[pc]), (prog.size() - 1) == pc? 0:prog[pc + 1]);
    else break;
	}
}

value VM::pop(){
  value v = stack[stack.size() - 1];
  stack.pop_back();
  return v;
}

bool VM::getValType(value v){
  if(std::get_if<double>(&v) != 0){
    return TYPE_NUM;
  } else {
    return TYPE_TEXT;
  }
}

std::string VM::val2str(value v){
  auto ptr = std::get_if<double>(&v);
  if(ptr != 0){
    std::ostringstream strs;
    strs << *ptr;
    return strs.str();
  } else {
    return *std::get_if<std::string>(&v);
  }
}

value VM::add2val(value v1, value v2){
  if(getValType(v1) == TYPE_TEXT || getValType(v2) == TYPE_TEXT){
    std::ostringstream strs;
    strs << val2str(v1) << val2str(v2);
    return strs.str();
  } else {
    return std::get<double>(v1) + std::get<double>(v2);
  }
}

value VM::sub2val(value v1, value v2){
  if(getValType(v1) == TYPE_TEXT || getValType(v2) == TYPE_TEXT){
    return strReplace(val2str(v1), val2str(v2), "");
  } else {
    return std::get<double>(v1) - std::get<double>(v2);
  }
}

value VM::mul2val(value v1, value v2){
  if(getValType(v1) + getValType(v2) == 1){
    std::string s = getValType(v1) == TYPE_TEXT? std::get<std::string>(v1) : std::get<std::string>(v2);
    std::string res = s;
    int i = getValType(v1) == TYPE_NUM? std::get<double>(v1) : std::get<double>(v2);
    for (int j = 1; j < i; j++) {
      res += s;
    }
    return res;
  } else if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) * std::get<double>(v2);
  } else {
    std::cerr << "STR * STR ????\n";
    return 0;
  }
}

value VM::div2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) / std::get<double>(v2);
  } else {
    std::cerr << "STR in / ????\n";
    return 0;
  }
}

value VM::mod2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return ((int)std::get<double>(v1)) % ((int)std::get<double>(v2));
  } else {
    std::cerr << "STR in % ????\n";
    return 0;
  }
}

value VM::isEQ(value v1, value v2){
  if(val2str(v1) == val2str(v2)){
    return 1;
  }
  return 0;
}

value VM::isFEQ(value v1, value v2){
  if(getValType(v1) == getValType(v2) && val2str(v1) == val2str(v2)){
    return 1;
  }
  return 0;
}

value VM::isGT(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) > std::get<double>(v2);
  }
  std::cerr << "STR in < ????";
  return 0;
}

double VM::toNUM(value v){
  if(getValType(v) == TYPE_NUM)return std::get<double>(v);
  else return std::stof(std::get<std::string>(v));
}

bool VM::run1(int prog, value arg){
  if(rec){
    if(prog == END){
      rec--;
      if(rec == 0){
        stack.push_back(recsize);
        recsize = 0;
      } else {
        stack.push_back(prog);
        recsize++;
      }
    } else {
      stack.push_back(prog);
      recsize++;
      if(prog == PUT){
        stack.push_back(arg);
        recsize++;
        return 1;
      } else if(prog == REC){
        rec++;
      }
    }
    return 0;
  }
#ifdef VM_DISASSEMBLE
  disassemble(prog, arg);
#ifdef VM_STACKLOG
  std::cout << "\t\t\t";
#else
  std::cout << std::endl;
#endif
#endif
#ifdef VM_STACKLOG
  printStack();
  std::cout << std::endl;
#endif
  bool res = 0;
  switch (prog) {
    case EXIT:
      running = false;
      exit_code = toNUM(pop());
      if(autoKill)exit(exit_code);
      break;
    case PUT:
      stack.push_back(arg);
      res = 1;
      break;
    case ADD:
      stack.push_back(add2val(pop(), pop()));
      break;
    case SUB:
      stack.push_back(sub2val(pop(), pop()));
      break;
    case MUL:
      stack.push_back(mul2val(pop(), pop()));
      break;
    case DIV:
      stack.push_back(div2val(pop(), pop()));
      break;
    case MOD:
      stack.push_back(mod2val(pop(), pop()));
      break;
    case PRINT:
      std::cout << val2str(pop());
      break;
    case REC:
      rec++;
      break;
    case RUN: {
      std::vector<value> prog;
      int ps = std::get<double>(pop());
      for(int i = 0; i < ps; i++){
        prog.insert(prog.begin(), pop());
      }
      run(prog);
      break;
    }
    case DLCALL: {
      void* lib;
      dlfunc fn;
      lib = dlopen(val2str(pop()).c_str(), RTLD_LAZY);
      if(!lib){
        std::cerr << "CANNOT OPEN LIBRARY\n";
        std::cerr << dlerror();
        break;
      }
      dlerror();//clear errors
      fn = ((dlfunc)dlsym(lib, val2str(pop()).c_str()));
      stack = fn(stack);
      dlclose(lib);
      break;
    }
    case POP:
      stack.pop_back();
      break;
    case LOGSTCK:
      printStack();
      break;
    case PRINTLN:
      std::cout << std::endl;
      break;
    case REPEAT: {
      int count = toNUM(pop());
      std::vector<value> prog;
      int ps = std::get<double>(pop());
      for(int i = 0; i < ps; i++){
        prog.insert(prog.begin(), pop());
      }
      for(; count > 0; count--){
        run(prog);
      }
      break;
    }
    case EQ:
      stack.push_back(isEQ(pop(), pop()));
      break;
    case FEQ:
      stack.push_back(isFEQ(pop(), pop()));
      break;
    case GT:
      stack.push_back(isGT(pop(), pop()));
      break;
  }
  return res;
}
