#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H
#include "VM.h"

struct Record{
  double value = 0;
  bool type = TYPE_NUM;
};

std::vector<Value> assemble(char* line);
std::vector<Record> mkRec(std::vector<Value> vals);

#endif
