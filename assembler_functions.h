#ifndef ASSEMBLER_FUNCTIONS_H
#define ASSEMBLER_FUNCTIONS_H
#include "VM.h"

struct Record{
  double value = 0;
  bool type = TYPE_NUM;
};

std::vector<value> assemble(std::string line);
std::vector<Record> mkRec(std::vector<value> vals);

#endif
