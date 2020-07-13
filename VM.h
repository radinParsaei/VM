#ifndef VM_H
#define VM_H

#define EXIT    0
#define PUT     1
#define ADD     2
#define SUB     3
#define MUL     4
#define DIV     5
#define MOD     6
#define PRINT   7
#define DLCALL  8
#define REC     9
#define END     10
#define RUN     11
#define POP     12
#define LOGSTCK 13
#define PRINTLN 14
#define REPEAT  15
#define EQ      16
#define FEQ     17
#define GT      18
#define GE      19
#define LT      20
#define LE      21
#define LAND    22
#define LOR     23
#define AND     24
#define OR      25
#define NOT     26
#define LNOT    27
#define LSHIFT  28
#define RSHIFT  29
#define XOR     30
#define NEG     31    //negative
#define BREAK   32
#define WTRUN   33    //while(top of stack) RUN
#define WFRUN   34    //while(!top of stack) RUN
#define IFTRUN  35    //if(top of stack) RUN
#define IFFRUN  36    //if(!top of stack) RUN
#define THREAD  37
#define MEMSET  38
#define MEMGET  39
#define MEMSIZE 40
#define MEMPUT  41
#define MEMINS  42
#define MEMDEL  43
#define TONUM   44
#define TOTXT   45
#define ISNUM   46
#define CANNUM  47

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
    struct Record{
      double value = 0;
      bool type = VALUE_TYPE_NUMBER;
    };
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
    static std::vector<Record> mkRec(std::vector<Value> vals);
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
