#include "VM.h"

VM::VM(){
  running = true;
  recsize = 0;
}

void VM::attachMem(std::vector<value> *mem){
  mempointer = mem;
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
    case GE:
      std::cout << "GE" << end;
      break;
    case LT:
      std::cout << "LT" << end;
      break;
    case LE:
      std::cout << "LE" << end;
      break;
    case LAND:
      std::cout << "LAND" << end;
      break;
    case LOR:
      std::cout << "LOR" << end;
      break;
    case AND:
      std::cout << "AND" << end;
      break;
    case OR:
      std::cout << "OR" << end;
      break;
    case NOT:
      std::cout << "NOT" << end;
      break;
    case LNOT:
      std::cout << "LNOT" << end;
      break;
    case LSHIFT:
      std::cout << "LSHIFT" << end;
      break;
    case RSHIFT:
      std::cout << "RSHIFT" << end;
      break;
    case XOR:
      std::cout << "XOR" << end;
      break;
    case NEG:
      std::cout << "NEG" << end;
      break;
    case BREAK:
      std::cout << "BREAK" << end;
      break;
    case WTRUN:
      std::cout << "WTRUN" << end;
      break;
    case WFRUN:
      std::cout << "WFRUN" << end;
      break;
    case IFFRUN:
      std::cout << "IFFRUN" << end;
      break;
    case IFTRUN:
      std::cout << "IFTRUN" << end;
      break;
    case THREAD:
      std::cout << "THREAD" << end;
      break;
    case MEMGET:
      std::cout << "MEMGET" << end;
      break;
    case MEMSET:
      std::cout << "MEMSET" << end;
      break;
    case MEMPUT:
      std::cout << "MEMPUT" << end;
      break;
    case MEMSIZE:
      std::cout << "MEMSIZE" << end;
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
    if(isBreaked){
      isBreaked = false;
      break;
    }
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
  std::cerr << "STR in > ????";
  return 0;
}

value VM::isGE(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) >= std::get<double>(v2);
  }
  std::cerr << "STR in >= ????";
  return 0;
}

value VM::isLT(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) < std::get<double>(v2);
  }
  std::cerr << "STR in < ????";
  return 0;
}

value VM::isLE(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) <= std::get<double>(v2);
  }
  std::cerr << "STR in <= ????";
  return 0;
}

value VM::LAND2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) && std::get<double>(v2);
  } else {
    std::cerr << "STR in LOGICAL AND????" << std::endl;
  }
}

value VM::LOR2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return std::get<double>(v1) || std::get<double>(v2);
  } else {
    std::cerr << "STR in LOGICAL OR????" << std::endl;
  }
}

value VM::AND2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return ((int)std::get<double>(v1)) & ((int)std::get<double>(v2));
  } else {
    std::cerr << "STR in BITWISE AND????" << std::endl;
  }
}

value VM::OR2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return ((int)std::get<double>(v1)) | ((int)std::get<double>(v2));
  } else {
    std::cerr << "STR in BITWISE OR????" << std::endl;
  }
}

value VM::NOTval(value v){
  if(!getValType(v)){
    return ~((int)std::get<double>(v));
  } else {
    std::cerr << "STR in BITWISE NOT????" << std::endl;
  }
}

value VM::LNOTval(value v){
  if(!getValType(v)){
    return !std::get<double>(v);
  } else {
    std::cerr << "STR in LOGICAL NOT????" << std::endl;
  }
}

value VM::LSHIFT2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return ((int)std::get<double>(v1)) << ((int)std::get<double>(v2));
  } else {
    std::cerr << "STR in LEFT SHIFT????" << std::endl;
  }
}

value VM::RSHIFT2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return ((int)std::get<double>(v1)) >> ((int)std::get<double>(v2));
  } else {
    std::cerr << "STR in RIGHT SHIFT????" << std::endl;
  }
}

value VM::XOR2val(value v1, value v2){
  if(!(getValType(v1) && getValType(v2))){
    return ((int)std::get<double>(v1)) ^ ((int)std::get<double>(v2));
  } else {
    std::cerr << "STR in XOR????" << std::endl;
  }
}

value VM::NEGval(value v){
  if(!getValType(v)){
    return -std::get<double>(v);
  } else {
    std::string str = std::get<std::string>(v);
    return std::string(str.rbegin(), str.rend());
  }
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
      for(; ps > 0; ps--){
        prog.insert(prog.begin(), pop());
      }
      run(prog);
      break;
    }
    case DLCALL: {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
      HINSTANCE hinstLib;
      dlfunc fn;
      std::string s = std::get<std::string>(pop());
      hinstLib = LoadLibraryA(s.c_str());
      if(hinstLib != NULL){
        fn = (dlfunc) GetProcAddress(hinstLib, (LPCSTR)std::get<std::string>(pop()).c_str());
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
#endif
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
      for(; ps > 0; ps--){
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
    case GE:
      stack.push_back(isGE(pop(), pop()));
      break;
    case LT:
      stack.push_back(isLT(pop(), pop()));
      break;
    case LE:
      stack.push_back(isLE(pop(), pop()));
      break;
    case LAND:
      stack.push_back(LAND2val(pop(), pop()));
      break;
    case LOR:
      stack.push_back(LOR2val(pop(), pop()));
      break;
    case AND:
      stack.push_back(AND2val(pop(), pop()));
      break;
    case OR:
      stack.push_back(OR2val(pop(), pop()));
      break;
    case NOT:
      stack.push_back(NOTval(pop()));
      break;
    case LNOT:
      stack.push_back(LNOTval(pop()));
      break;
    case LSHIFT:
      stack.push_back(LSHIFT2val(pop(), pop()));
      break;
    case RSHIFT:
      stack.push_back(RSHIFT2val(pop(), pop()));
      break;
    case XOR:
      stack.push_back(XOR2val(pop(), pop()));
      break;
    case NEG:
      stack.push_back(NEGval(pop()));
      break;
    case BREAK:
      isBreaked = true;
      break;
    case WTRUN:
      if(stack.size() < 2)break;
      if(!getValType(stack[stack.size() - 1])){
        bool tos = std::get<double>(stack[stack.size() - 1]);
        if(tos){
          stack.pop_back();
          std::vector<value> prog;
          int ps = std::get<double>(pop());
          for(; ps > 0; ps--){
            prog.insert(prog.begin(), pop());
          }
          while(tos){
            run(prog);
            tos = std::get<double>(stack[stack.size() - 1]);
            if(tos)stack.pop_back();
          }
        }
      }
      break;
    case WFRUN:
      if(stack.size() < 2)break;
      if(!getValType(stack[stack.size() - 1])){
        bool tos = std::get<double>(stack[stack.size() - 1]);
        if(!tos){
          stack.pop_back();
          std::vector<value> prog;
          int ps = std::get<double>(pop());
          for(; ps > 0; ps--){
            prog.insert(prog.begin(), pop());
          }
          while(!tos){
            run(prog);
            tos = std::get<double>(stack[stack.size() - 1]);
            if(tos)stack.pop_back();
          }
        }
      }
      break;
    case IFTRUN:
      if(stack.size() < 2)break;
      if(!getValType(stack[stack.size() - 1])){
        bool tos = std::get<double>(stack[stack.size() - 1]);
        if(tos){
          stack.pop_back();
          std::vector<value> prog;
          int ps = std::get<double>(pop());
          for(; ps > 0; ps--){
            prog.insert(prog.begin(), pop());
          }
          run(prog);
        }
      }
      break;
    case IFFRUN:
      if(stack.size() < 2)break;
      if(!getValType(stack[stack.size() - 1])){
        bool tos = std::get<double>(stack[stack.size() - 1]);
        if(!tos){
          stack.pop_back();
          std::vector<value> prog;
          int ps = std::get<double>(pop());
          for(; ps > 0; ps--){
            prog.insert(prog.begin(), pop());
          }
          run(prog);
        }
      }
      break;
    case THREAD: {
      std::vector<value> prog;
      int ps = std::get<double>(pop());
      for(; ps > 0; ps--){
        prog.insert(prog.begin(), pop());
      }
      std::thread t([=]{
        VM *vm = new VM();
        vm->attachMem(mempointer);
        vm->run(prog);
      });
      t.detach();
      break;
    }
    case MEMSET: {
      int n = std::get<double>(pop());
      if(mempointer->size() < (n + 1))mempointer->resize(n + 1);
      mempointer->at(n) = pop();
      break;
    }
    case MEMGET:
      stack.push_back(mempointer->at(std::get<double>(pop())));
      break;
    case MEMSIZE:
      stack.push_back(mempointer->size());
      break;
    case MEMPUT:
      mempointer->push_back(pop());
      break;
    case MEMINS:
      mempointer->insert(mempointer->begin() + std::get<double>(pop()), pop());
      break;
    case MEMDEL:
      mempointer->erase(mempointer->begin() + std::get<double>(pop()));
      break;
  }
  return res;
}
