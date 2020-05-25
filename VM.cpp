#include "VM.h"

VM::VM(){
#ifdef DUSE_GMP_LIB
  mpf_set_default_prec(1024);
#endif
  running = true;
  recsize = 0;
}

VM::~VM() {
#if THREADING == PROTOTHREADING
  while (threads.size() != 0) {
    for (int i = 0; i < threads.size(); i++) {
      threads[i].runNext();
      if (threads[i].isFinished()) {
        threads.erase(threads.begin() + i);
      }
    }
  }
#endif
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

const char* VM::disassemble(int prog, Value val){
  switch (prog) {
    case EXIT:    return "EXIT";
    case PUT:     return Utils::append((val.getType()? "PUT\tTXT" : "PUT\tNUM"), val.toString());
    case ADD:     return "ADD";
    case SUB:     return "SUB";
    case MUL:     return "MUL";
    case DIV:     return "DIV";
    case MOD:     return "MOD";
    case DLCALL:  return "DLCALL";
    case RUN:     return "RUN";
    case REC:     return "REC";
    case END:     return "END";
    case PRINT:   return "PRINT";
    case POP:     return "POP";
    case LOGSTCK: return "LOGSTCK";
    case PRINTLN: return "PRINTLN";
    case REPEAT:  return "REPEAT";
    case EQ:      return "EQ";
    case FEQ:     return "FEQ";
    case GT:      return "GT";
    case GE:      return "GE";
    case LT:      return "LT";
    case LE:      return "LE";
    case LAND:    return "LAND";
    case LOR:     return "LOR";
    case AND:     return "AND";
    case OR:      return "OR";
    case NOT:     return "NOT";
    case LNOT:    return "LNOT";
    case LSHIFT:  return "LSHIFT";
    case RSHIFT:  return "RSHIFT";
    case XOR:     return "XOR";
    case NEG:     return "NEG";
    case BREAK:   return "BREAK";
    case WTRUN:   return "WTRUN";
    case WFRUN:   return "WFRUN";
    case IFFRUN:  return "IFFRUN";
    case IFTRUN:  return "IFTRUN";
    case THREAD:  return "THREAD";
    case MEMGET:  return "MEMGET";
    case MEMSET:  return "MEMSET";
    case MEMPUT:  return "MEMPUT";
    case MEMSIZE: return "MEMSIZE";
    case MEMDEL:  return "MEMDEL";
    case MEMINS:  return "MEMINS";
    case TOTXT:   return "TOTXT";
    case TONUM:   return "TONUM";
    case ISNUM:   return "ISNUM";
    case CANNUM:  return "CANNUM";
    default:      return "???";
    return 0;
  }
}

void VM::printStack(){
  std::cout << "[";
  for(int i = 0; i < stack.size(); i++){
    std::cout << (stack[i].toString()) << ((i + 1) == stack.size()? "":", ");
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

std::vector<Value> VM::assemble(char* line){
  std::vector<Value> prog;
  char* pline;
  line = Utils::rtrim(line);
  if(Utils::find(line, "PUT") == 0){
    line = Utils::substring(line, Utils::stringLength(line) - 3, 3);
    prog.push_back(PUT);
    line = Utils::rtrim(line);
    if(Utils::find(line, "NUM") == 0){
      line = Utils::rtrim(Utils::substring(line, Utils::stringLength(line) - 3, 3));
      int i = 0;
      while ((isdigit(line[i]) || line[i] == '.') || line[i] == '-')i++;
      prog.push_back(NUMBER(Utils::substring(line, i)));
    } else {
      pline = line;
      line = Utils::substring(line, Utils::stringLength(line) - 3, 3);
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\n", "\n");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\n", "\\n");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\t", "\t");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\t", "\\t");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\r", "\r");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\r", "\\r");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\a", "\a");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\a", "\\a");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\b", "\b");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\b", "\\b");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\f", "\f");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\f", "\\f");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\'", "\'");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\"", "\"");
      free(pline);
      pline = line;
      line = Utils::replace(line, "\\\\", "\\");
      free(pline);
      prog.push_back(line);
    }
  } else if(Utils::find(line, "ADD") == 0){
    prog.push_back(ADD);
  } else if(Utils::find(line, "SUB") == 0){
    prog.push_back(SUB);
  } else if(Utils::find(line, "MUL") == 0){
    prog.push_back(MUL);
  } else if(Utils::find(line, "PRINTLN") == 0){
    prog.push_back(PRINTLN);
  } else if(Utils::find(line, "DIV") == 0){
    prog.push_back(DIV);
  } else if(Utils::find(line, "MOD") == 0){
    prog.push_back(MOD);
  } else if(Utils::find(line, "PRINT") == 0){
    prog.push_back(PRINT);
  } else if(Utils::find(line, "DLCALL") == 0){
    prog.push_back(DLCALL);
  } else if(Utils::find(line, "REC") == 0){
    prog.push_back(REC);
  } else if(Utils::find(line, "END") == 0){
    prog.push_back(END);
  } else if(Utils::find(line, "RUN") == 0){
    prog.push_back(RUN);
  } else if(Utils::find(line, "POP") == 0){
    prog.push_back(POP);
  } else if(Utils::find(line, "LOGSTCK") == 0){
    prog.push_back(LOGSTCK);
  } else if(Utils::find(line, "REPEAT") == 0){
    prog.push_back(REPEAT);
  } else if(Utils::find(line, "EQ") == 0){
    prog.push_back(EQ);
  } else if(Utils::find(line, "FEQ") == 0){
    prog.push_back(FEQ);
  } else if(Utils::find(line, "GT") == 0){
    prog.push_back(GT);
  } else if(Utils::find(line, "GE") == 0){
    prog.push_back(GE);
  } else if(Utils::find(line, "LT") == 0){
    prog.push_back(LT);
  } else if(Utils::find(line, "LE") == 0){
    prog.push_back(LE);
  } else if(Utils::find(line, "LAND") == 0){
    prog.push_back(LAND);
  } else if(Utils::find(line, "AND") == 0){
    prog.push_back(AND);
  } else if(Utils::find(line, "LOR") == 0){
    prog.push_back(LOR);
  } else if(Utils::find(line, "OR") == 0){
    prog.push_back(OR);
  } else if(Utils::find(line, "NOT") == 0){
    prog.push_back(NOT);
  } else if(Utils::find(line, "LNOT") == 0){
    prog.push_back(LNOT);
  } else if(Utils::find(line, "LSHIFT") == 0){
    prog.push_back(LSHIFT);
  } else if(Utils::find(line, "RSHIFT") == 0){
    prog.push_back(RSHIFT);
  } else if(Utils::find(line, "XOR") == 0){
    prog.push_back(XOR);
  } else if(Utils::find(line, "NEG") == 0){
    prog.push_back(NEG);
  } else if(Utils::find(line, "BREAK") == 0){
    prog.push_back(BREAK);
  } else if(Utils::find(line, "WTRUN") == 0){
    prog.push_back(WTRUN);
  } else if(Utils::find(line, "WFRUN") == 0){
    prog.push_back(WFRUN);
  } else if(Utils::find(line, "IFFRUN") == 0){
    prog.push_back(IFFRUN);
  } else if(Utils::find(line, "IFTRUN") == 0){
    prog.push_back(IFTRUN);
  } else if(Utils::find(line, "THREAD") == 0){
    prog.push_back(THREAD);
  } else if(Utils::find(line, "MEMGET") == 0){
    prog.push_back(MEMGET);
  } else if(Utils::find(line, "MEMSET") == 0){
    prog.push_back(MEMSET);
  } else if(Utils::find(line, "MEMSIZE") == 0){
    prog.push_back(MEMSIZE);
  } else if(Utils::find(line, "MEMPUT") == 0){
    prog.push_back(MEMPUT);
  } else if(Utils::find(line, "MEMINS") == 0){
    prog.push_back(MEMINS);
  } else if(Utils::find(line, "MEMDEL") == 0){
    prog.push_back(MEMDEL);
  } else if(Utils::find(line, "TOTXT") == 0){
    prog.push_back(TOTXT);
  } else if(Utils::find(line, "TONUM") == 0){
    prog.push_back(TONUM);
  } else if(Utils::find(line, "CANNUM") == 0){
    prog.push_back(CANNUM);
  } else if(Utils::find(line, "ISNUM") == 0){
    prog.push_back(ISNUM);
  } else if(Utils::find(line, "EXIT") == 0){
    prog.push_back(EXIT);
  }
  return prog;
}

std::vector<VM::Record> VM::mkRec(std::vector<Value> vals){
  std::vector<VM::Record> records;
  for(Value v : vals){
    if(v.getType() == TYPE_NUM){
      Record r;
      r.type = TYPE_NUM;
      r.value = v.getDouble();
      records.push_back(r);
    } else {
      Record r;
      const char* str = v.getString();
      r.type = TYPE_TEXT;
      for (size_t i = 0; i < Utils::stringLength(str); i++) {
        r.value = str[i];
        records.push_back(r);
      }
    }
  }
  return records;
}


bool VM::run1(int prog, Value arg){
#if THREADING == PROTOTHREADING
  for (int i = 0; i < threads.size(); i++) {
    threads[i].runNext();
    if (threads[i].isFinished()) {
      threads.erase(threads.begin() + i);
    }
  }
#endif
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
#ifndef DYNAMIC_LIBS_NOT_AVAILABLE
    case DLCALL: {
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
      HINSTANCE hinstLib;
      dlfunc fn;
      std::string s = pop().getString();
      hinstLib = LoadLibraryA(s.c_str());
      if(hinstLib != NULL){
        fn = (dlfunc) GetProcAddress(hinstLib, (LPCSTR)pop().getString());
        if (NULL != fn){
          fn(&stack);
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
      fn(&stack);
      dlclose(lib);
#endif
      break;
    }
#endif
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
#if THREADING == PROTOTHREADING
      threads.push_back(Thread(prog, mempointer));
#else
      std::thread t([=]{
        VM *vm = new VM();
        vm->attachMem(mempointer);
        vm->run(prog);
      });
      t.detach();
#endif
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
