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

#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <variant>
#include <dlfcn.h>

#include "VM_confs.h"

#ifdef VM_STACKLOG
#warning VM_STACKLOG enabled
#endif

#ifdef VM_DISASSEMBLE
#warning VM_DISASSEMBLE enabled
#endif

typedef std::variant<double, std::string> value;
typedef std::vector<value> (*dlfunc)(std::vector<value> args);

class VM {
  public:
    void printStack();
    static bool disassemble(int prog, value val, std::string end = "\t");
    bool running;
    bool autoKill;
    VM();
    void run(std::vector<value> prog, bool forceRun = true, int pc = 0);
    bool run1(int prog, value arg);
    static std::string val2str(value v);
    static value add2val(value v1, value v2);
    static value sub2val(value v1, value v2);
    static value mul2val(value v1, value v2);
    static value div2val(value v1, value v2);
    static value mod2val(value v1, value v2);
    static value isEQ(value v1, value v2);
    static value isFEQ(value v1, value v2);
    static value isGT(value v1, value v2);
    static bool getValType(value v);
    static double toNUM(value v);
    std::vector<value> getStack();
    void setStack(std::vector<value> v);
  private:
    std::vector<value> stack;//stack memory
    value pop();//pops a data from the stack
    int rec = 0;
    int recsize;
    int exit_code;
};

#endif
