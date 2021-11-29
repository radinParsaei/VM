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
    case opcode_EXIT:    return "EXIT";
    case opcode_PUT:     return TEXT(val.getType() == VALUE_TYPE_NUMBER? "PUT\tNUM" : (val.getType() == True || val.getType() == False)? "PUT\tBOOL" : (val.getType() == VALUE_TYPE_TEXT? "PUT\tTXT" : "PUT\t")) + val.toString();
    case opcode_ADD:     return "ADD";
    case opcode_SUB:     return "SUB";
    case opcode_MUL:     return "MUL";
    case opcode_DIV:     return "DIV";
    case opcode_MOD:     return "MOD";
    case opcode_POW:     return "POW";
    case opcode_DLCALL:  return "DLCALL";
    case opcode_RUN:     return "RUN";
    case opcode_REC:     return "REC";
    case opcode_END:     return "END";
    case opcode_PRINT:   return "PRINT";
    case opcode_POP:     return "POP";
    case opcode_REPEAT:  return "REPEAT";
    case opcode_EQ:      return "EQ";
    case opcode_FEQ:     return "FEQ";
    case opcode_GT:      return "GT";
    case opcode_GE:      return "GE";
    case opcode_LT:      return "LT";
    case opcode_LE:      return "LE";
    case opcode_LAND:    return "LAND";
    case opcode_LOR:     return "LOR";
    case opcode_AND:     return "AND";
    case opcode_OR:      return "OR";
    case opcode_NOT:     return "NOT";
    case opcode_LNOT:    return "LNOT";
    case opcode_LSHIFT:  return "LSHIFT";
    case opcode_RSHIFT:  return "RSHIFT";
    case opcode_XOR:     return "XOR";
    case opcode_GET:     return "GET";
    case opcode_SET:     return "SET";
    case opcode_APPEND:  return "APPEND";
    case opcode_INSERT:  return "INSERT";
    case opcode_PUTARR:  return "PUTARR";
    case opcode_MKARR:   return "MKARR";
    case opcode_NEG:     return "NEG";
    case opcode_BREAK:   return "BREAK";
    case opcode_CONTINU: return "CONTINUE";
    case opcode_WTRUN:   return "WTRUN";
    case opcode_WFRUN:   return "WFRUN";
    case opcode_IFFRUN:  return "IFFRUN";
    case opcode_IFTRUN:  return "IFTRUN";
    case opcode_THREAD:  return "THREAD";
    case opcode_MEMGET:  return "MEMGET";
    case opcode_MEMSET:  return "MEMSET";
    case opcode_MEMPUT:  return "MEMPUT";
    case opcode_MEMSIZE: return "MEMSIZE";
    case opcode_MEMDEL:  return "MEMDEL";
    case opcode_MEMINS:  return "MEMINS";
    case opcode_TOTXT:   return "TOTXT";
    case opcode_TONUM:   return "TONUM";
    case opcode_TOPTR:   return "TOPTR";
    case opcode_ISNUM:   return "ISNUM";
    case opcode_CANNUM:  return "CANNUM";
    case opcode_TOBOOL:  return "TOBOOL";
    case opcode_SKIP:    return "SKIP";
    case opcode_IFSKIP:  return "IFSKIP";
    case opcode_MKFN:    return "MKFN";
    case opcode_CALLFN:  return "CALLFN";
    case opcode_EXITFN:  return "EXITFN";
    case opcode_STCKGET: return "STCKGET";
    case opcode_STCKGET2:return "STCKGET2";
    case opcode_STCKMOV: return "STCKMOV";
    case opcode_STCKDEL: return "STCKDEL";
    default:      return "???";
    return 0;
  }
}

void VM::printStack() {
  Value::print("[");
  for(int i = 0; i < stack.size(); i++) {
    Value::print(stack[i].toString());
    Value::print(((i + 1) == stack.size()? "":", "));
  }
  Value::print("]");
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
    prog.push_back(opcode_PUTARR);
  } else if (line.startsWith("PUT")) {
    line.substring(3);
    line.trimLeft();
    prog.push_back(opcode_PUT);
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
    prog.push_back(opcode_ADD);
  } else if (line.startsWith("SUB")) {
    prog.push_back(opcode_SUB);
  } else if (line.startsWith("MUL")) {
    prog.push_back(opcode_MUL);
  } else if (line.startsWith("DIV")) {
    prog.push_back(opcode_DIV);
  } else if (line.startsWith("MOD")) {
    prog.push_back(opcode_MOD);
  } else if (line.startsWith("POW")) {
    prog.push_back(opcode_POW);
  } else if (line.startsWith("PRINT")) {
    prog.push_back(opcode_PRINT);
  } else if (line.startsWith("DLCALL")) {
    prog.push_back(opcode_DLCALL);
  } else if (line.startsWith("REC")) {
    prog.push_back(opcode_REC);
  } else if (line.startsWith("END")) {
    prog.push_back(opcode_END);
  } else if (line.startsWith("RUN")) {
    prog.push_back(opcode_RUN);
  } else if (line.startsWith("POP")) {
    prog.push_back(opcode_POP);
  } else if (line.startsWith("REPEAT")) {
    prog.push_back(opcode_REPEAT);
  } else if (line.startsWith("EQ")) {
    prog.push_back(opcode_EQ);
  } else if (line.startsWith("FEQ")) {
    prog.push_back(opcode_FEQ);
  } else if (line.startsWith("GT")) {
    prog.push_back(opcode_GT);
  } else if (line.startsWith("GET")) {
    prog.push_back(opcode_GET);
  } else if (line.startsWith("GE")) {
    prog.push_back(opcode_GE);
  } else if (line.startsWith("LT")) {
    prog.push_back(opcode_LT);
  } else if (line.startsWith("LE")) {
    prog.push_back(opcode_LE);
  } else if (line.startsWith("LAND")) {
    prog.push_back(opcode_LAND);
  } else if (line.startsWith("AND")) {
    prog.push_back(opcode_AND);
  } else if (line.startsWith("LOR")) {
    prog.push_back(opcode_LOR);
  } else if (line.startsWith("OR")) {
    prog.push_back(opcode_OR);
  } else if (line.startsWith("NOT")) {
    prog.push_back(opcode_NOT);
  } else if (line.startsWith("LNOT")) {
    prog.push_back(opcode_LNOT);
  } else if (line.startsWith("LSHIFT")) {
    prog.push_back(opcode_LSHIFT);
  } else if (line.startsWith("RSHIFT")) {
    prog.push_back(opcode_RSHIFT);
  } else if (line.startsWith("XOR")) {
    prog.push_back(opcode_XOR);
  } else if (line.startsWith("NEG")) {
    prog.push_back(opcode_NEG);
  } else if (line.startsWith("BREAK")) {
    prog.push_back(opcode_BREAK);
  } else if (line.startsWith("CONTINU")) {
    prog.push_back(opcode_CONTINU);
  } else if (line.startsWith("WTRUN")) {
    prog.push_back(opcode_WTRUN);
  } else if (line.startsWith("WFRUN")) {
    prog.push_back(opcode_WFRUN);
  } else if (line.startsWith("IFFRUN")) {
    prog.push_back(opcode_IFFRUN);
  } else if (line.startsWith("IFTRUN")) {
    prog.push_back(opcode_IFTRUN);
  } else if (line.startsWith("THREAD")) {
    prog.push_back(opcode_THREAD);
  } else if (line.startsWith("MEMGET")) {
    prog.push_back(opcode_MEMGET);
  } else if (line.startsWith("MEMSET")) {
    prog.push_back(opcode_MEMSET);
  } else if (line.startsWith("MEMSIZE")) {
    prog.push_back(opcode_MEMSIZE);
  } else if (line.startsWith("MEMPUT")) {
    prog.push_back(opcode_MEMPUT);
  } else if (line.startsWith("MEMINS")) {
    prog.push_back(opcode_MEMINS);
  } else if (line.startsWith("MEMDEL")) {
    prog.push_back(opcode_MEMDEL);
  } else if (line.startsWith("TOTXT")) {
    prog.push_back(opcode_TOTXT);
  } else if (line.startsWith("TONUM")) {
    prog.push_back(opcode_TONUM);
  } else if (line.startsWith("TOPTR")) {
    prog.push_back(opcode_TOPTR);
  } else if (line.startsWith("CANNUM")) {
    prog.push_back(opcode_CANNUM);
  } else if (line.startsWith("ISNUM")) {
    prog.push_back(opcode_ISNUM);
  } else if (line.startsWith("EXITFN")) {
    prog.push_back(opcode_EXITFN);
  } else if (line.startsWith("TOBOOL")) {
    prog.push_back(opcode_TOBOOL);
  } else if (line.startsWith("SKIP")) {
    prog.push_back(opcode_SKIP);
  } else if (line.startsWith("IFSKIP")) {
    prog.push_back(opcode_IFSKIP);
  } else if (line.startsWith("MKFN")) {
    prog.push_back(opcode_MKFN);
  } else if (line.startsWith("CALLFN")) {
    prog.push_back(opcode_CALLFN);
  } else if (line.startsWith("STCKGET2")) {
    prog.push_back(opcode_STCKGET2);
  } else if (line.startsWith("STCKGET")) {
    prog.push_back(opcode_STCKGET);
  } else if (line.startsWith("STCKMOV")) {
    prog.push_back(opcode_STCKMOV);
  } else if (line.startsWith("EXIT")) {
    prog.push_back(opcode_EXIT);
  } else if (line.startsWith("STCKDEL")) {
    prog.push_back(opcode_STCKDEL);
  } else if (line.startsWith("APPEND")) {
    prog.push_back(opcode_APPEND);
  } else if (line.startsWith("INSERT")) {
    prog.push_back(opcode_INSERT);
  } else if (line.startsWith("SET")) {
    prog.push_back(opcode_SET);
  } else if (line.startsWith("MKARR")) {
    prog.push_back(opcode_MKARR);
  }
  return prog;
}

bool VM::run1(int prog, Value arg) {
  skip && skip--;
  if (skip) return prog == opcode_PUT;
#if THREADING == PROTOTHREADING
  for (int i = 0; i < threads.size(); i++) {
    threads[i].runNext();
    if (threads[i].isFinished()) {
      threads.erase(threads.begin() + i);
    }
  }
#endif
  if(rec) {
    if(prog == opcode_END) {
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
      if(prog == opcode_PUT) {
        stack.push_back(arg);
        recsize++;
        return 1;
      } else if(prog == opcode_REC) {
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
    return prog == opcode_PUT;
  }
  bool res = 0;
  switch (prog) {
    case opcode_EXIT:
      running = false;
      exit_code = pop().getLong();
      if(autoKill)exit(exit_code);
      break;
    case opcode_PUT:
      stack.push_back(arg);
      res = 1;
      break;
    case opcode_ADD: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a + b);
      break;
    }
    case opcode_SUB: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a - b);
      break;
    }
    case opcode_MUL:
      stack.push_back(pop() * pop());
      break;
    case opcode_DIV: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a / b);
      break;
    }
    case opcode_MOD: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a % b);
      break;
    }
    case opcode_POW: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a.pow(b));
      break;
    }
    case opcode_PRINT:
      Value::print(pop());
      break;
    case opcode_REC:
      rec++;
      break;
    case opcode_RUN: {
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--) {
        prog.insert(prog.begin(), pop());
      }
      run(prog);
      break;
    }
#ifndef DYNAMIC_LIBS_NOT_AVAILABLE
    case opcode_DLCALL: {
      if (_dlcall(this)) break;
      Value v1 = pop();
      if (v1 == "." && internalLibraryFunction) internalLibraryFunction(pop(), this);
      break;
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
    case opcode_DLCALL:
      if (_dlcall(this)) break;
      if (pop() == "." && internalLibraryFunction) internalLibraryFunction(pop(), this);
      break;
#endif
    case opcode_POP:
      stack.pop_back();
      break;
    case opcode_REPEAT: {
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
    case opcode_EQ:
      stack.push_back(pop().equals(pop()));
      break;
    case opcode_FEQ:
      stack.push_back(pop().strictEquals(pop()));
      break;
    case opcode_GT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a > b);
      break;
    }
    case opcode_GE: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a >= b);
      break;
    }
    case opcode_LT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a < b);
      break;
    }
    case opcode_LE: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a <= b);
      break;
    }
    case opcode_LAND:
      stack.push_back(pop() && pop());
      break;
    case opcode_LOR:
      stack.push_back(pop() || pop());
      break;
    case opcode_AND:
      stack.push_back(pop() & pop());
      break;
    case opcode_OR:
      stack.push_back(pop() | pop());
      break;
    case opcode_NOT:
      stack.push_back(~pop());
      break;
    case opcode_LNOT:
      stack.push_back(!pop());
      break;
    case opcode_LSHIFT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a << b);
      break;
    }
    case opcode_RSHIFT: {
      Value a = pop();
      Value b = pop();
      stack.push_back(a >> b);
      break;
    }
    case opcode_XOR:
      stack.push_back(pop() ^ pop());
      break;
    case opcode_NEG: {
      Value a = pop();
      if (a.getType() == VALUE_TYPE_NUMBER) {
        stack.push_back(-a);
      } else {
        stack.push_back(a.reverse());
      }
      break;
    }
    case opcode_BREAK:
      isBreaked = true;
      break;
    case opcode_CONTINU:
      isContinued = true;
      break;
    case opcode_WTRUN: {
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
    case opcode_WFRUN: {
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
    case opcode_IFTRUN: {
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
    case opcode_IFFRUN: {
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
    case opcode_THREAD: {
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
    case opcode_MEMSET: {
      int n = pop().getLong();
      if(mempointer->size() < (n + 1))mempointer->resize(n + 1);
      mempointer->at(n) = pop();
      break;
    }
    case opcode_MEMGET:
      stack.push_back(mempointer->at(pop().getLong()));
      break;
    case opcode_MEMSIZE:
      stack.push_back((int)mempointer->size());
      break;
    case opcode_MEMPUT:
      mempointer->push_back(pop());
      break;
    case opcode_MEMINS: {
      Value a = pop();
      Value b = pop();
      mempointer->insert(mempointer->begin() + a.getLong(), b);
      break;
    }
    case opcode_MEMDEL:
      mempointer->erase(mempointer->begin() + pop().getLong());
      break;
    case opcode_TOTXT:
      stack.push_back(pop().toString());
      break;
    case opcode_TONUM:
      stack[stack.size() - 1].toNum();
      break;
    case opcode_TOPTR:
      stack[stack.size() - 1].toPtr();
      break;
    case opcode_ISNUM:
      if(stack.size() == 0) break;
      stack.push_back(!stack[stack.size() - 1].getType());
      break;
    case opcode_CANNUM: {
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
    case opcode_TOBOOL: {
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
    case opcode_SKIP:
      skip = pop().getLong() + 1;
      break;
    case opcode_IFSKIP:
      if (pop().getBool()) {
        skip = pop().getLong() + 1;
      } else {
        pop();
      }
      break;
    case opcode_MKFN: {
      int fnCode = pop().getLong();
      std::vector<Value> prog;
      int ps = pop().getLong();
      for(; ps > 0; ps--) {
        prog.insert(prog.begin(), pop());
      }
      functions.insert(std::make_pair(fnCode, prog));
      break;
    }
    case opcode_CALLFN:
      run(functions[pop().getLong()]);
      fnExited = false;
      break;
    case opcode_EXITFN:
      fnExited = true;
      isBreaked = true;
      break;
    case opcode_STCKGET: {
      int point = pop().getLong();
      Value data = stack[point];
      stack.erase(stack.begin() + point);
      stack.push_back(data);
      break;
    }
    case opcode_STCKGET2: {
      int point = pop().getLong();
      stack.push_back(stack[point]);
      break;
    }
    case opcode_STCKMOV: {
      int point = pop().getLong();
      Value data = pop();
      stack.insert(stack.begin() + point, data);
      break;
    }
    case opcode_STCKDEL: {
      stack.erase(stack.begin() + pop().getLong());
      break;
    }
    case opcode_PUTARR:
      stack.push_back(Array);
      break;
    case opcode_MKARR: {
      long size = pop().getLong();
      Value tmp(Array);
      for (; size > 0; size--) {
        tmp.insert(0, pop());
      }
      stack.push_back(tmp);
      break;
    }
    case opcode_SET: {
      long point = pop().getLong();
      Value data = pop();
      stack[stack.size() - 1].set(point, data);
      break;
    }
    case opcode_GET: {
      long point = pop().getLong();
      Value array = pop();
      stack.push_back(array.get(point));
      break;
    }
    case opcode_APPEND: {
      Value data = pop();
      stack[stack.size() - 1].append(data);
      break;
    }
    case opcode_INSERT: {
      long point = pop().getLong();
      Value data = pop();
      stack[stack.size() - 1].insert(point, data);
      break;
    }
  }
  return res;
}
