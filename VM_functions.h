#ifndef VM_FUNCTIONS_H
#define VM_FUNCTIONS_H

#define TYPE_NUM  0
#define TYPE_TEXT 1

#include <iostream>
#include <vector>
#include <string.h>
#include <sstream>
#include <cassert>
#include <variant>
#include <thread>
#include "VM.h"

typedef std::variant<double, std::string> value;
inline std::vector<value> stack;
inline std::vector<value> *mem = new std::vector<value>();

extern "C" void DLCALL_task();
extern "C" void PRINT_task();
extern "C" void ADD_task();
extern "C" void SUB_task();
extern "C" void MUL_task();
extern "C" void DIV_task();
extern "C" void MOD_task();
extern "C" void EQ_task();
extern "C" void FEQ_task();
extern "C" void GT_task();
extern "C" void GE_task();
extern "C" void LT_task();
extern "C" void LE_task();
extern "C" void LAND_task();
extern "C" void LOR_task();
extern "C" void AND_task();
extern "C" void OR_task();
extern "C" void NOT_task();
extern "C" void LNOT_task();
extern "C" void LSHIFT_task();
extern "C" void RSHIFT_task();
extern "C" void XOR_task();
extern "C" void NEG_task();
extern "C" void RUN_task();
extern "C" void WTRUN_task();
extern "C" void WFRUN_task();
extern "C" void IFTRUN_task();
extern "C" void IFFRUN_task();
extern "C" void THREAD_task();
extern "C" void POP_task();
extern "C" void REPEAT_task();
extern "C" void PRINTLN_task();
extern "C" void LOGSTCK_task();
extern "C" void PUT_tasks(const char* data);
extern "C" void PUT_taski(int data);
extern "C" void MEMSET_task();
extern "C" void MEMGET_task();
extern "C" void MEMSIZE_task();
extern "C" void MEMPUT_task();
extern "C" void MEMINS_task();
extern "C" void EXIT_task();
#endif
