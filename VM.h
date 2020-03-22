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

#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <variant>
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <thread>

#include "VM_confs.h"

#ifdef VM_STACKLOG
#warning VM_STACKLOG enabled
#endif

#ifdef VM_DISASSEMBLE
#warning VM_DISASSEMBLE enabled
#endif

typedef std::variant<double, std::string> value;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
typedef std::vector<value> (__stdcall *dlfunc)(std::vector<value>);
#else
typedef std::vector<value> (*dlfunc)(std::vector<value> args);
#endif

class VM {
  public:
    void printStack();
    static bool disassemble(int prog, value val, std::string end = "\t");
    bool running;
    bool autoKill;
    VM();
    void run(std::vector<value> prog, bool forceRun = true, int pc = 0);
    bool run1(int prog, value arg);
    static bool getValType(value v);
    static double toNUM(value v);
    static std::string val2str(value v);
    static value add2val(value v1, value v2);
    static value sub2val(value v1, value v2);
    static value mul2val(value v1, value v2);
    static value div2val(value v1, value v2);
    static value mod2val(value v1, value v2);
    static value isEQ(value v1, value v2);
    static value isFEQ(value v1, value v2);
    static value isGT(value v1, value v2);
    static value isGE(value v1, value v2);
    static value isLT(value v1, value v2);
    static value isLE(value v1, value v2);
    static value LAND2val(value v1, value v2); //logical AND
    static value LOR2val(value v1, value v2); //logical OR
    static value AND2val(value v1, value v2); //bitwise AND
    static value OR2val(value v1, value v2); //bitwise OR
    static value NOTval(value v); //bitwise NOT
    static value LNOTval(value v); //logical AND
    static value LSHIFT2val(value v1, value v2); //left shift
    static value RSHIFT2val(value v1, value v2); //right shift
    static value XOR2val(value v1, value v2); //XOR
    static value NEGval(value v); //NEG
    std::vector<value> getStack();
    void setStack(std::vector<value> v);
    void attachMem(std::vector<value> *mem);
  private:
    std::vector<value> stack;//stack memory
    std::vector<value> *mempointer;//storage for saving variables data
    value pop();//pops a data from the stack
    int rec = 0;
    int recsize;
    int exit_code;
    bool isBreaked = false;
};

#endif
