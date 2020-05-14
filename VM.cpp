#include "VM.h"

VM::VM(){
#ifdef DUSE_GMP_LIB
  mpf_set_default_prec(1024);
#endif
  running = true;
  recsize = 0;
}

void VM::attachMem(std::vector<Value> *mem){
  mempointer = mem;
}

std::vector<Value> VM::getStack(){
  return stack;
}

void VM::setStack(std::vector<Value> v){
  stack = v;
}

bool VM::disassemble(int prog, Value val, std::string end){
  switch (prog) {
    case EXIT:
      std::cout << "EXIT" << end;
      break;
    case PUT:
      std::cout << "PUT" << "\t";
      if(val.getType() == TYPE_NUM){
        std::cout << "NUM" << val.getNumber();
      } else {
        std::cout << "TXT" << val.getString();
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
      break;
    case MEMDEL:
      std::cout << "MEMDEL" << end;
      break;
    case MEMINS:
      std::cout << "MEMINS" << end;
      break;
    case TOTXT:
      std::cout << "TOTXT" << end;
      break;
    case TONUM:
      std::cout << "TONUM" << end;
      break;
    case ISNUM:
      std::cout << "ISNUM" << end;
      break;
    case CANNUM:
      std::cout << "CANNUM" << end;
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
    char* ptr;
    std::cout << (ptr = stack[i].toString()) << ((i + 1) == stack.size()? "":", ");
    free(ptr);
  }
  std::cout << "]";
}

void VM::run(std::vector<Value> prog, bool forceRun, int pc) {
  if(forceRun)running = true;
  for (; pc < prog.size(); pc++) {//fetch
    if(running)pc += run1(prog[pc].getLong(), (prog.size() - 1) == pc? 0:prog[pc + 1]);
    else break;
    if(isBreaked){
      isBreaked = false;
      break;
    }
	}
}

Value VM::pop(){
  Value v = stack[stack.size() - 1];
  stack.pop_back();
  return v;
}

Value VM::add2val(Value v1, Value v2){
  if(v1.getType() == TYPE_TEXT || v2.getType() == TYPE_TEXT){
    const char* str1 = v1.toString();
    const char* str2 = v2.toString();
    Value res = Utils::append(str1, str2);
    free((char*)str1);
    free((char*)str2);
    return res;
  } else {
#ifdef USE_GMP_LIB
    return mpf_class(v1.getNumber() + v2.getNumber());
#else
    return v1.getNumber() + v2.getNumber();
#endif
  }
}

Value VM::sub2val(Value v1, Value v2){
  if(v1.getType() == TYPE_TEXT || v2.getType() == TYPE_TEXT){
    return Utils::replace(v1.toString(), v2.toString(), "");
  } else {
#ifdef USE_GMP_LIB
    return mpf_class(v1.getNumber() - v2.getNumber());
#else
    return v1.getNumber() - v2.getNumber();
#endif
  }
}

Value VM::mul2val(Value v1, Value v2){
  if(v1.getType() + v2.getType() == 1){
    char* s = v1.getType() == TYPE_TEXT? v1.getString() : v2.getString();
    int i = v1.getType() == TYPE_NUM? v1.getLong() : v2.getLong();
    return Utils::repeat(s, i);
  } else if(!(v1.getType() && v2.getType())){
#ifdef USE_GMP_LIB
    return mpf_class(v1.getNumber() * v2.getNumber());
#else
    return v1.getNumber() * v2.getNumber();
#endif
  } else {
    std::cerr << "STR * STR ????\n";
    return 0;
  }
}

Value VM::div2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
#ifdef USE_GMP_LIB
    return mpf_class(v1.getNumber() / v2.getNumber());
#else
    return v1.getNumber() / v2.getNumber();
#endif
  } else {
    std::cerr << "STR in / ????\n";
    return 0;
  }
}

Value VM::mod2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return (v1.getLong()) % (v2.getLong());
  } else {
    std::cerr << "STR in % ????\n";
    return 0;
  }
}

Value VM::isEQ(Value v1, Value v2){
  char* str1 = v1.toString();
  char* str2 = v2.toString();
  bool res = Utils::isEQ(str1, str2);
  free(str1);
  free(str2);
  return res;
}

Value VM::isFEQ(Value v1, Value v2){
  char* str1 = v1.toString();
  char* str2 = v2.toString();
  bool res = v1.getType() == v2.getType() && Utils::isEQ(str1, str2);
  free(str1);
  free(str2);
  return res;
}

Value VM::isGT(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getNumber() > v2.getNumber();
  }
  std::cerr << "STR in > ????";
  return 0;
}

Value VM::isGE(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getNumber() >= v2.getNumber();
  }
  std::cerr << "STR in >= ????";
  return 0;
}

Value VM::isLT(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getNumber() < v2.getNumber();
  }
  std::cerr << "STR in < ????";
  return 0;
}

Value VM::isLE(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getNumber() <= v2.getNumber();
  }
  std::cerr << "STR in <= ????";
  return 0;
}

Value VM::LAND2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getLong() && v2.getLong();
  } else {
    std::cerr << "STR in LOGICAL AND????" << std::endl;
  }
}

Value VM::LOR2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getLong() || v2.getLong();
  } else {
    std::cerr << "STR in LOGICAL OR????" << std::endl;
  }
}

Value VM::AND2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return (v1.getLong()) & (v2.getLong());
  } else {
    std::cerr << "STR in BITWISE AND????" << std::endl;
  }
}

Value VM::OR2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return (v1.getLong()) | (v2.getLong());
  } else {
    std::cerr << "STR in BITWISE OR????" << std::endl;
  }
}

Value VM::NOTval(Value v){
  if(!v.getType()){
    return ~(v.getLong());
  } else {
    std::cerr << "STR in BITWISE NOT????" << std::endl;
  }
}

Value VM::LNOTval(Value v){
  if(!v.getType()){
    return !v.getLong();
  } else {
    std::cerr << "STR in LOGICAL NOT????" << std::endl;
  }
}

Value VM::LSHIFT2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getLong() << v2.getLong();
  } else {
    std::cerr << "STR in LEFT SHIFT????" << std::endl;
  }
}

Value VM::RSHIFT2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return (v1.getLong()) >> (v2.getLong());
  } else {
    std::cerr << "STR in RIGHT SHIFT????" << std::endl;
  }
}

Value VM::XOR2val(Value v1, Value v2){
  if(!(v1.getType() && v2.getType())){
    return v1.getLong() ^ v2.getLong();
  } else {
    std::cerr << "STR in XOR????" << std::endl;
  }
}

Value VM::NEGval(Value v){
  if(!v.getType()){
#ifdef USE_GMP_LIB
    return mpf_class(NUMBER(-1) * v.getNumber());
#else
    return NUMBER(-1) * v.getNumber();
#endif
  } else {
    return Utils::reverse(v.getString());
  }
}

bool VM::run1(int prog, Value arg){
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
      exit_code = pop().getLong();
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
      std::cout << pop().toString();
      break;
    case REC:
      rec++;
      break;
    case RUN: {
      std::vector<Value> prog;
      int ps = pop().getLong();
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
      std::string s = pop().getString();
      hinstLib = LoadLibraryA(s.c_str());
      if(hinstLib != NULL){
        fn = (dlfunc) GetProcAddress(hinstLib, (LPCSTR)pop().getString());
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
        break;
      }
      dlerror();//clear errors
      fn = ((dlfunc)dlsym(lib, pop().toString()));
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
      int count = pop().getLong();
      std::vector<Value> prog;
      int ps = pop().getLong();
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
            run(prog);
            tos = stack[stack.size() - 1].getLong();
            if(tos)stack.pop_back();
          }
        }
      }
      break;
    case WFRUN:
      if(stack.size() < 2)break;
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
            run(prog);
            tos = stack[stack.size() - 1].getLong();
            if(!tos)stack.pop_back();
          }
        }
      }
      break;
    case IFTRUN:
      if(stack.size() < 2)break;
      if(!stack[stack.size() - 1].getType()){
        bool tos = stack[stack.size() - 1].getLong();
        if(tos){
          stack.pop_back();
          std::vector<Value> prog;
          int ps = pop().getLong();
          for(; ps > 0; ps--){
            prog.insert(prog.begin(), pop());
          }
          run(prog);
        }
      }
      break;
    case IFFRUN:
      if(stack.size() < 2)break;
      if(!stack[stack.size() - 1].getType()){
        bool tos = stack[stack.size() - 1].getLong();
        if(!tos){
          stack.pop_back();
          std::vector<Value> prog;
          int ps = pop().getLong();
          for(; ps > 0; ps--){
            prog.insert(prog.begin(), pop());
          }
          run(prog);
        }
      }
      break;
    case THREAD: {
      std::vector<Value> prog;
      int ps = pop().getLong();
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
      int n = pop().getLong();
      if(mempointer->size() < (n + 1))mempointer->resize(n + 1);
      mempointer->at(n) = pop();
      break;
    }
    case MEMGET:
      stack.push_back(mempointer->at(pop().getLong()));
      break;
    case MEMSIZE:
      stack.push_back((int)mempointer->size());
      break;
    case MEMPUT:
      mempointer->push_back(pop());
      break;
    case MEMINS:
      mempointer->insert(mempointer->begin() + pop().getLong(), pop());
      break;
    case MEMDEL:
      mempointer->erase(mempointer->begin() + pop().getLong());
      break;
    case TOTXT:
      stack.push_back(pop().toString());
      break;
    case TONUM:
#ifdef USE_GMP_LIB
      stack.push_back(NUMBER(pop().toString()));
#else
      stack.push_back(pop().getNumber());
#endif
      break;
    case ISNUM:
      if(stack.size() == 0) break;
      stack.push_back(!stack[stack.size() - 1].getType());
      break;
    case CANNUM:
      if(stack.size() == 0) break;
      Value v = stack[stack.size() - 1];
      if(!v.getType()){
        stack.push_back(1);
      } else {
        int64_t i = 0;
        for(; v.getString()[i] != 0; i++){
          if(!isdigit(v.getString()[i])){
            stack.push_back(0);
            return res;
          }
        }
        stack.push_back(1);
      }
      break;
  }
  return res;
}
