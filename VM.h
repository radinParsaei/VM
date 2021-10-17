#ifndef VM_H
#define VM_H

//VM controllers and stack controllers (0-15 registered for this category)
#define opcode_EXIT     0
#define opcode_PUT      1
#define opcode_POP      2
#define opcode_STCKGET  3
#define opcode_STCKMOV  4
#define opcode_STCKDEL  5
#define opcode_STCKGET2 6
#define opcode_PUTARR   7
#define opcode_MKARR    8
//operations (15-50 registered for this category)
#define opcode_ADD     15
#define opcode_SUB     16
#define opcode_MUL     17
#define opcode_DIV     18
#define opcode_MOD     19
#define opcode_EQ      20
#define opcode_FEQ     21
#define opcode_GT      22
#define opcode_GE      23
#define opcode_LT      24
#define opcode_LE      25
#define opcode_LAND    26
#define opcode_LOR     27
#define opcode_AND     28
#define opcode_OR      29
#define opcode_NOT     30
#define opcode_LNOT    31
#define opcode_LSHIFT  32
#define opcode_RSHIFT  33
#define opcode_XOR     34
#define opcode_NEG     35    //negative
#define opcode_POW     36
#define opcode_SET     37
#define opcode_GET     38
#define opcode_APPEND  39
#define opcode_INSERT  40
//memory-related opcodes (50-65 registered for this category)
#define opcode_MEMSET  50
#define opcode_MEMGET  51
#define opcode_MEMSIZE 52
#define opcode_MEMPUT  53
#define opcode_MEMINS  54
#define opcode_MEMDEL  55
//type conversion opcodes (65-75 registered for this category)
#define opcode_TONUM   65
#define opcode_TOTXT   66
#define opcode_ISNUM   67
#define opcode_CANNUM  68
#define opcode_TOBOOL  69
#define opcode_TOPTR   70
//run cycle controllers (75-100 registered for this category)
#define opcode_REC     75
#define opcode_END     76
#define opcode_RUN     77
#define opcode_REPEAT  78
#define opcode_BREAK   79
#define opcode_IFTRUN  80    //if(top of stack) RUN
#define opcode_IFFRUN  81    //if(!top of stack) RUN
#define opcode_WTRUN   82    //while(top of stack) RUN
#define opcode_WFRUN   83    //while(!top of stack) RUN
#define opcode_THREAD  84
#define opcode_SKIP    85
#define opcode_IFSKIP  86
#define opcode_MKFN    87
#define opcode_CALLFN  88
#define opcode_EXITFN  89
#define opcode_CONTINU 90
//library calls and input/output, etc (100- registered for this category)
#define opcode_DLCALL  100
#define opcode_PRINT   101

#include "VM_confs.h"
#ifndef USE_ARDUINO_STRING
#include <iostream>
#endif
#include <vector>
#include <map>
#ifndef DYNAMIC_LIBS_NOT_AVAILABLE
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#endif
#if THREADING != PROTOTHREADING
#include <thread>
#endif
#define NUMBER_COMPARISON_OPERATORS
#define VALUE_MULTI_TYPE_SUPPORT
#include "value.h"


#ifdef VM_STACKLOG
#warning VM_STACKLOG enabled
#endif

#ifdef VM_DISASSEMBLE
#warning VM_DISASSEMBLE enabled
#endif


class Thread;
class VM {
  public:
    void printStack();
    bool running;
    bool autoKill;
    void (*internalLibraryFunction)(Value, VM*);
    VM();
#if THREADING == PROTOTHREADING
    ~VM();
    void runAllProtoThreads();
#endif
    bool run(std::vector<Value> prog, bool forceRun = true, int pc = 0);
    bool run1(int prog, Value arg = 0);
    void setInternalLibraryFunction(void (*internalLibraryFunction) (Value, VM*));
    std::vector<Value> getStack();
    std::vector<Value>* getMemoryPointer();
    void setStack(std::vector<Value> v);
    void attachMem(std::vector<Value> *mem);
    static std::vector<Value> assemble(Value line);
    static Value disassemble(int prog, Value val);
    Value pop();//pops a data from the stack
    void push(Value);
    void set(size_t addr, Value data);
  private:
    int skip = 1;
    std::vector<Value> stack;//stack memory
    std::vector<Value> *mempointer;//storage for saving variables data
    std::map< int, std::vector<Value> > functions;
#if THREADING == PROTOTHREADING
    std::vector<Thread> threads;
#endif
    int rec = 0;
    int recsize;
    int exit_code;
    bool isBreaked = false;
    bool isContinued = false;
    bool fnExited = false;
};

#if THREADING == PROTOTHREADING
class Thread {
private:
  size_t pc = 0;
  size_t s;
  Value* program;
  VM vm;
public:
  Thread(std::vector<Value> program, std::vector<Value>* mem) {
    s = program.size();
    this->program = new Value[program.size()];
    for (size_t i = 0; i < program.size(); i++) {
      this->program[i] = program[i];
    }
    vm.attachMem(mem);
  }
  Thread(Value* program, size_t s, std::vector<Value>* mem) {
    this->program = program;
    this->s = s;
    vm.attachMem(mem);
  }
  void runNext() {
    pc += vm.run1(program[pc].getLong(), (pc < s - 1? program[pc + 1] : 0));
    pc++;
  }
  bool isFinished() {
    return pc >= s;
  }
};
#endif
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
typedef void (__stdcall *dlfunc)(VM*);
#else
typedef void (*dlfunc)(VM*);
#endif

#include "VM_exts.h"
