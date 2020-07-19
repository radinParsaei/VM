#ifndef VM_H
#define VM_H

//VM controllers and stack controllers (0-15 registered for this category)
#define EXIT    0
#define PUT     1
#define POP     2
//operations (15-50 registered for this category)
#define ADD     15
#define SUB     16
#define MUL     17
#define DIV     18
#define MOD     19
#define EQ      20
#define FEQ     21
#define GT      22
#define GE      23
#define LT      24
#define LE      25
#define LAND    26
#define LOR     27
#define AND     28
#define OR      29
#define NOT     30
#define LNOT    31
#define LSHIFT  32
#define RSHIFT  33
#define XOR     34
#define NEG     35    //negative
//memory-related opcodes (50-65 registered for this category)
#define MEMSET  50
#define MEMGET  51
#define MEMSIZE 52
#define MEMPUT  53
#define MEMINS  54
#define MEMDEL  55
//type conversion opcodes (65-75 registered for this category)
#define TONUM   65
#define TOTXT   66
#define ISNUM   67
#define CANNUM  68
#define TOBOOL  69
//run cycle controllers (75-100 registered for this category)
#define REC     75
#define END     76
#define RUN     77
#define REPEAT  78
#define BREAK   79
#define IFTRUN  80    //if(top of stack) RUN
#define IFFRUN  81    //if(!top of stack) RUN
#define WTRUN   82    //while(top of stack) RUN
#define WFRUN   83    //while(!top of stack) RUN
#define THREAD  84
//library calls and input/output, etc (100- registered for this category)
#define DLCALL  100
#define PRINT   101

#include "VM_confs.h"
#include <iostream>
#include <vector>
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
    ~VM();
    bool run(std::vector<Value> prog, bool forceRun = true, int pc = 0);
    bool run1(int prog, Value arg = 0);
    void setInternalLibraryFunction(void (*internalLibraryFunction) (Value, VM*));
    std::vector<Value> getStack();
    void setStack(std::vector<Value> v);
    void attachMem(std::vector<Value> *mem);
    static std::vector<Value> assemble(Value line);
    static Value disassemble(int prog, Value val);
  private:
    std::vector<Value> stack;//stack memory
    std::vector<Value> *mempointer;//storage for saving variables data
#if THREADING == PROTOTHREADING
    std::vector<Thread> threads;
#endif
    Value pop();//pops a data from the stack
    int rec = 0;
    int recsize;
    int exit_code;
    bool isBreaked = false;
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
