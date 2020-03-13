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
#include "VM.h"

typedef std::variant<double, std::string> value;
inline std::vector<value> stack;

void DLCALL_task();
void PRINT_task();
void ADD_task();
void SUB_task();
void MUL_task();
void DIV_task();
void MOD_task();
void EQ_task();
void FEQ_task();
void GT_task();
void GE_task();
void LT_task();
void LE_task();
void RUN_task();
void POP_task();
void REPEAT_task();
void PRINTLN_task();
void LOGSTCK_task();
void PUT_task(const char* data);
void PUT_task(int data);
void EXIT_task();
#endif
