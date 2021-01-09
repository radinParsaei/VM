#include "VM.h"
#include "static_libcall.h"

void freeUnusedPointer(long) {}

VM::VM() {
#ifdef USE_GMP_LIB
  mpf_set_default_prec(1024);
#else
  BigNumber::begin(10);
#endif
  running = true;
  recsize = 0;
}

#if THREADING == PROTOTHREADING
void VM::runAllProtoThreads() {
  while (threads.size() != 0) {
    for (int i = 0; i < threads.size(); i++) {
      threads[i].runNext();
      if (threads[i].isFinished()) {
        threads.erase(threads.begin() + i);
      }
    }
  }
}

VM::~VM() {
#ifdef RUN_PROTO_THREADS_AT_EXIT
  runAllProtoThreads();
#endif
}
#endif

void VM::attachMem(std::vector<Value> *mem) {
  mempointer = mem;
}

void VM::setInternalLibraryFunction(void (*internalLibraryFunction) (Value, VM*)) {
  this->internalLibraryFunction = internalLibraryFunction;
}

std::vector<Value> VM::getStack() {
  return stack;
}

std::vector<Value>* VM::getMemoryPointer() {
  return mempointer;
}

void VM::setStack(std::vector<Value> v) {
  stack = v;
}

Value VM::disassemble(int prog, Value val) {
  switch (prog) {
    case EXIT:    return "EXIT";
    case PUT:     return std::string(val.getType() == VALUE_TYPE_NUMBER? "PUT\tNUM" : (val.getType() == True || val.getType() == False)? "PUT\tBOOL" : (val.getType() == VALUE_TYPE_TEXT? "PUT\tTXT" : "PUT\t")) + val.toString();
    case ADD:     return "ADD";
    case SUB:     return "SUB";
    case MUL:     return "MUL";
    case DIV:     return "DIV";
    case MOD:     return "MOD";
    case POW:     return "POW";
    case DLCALL:  return "DLCALL";
    case RUN:     return "RUN";
    case REC:     return "REC";
    case END:     return "END";
    case PRINT:   return "PRINT";
    case POP:     return "POP";
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
    case GET:     return "GET";
    case SET:     return "SET";
    case APPEND:  return "APPEND";
    case INSERT:  return "INSERT";
    case PUTARR:  return "PUTARR";
    case NEG:     return "NEG";
    case BREAK:   return "BREAK";
    case CONTINU: return "CONTINUE";
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
    case TOPTR:   return "TOPTR";
    case ISNUM:   return "ISNUM";
    case CANNUM:  return "CANNUM";
    case TOBOOL:  return "TOBOOL";
    case SKIP:    return "SKIP";
    case IFSKIP:  return "IFSKIP";
    case MKFN:    return "MKFN";
    case CALLFN:  return "CALLFN";
    case EXITFN:  return "EXITFN";
    case STCKGET: return "STCKGET";
    case STCKGET2:return "STCKGET2";
    case STCKMOV: return "STCKMOV";
    case STCKDEL: return "STCKDEL";
    default:      return "???";
    return 0;
  }
}

void VM::printStack() {
  std::cout << "[";
  for(int i = 0; i < stack.size(); i++) {
    std::cout << (stack[i].toString()) << ((i + 1) == stack.size()? "":", ");
  }
  std::cout << "]";
}

bool VM::run(std::vector<Value> prog, bool forceRun, int pc) {
  isBreaked = false;
  if(forceRun)running = true;
  for (; pc < prog.size(); pc++) {//fetch
    if(isContinued) {
      break;
    }
    if(running)pc += run1(prog[pc].getLong(), (prog.size() - 1) == pc? 0:prog[pc + 1]);
    else break;
    if(isBreaked) {
      isBreaked = false;
      return true;
    }
  }
  return false;
}

Value VM::pop() {
  Value v = stack[stack.size() - 1];
  stack.pop_back();
  return v;
}

void VM::push(Value data) {
  stack.push_back(data);
}

void VM::set(size_t addr, Value data) {
  stack[addr] = data;
}

std::vector<Value> VM::assemble(Value line) {
  std::vector<Value> prog;
  line.trimLeft();
  if (line.startsWith("PUTARR")) {
    prog.push_back(PUTARR);
  } else if (line.startsWith("PUT")) {
    line.substring(3);
    line.trimLeft();
    prog.push_back(PUT);
    if (line.startsWith("NUM")) {
      line.substring(3);
      int i = 0;
      while (!(isdigit(line[i].toString()[0]) || line[i] == '.' || line[i] == '-')) i++;
      line.substring(i);
      line.trim();
      line.toNum();
      prog.push_back(line);
    } else if (line.startsWith("TXT")) {
      line.substring(3);
      line.replace("\\n", "\n");
      line.replace("\\\n", "\\n");
      line.replace("\\t", "\t");
      line.replace("\\\t", "\\t");
      line.replace("\\r", "\r");
      line.replace("\\\r", "\\r");
      line.replace("\\a", "\a");
      line.replace("\\\a", "\\a");
      line.replace("\\b", "\b");
      line.replace("\\\b", "\\b");
      line.replace("\\f", "\f");
      line.replace("\\\f", "\\f");
      line.replace("\\\'", "\'");
      line.replace("\\\"", "\"");
      line.replace("\\\\", "\\");
      prog.push_back(line);
    } else if (line.startsWith("BOOL")) {
      line.substring(4);
      line.trim();
      line.replace("1", "true");
      line.replace("0", "false");
      prog.push_back(line.toLower() == "true");
    } else {
      prog.push_back(null);
    }
  } else if (line.startsWith("ADD")) {
    prog.push_back(ADD);
  } else if (line.startsWith("SUB")) {
    prog.push_back(SUB);
  } else if (line.startsWith("MUL")) {
    prog.push_back(MUL);
  } else if (line.startsWith("DIV")) {
    prog.push_back(DIV);
  } else if (line.startsWith("MOD")) {
    prog.push_back(MOD);
  } else if (line.startsWith("POW")) {
    prog.push_back(POW);
  } else if (line.startsWith("PRINT")) {
    prog.push_back(PRINT);
  } else if (line.startsWith("DLCALL")) {
    prog.push_back(DLCALL);
  } else if (line.startsWith("REC")) {
    prog.push_back(REC);
  } else if (line.startsWith("END")) {
    prog.push_back(END);
  } else if (line.startsWith("RUN")) {
    prog.push_back(RUN);
  } else if (line.startsWith("POP")) {
    prog.push_back(POP);
  } else if (line.startsWith("REPEAT")) {
    prog.push_back(REPEAT);
  } else if (line.startsWith("EQ")) {
    prog.push_back(EQ);
  } else if (line.startsWith("FEQ")) {
    prog.push_back(FEQ);
  } else if (line.startsWith("GT")) {
    prog.push_back(GT);
  } else if (line.startsWith("GET")) {
    prog.push_back(GET);
  } else if (line.startsWith("GE")) {
    prog.push_back(GE);
  } else if (line.startsWith("LT")) {
    prog.push_back(LT);
  } else if (line.startsWith("LE")) {
    prog.push_back(LE);
  } else if (line.startsWith("LAND")) {
    prog.push_back(LAND);
  } else if (line.startsWith("AND")) {
    prog.push_back(AND);
  } else if (line.startsWith("LOR")) {
    prog.push_back(LOR);
  } else if (line.startsWith("OR")) {
    prog.push_back(OR);
  } else if (line.startsWith("NOT")) {
    prog.push_back(NOT);
  } else if (line.startsWith("LNOT")) {
    prog.push_back(LNOT);
  } else if (line.startsWith("LSHIFT")) {
    prog.push_back(LSHIFT);
  } else if (line.startsWith("RSHIFT")) {
    prog.push_back(RSHIFT);
  } else if (line.startsWith("XOR")) {
    prog.push_back(XOR);
  } else if (line.startsWith("NEG")) {
    prog.push_back(NEG);
  } else if (line.startsWith("BREAK")) {
    prog.push_back(BREAK);
  } else if (line.startsWith("CONTINU")) {
    prog.push_back(CONTINU);
  } else if (line.startsWith("WTRUN")) {
    prog.push_back(WTRUN);
  } else if (line.startsWith("WFRUN")) {
    prog.push_back(WFRUN);
  } else if (line.startsWith("IFFRUN")) {
    prog.push_back(IFFRUN);
  } else if (line.startsWith("IFTRUN")) {
    prog.push_back(IFTRUN);
  } else if (line.startsWith("THREAD")) {
    prog.push_back(THREAD);
  } else if (line.startsWith("MEMGET")) {
    prog.push_back(MEMGET);
  } else if (line.startsWith("MEMSET")) {
    prog.push_back(MEMSET);
  } else if (line.startsWith("MEMSIZE")) {
    prog.push_back(MEMSIZE);
  } else if (line.startsWith("MEMPUT")) {
    prog.push_back(MEMPUT);
  } else if (line.startsWith("MEMINS")) {
    prog.push_back(MEMINS);
  } else if (line.startsWith("MEMDEL")) {
    prog.push_back(MEMDEL);
  } else if (line.startsWith("TOTXT")) {
    prog.push_back(TOTXT);
  } else if (line.startsWith("TONUM")) {
    prog.push_back(TONUM);
  } else if (line.startsWith("TOPTR")) {
    prog.push_back(TOPTR);
  } else if (line.startsWith("CANNUM")) {
    prog.push_back(CANNUM);
  } else if (line.startsWith("ISNUM")) {
    prog.push_back(ISNUM);
  } else if (line.startsWith("EXITFN")) {
    prog.push_back(EXITFN);
  } else if (line.startsWith("TOBOOL")) {
    prog.push_back(TOBOOL);
  } else if (line.startsWith("SKIP")) {
    prog.push_back(SKIP);
  } else if (line.startsWith("IFSKIP")) {
    prog.push_back(IFSKIP);
  } else if (line.startsWith("MKFN")) {
    prog.push_back(MKFN);
  } else if (line.startsWith("CALLFN")) {
    prog.push_back(CALLFN);
  } else if (line.startsWith("STCKGET2")) {
    prog.push_back(STCKGET2);
  } else if (line.startsWith("STCKGET")) {
    prog.push_back(STCKGET);
  } else if (line.startsWith("STCKMOV")) {
    prog.push_back(STCKMOV);
  } else if (line.startsWith("EXIT")) {
    prog.push_back(EXIT);
  } else if (line.startsWith("STCKDEL")) {
    prog.push_back(STCKDEL);
  } else if (line.startsWith("APPEND")) {
    prog.push_back(APPEND);
  } else if (line.startsWith("INSERT")) {
    prog.push_back(INSERT);
  } else if (line.startsWith("SET")) {
    prog.push_back(SET);
  }
  return prog;
}

bool VM::run1(int prog, Value arg) {
  skip && skip--;
  if (skip) return prog == PUT;
#if THREADING == PROTOTHREADING
  for (int i = 0; i < threads.size(); i++) {
    threads[i].runNext();
    if (threads[i].isFinished()) {
      threads.erase(threads.begin() + i);
    }
  }
#endif
  if(rec) {
    if(prog == END) {
      rec--;
      if(rec == 0) {
        stack.push_back(recsize);
        recsize = 0;
      } else {
        stack.push_back(prog);
        recsize++;
      }
    } else {
      stack.push_back(prog);
      recsize++;
      if(prog == PUT) {
        stack.push_back(arg);
        recsize++;
        return 1;
      } else if(prog == REC) {
        rec++;
      }
    }
    return 0;
  }
#ifdef VM_DISASSEMBLE
  std::cout << disassemble(prog, arg);
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
  if (VM_ext_run(prog, arg, this)) {
    return prog == PUT;
  }
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
    case ADD: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a + b);
      break;
    }
    case SUB: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a - b);
      break;
    }
    case MUL:
      stack.push_back(pop() * pop());
      break;
    case DIV: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a / b);
      break;
    }
    case MOD: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a % b);
      break;
    }
    case POW: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a.pow(b));
      break;
    }
    case PRINT:
      std::cout << pop();
      break;
    case REC:
      rec++;
      break;
    case RUN: {
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--) {
        prog.insert(prog.begin(), pop());
      }
      run(prog);
      break;
    }
#ifndef DYNAMIC_LIBS_NOT_AVAILABLE
    case DLCALL: {
      if (_dlcall(this)) break;
      Value v1 = pop();
      if (v1 == "." && internalLibraryFunction) internalLibraryFunction(pop(), this);
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
      HINSTANCE hinstLib;
      dlfunc fn;
      std::string s = v1.getString();
      hinstLib = LoadLibraryA(s.c_str());
      if(hinstLib != NULL) {
        fn = (dlfunc) GetProcAddress(hinstLib, (LPCSTR)pop().getString().c_str());
        if (NULL != fn) {
          fn(this);
        } else {
          std::cerr << "SYMBOL NOT FOUND" << GetLastError();
        }
      } else {
        std::cerr << "CANNOT OPEN LIBRARY\n";
        std::cerr << GetLastError();
      }
#else
      void* lib;
      dlfunc fn;
      lib = dlopen(v1.toString().c_str(), RTLD_LAZY);
      if(!lib) {
        std::cerr << "CANNOT OPEN LIBRARY\n";
        std::cerr << dlerror();
        break;
      }
      dlerror();//clear errors
      fn = ((dlfunc)dlsym(lib, pop().toString().c_str()));
      fn(this);
#endif
      break;
    }
#else
    case DLCALL:
      if (_dlcall(this)) break;
      if (pop() == "." && internalLibraryFunction) internalLibraryFunction(pop(), this);
      break;
#endif
    case POP:
      stack.pop_back();
      break;
    case REPEAT: {
      int count = pop().getLong();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--) {
        prog.insert(prog.begin(), pop());
      }
      for(; count > 0; count--) {
        if (run(prog)) {
          isContinued = false;
          isBreaked = fnExited;
          break;
        }
        isContinued = false;
      }
      break;
    }
    case EQ:
      stack.push_back(pop().equals(pop()));
      break;
    case FEQ:
      stack.push_back(pop().strictEquals(pop()));
      break;
    case GT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a > b);
      break;
    }
    case GE: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a >= b);
      break;
    }
    case LT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a < b);
      break;
    }
    case LE: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a <= b);
      break;
    }
    case LAND:
      stack.push_back(pop() && pop());
      break;
    case LOR:
      stack.push_back(pop() || pop());
      break;
    case AND:
      stack.push_back(pop() & pop());
      break;
    case OR:
      stack.push_back(pop() | pop());
      break;
    case NOT:
      stack.push_back(~pop());
      break;
    case LNOT:
      stack.push_back(!pop());
      break;
    case LSHIFT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a << b);
      break;
    }
    case RSHIFT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a >> b);
      break;
    }
    case XOR:
      stack.push_back(pop() ^ pop());
      break;
    case NEG: {
      Value a = pop();
      if (a.getType() == VALUE_TYPE_NUMBER) {
        stack.push_back(-a);
      } else {
        stack.push_back(a.reverse());
      }
      break;
    }
    case BREAK:
      isBreaked = true;
      break;
    case CONTINU:
      isContinued = true;
      break;
    case WTRUN: {
        if(stack.size() < 2)break;
        bool tos = stack[stack.size() - 1].getBool();
        if(tos) {
          stack.pop_back();
          std::vector<Value> prog;
          int ps = pop().getLong();
          for(; ps > 0; ps--) {
            prog.insert(prog.begin(), pop());
          }
          while(tos) {
            if (run(prog)) {
              isContinued = false;
              isBreaked = fnExited;
              break;
            }
            isContinued = false;
            tos = stack[stack.size() - 1].getBool();
            if(tos)stack.pop_back();
          }
        }
        break;
      }
    case WFRUN: {
        if(stack.size() < 2)break;
        bool tos = stack[stack.size() - 1].getBool();
        if(!tos) {
          stack.pop_back();
          std::vector<Value> prog;
          int ps = pop().getLong();
          for(; ps > 0; ps--) {
            prog.insert(prog.begin(), pop());
          }
          while(!tos) {
            if (run(prog)) {
              isContinued = false;
              isBreaked = fnExited;
              break;
            }
            isContinued = false;
            tos = stack[stack.size() - 1].getBool();
            if(!tos)stack.pop_back();
          }
        }
        break;
      }
    case IFTRUN: {
        if(stack.size() < 2)break;
        bool tos = stack[stack.size() - 1].getBool();
        if(tos) {
          stack.pop_back();
          std::vector<Value> prog;
          int ps = pop().getLong();
          for(; ps > 0; ps--) {
            prog.insert(prog.begin(), pop());
          }
          isBreaked = run(prog);
        }
        break;
      }
    case IFFRUN: {
        if(stack.size() < 2)break;
        bool tos = stack[stack.size() - 1].getBool();
        if(!tos) {
          stack.pop_back();
          std::vector<Value> prog;
          int ps = pop().getLong();
          for(; ps > 0; ps--) {
            prog.insert(prog.begin(), pop());
          }
          isBreaked = run(prog);
        }
        break;
      }
    case THREAD: {
      std::vector<Value> prog;
      int ps = pop().getLong();
#if THREADING == PROTOTHREADING
      int psTmp = ps;
#endif
      for(; ps > 0; ps--) {
        prog.insert(prog.begin(), pop());
      }
#if THREADING == PROTOTHREADING
      Value* p = new Value[psTmp];
      std::copy(prog.begin(), prog.end(), p);
      threads.push_back(Thread(p, psTmp, mempointer));
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
    case MEMINS: {
      Value a = pop();
      Value b = pop();
      mempointer->insert(mempointer->begin() + a.getLong(), b);
      break;
    }
    case MEMDEL:
      mempointer->erase(mempointer->begin() + pop().getLong());
      break;
    case TOTXT:
      stack.push_back(pop().toString());
      break;
    case TONUM:
      stack[stack.size() - 1].toNum();
      break;
    case TOPTR:
      stack[stack.size() - 1].toPtr();
      break;
    case ISNUM:
      if(stack.size() == 0) break;
      stack.push_back(!stack[stack.size() - 1].getType());
      break;
    case CANNUM: {
      if(stack.size() == 0) break;
      Value v = stack[stack.size() - 1];
      if(!v.getType()) {
        stack.push_back(1);
      } else {
        int64_t i = 0;
        for(; v.getString()[i] != 0; i++) {
          if(!isdigit(v.getString()[i])) {
            stack.push_back(0);
            return res;
          }
        }
        stack.push_back(1);
      }
      break;
    }
    case TOBOOL: {
      Value a = stack[stack.size() - 1];
      if (a == True || a == False) break;
      stack.pop_back();
      if (a == 0 || a == null) {
        stack.push_back(False);
      } else {
        stack.push_back(True);
      }
      break;
    }
    case SKIP:
      skip = pop().getLong() + 1;
      break;
    case IFSKIP:
      if (pop().getBool()) {
        skip = pop().getLong() + 1;
      } else {
        pop();
      }
      break;
    case MKFN: {
      int fnCode = pop().getLong();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--) {
        prog.insert(prog.begin(), pop());
      }
      functions.insert(std::make_pair(fnCode, prog));
      break;
    }
    case CALLFN:
      run(functions[pop().getLong()]);
      fnExited = false;
      break;
    case EXITFN:
      fnExited = true;
      isBreaked = true;
      break;
    case STCKGET: {
      int point = pop().getLong();
      Value data = stack[point];
      stack.erase(stack.begin() + point);
      stack.push_back(data);
      break;
    }
    case STCKGET2: {
      int point = pop().getLong();
      stack.push_back(stack[point]);
      break;
    }
    case STCKMOV: {
      int point = pop().getLong();
      Value data = pop();
      stack.insert(stack.begin() + point, data);
      break;
    }
    case STCKDEL: {
      stack.erase(stack.begin() + pop().getLong());
      break;
    }
    case PUTARR:
      stack.push_back(Array);
      break;
    case SET: {
      long point = pop().getLong();
      Value data = pop();
      stack[stack.size() - 1].set(point, data);
      break;
    }
    case GET: {
      long point = pop().getLong();
      Value array = pop();
      stack.push_back(array.get(point));
      break;
    }
    case APPEND: {
      Value data = pop();
      stack[stack.size() - 1].append(data);
      break;
    }
    case INSERT: {
      long point = pop().getLong();
      Value data = pop();
      stack[stack.size() - 1].insert(point, data);
      break;
    }
  }
  return res;
}
