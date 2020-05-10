#ifndef VM_H
#define VM_H

#define TYPE_NUM  0
#define TYPE_TEXT 1

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

#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <thread>
#include <BigNumber.h>
#include <value.h>

#include "VM_confs.h"

#ifdef VM_STACKLOG
#warning VM_STACKLOG enabled
#endif

#ifdef VM_DISASSEMBLE
#warning VM_DISASSEMBLE enabled
#endif

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
typedef std::vector<Value> (__stdcall *dlfunc)(std::vector<Value>);
#else
typedef std::vector<Value> (*dlfunc)(std::vector<Value> args);
#endif

class VM {
  public:
    void printStack();
    static bool disassemble(int prog, Value val, std::string end = "\t");
    bool running;
    bool autoKill;
    VM();
    void run(std::vector<Value> prog, bool forceRun = true, int pc = 0);
    bool run1(int prog, Value arg = 0);
    static BigNumber toNUM(Value v);
    static Value add2val(Value v1, Value v2);
    static Value sub2val(Value v1, Value v2);
    static Value mul2val(Value v1, Value v2);
    static Value div2val(Value v1, Value v2);
    static Value mod2val(Value v1, Value v2);
    static Value isEQ(Value v1, Value v2);
    static Value isFEQ(Value v1, Value v2);
    static Value isGT(Value v1, Value v2);
    static Value isGE(Value v1, Value v2);
    static Value isLT(Value v1, Value v2);
    static Value isLE(Value v1, Value v2);
    static Value LAND2val(Value v1, Value v2); //logical AND
    static Value LOR2val(Value v1, Value v2); //logical OR
    static Value AND2val(Value v1, Value v2); //bitwise AND
    static Value OR2val(Value v1, Value v2); //bitwise OR
    static Value NOTval(Value v); //bitwise NOT
    static Value LNOTval(Value v); //logical AND
    static Value LSHIFT2val(Value v1, Value v2); //left shift
    static Value RSHIFT2val(Value v1, Value v2); //right shift
    static Value XOR2val(Value v1, Value v2); //XOR
    static Value NEGval(Value v); //NEG
    std::vector<Value> getStack();
    void setStack(std::vector<Value> v);
    void attachMem(std::vector<Value> *mem);
  private:
    std::vector<Value> stack;//stack memory
    std::vector<Value> *mempointer;//storage for saving variables data
    Value pop();//pops a data from the stack
    int rec = 0;
    int recsize;
    int exit_code;
    bool isBreaked = false;
};

#endif
