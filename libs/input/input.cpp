#include "input.h"

LIB_FUNCTION(input) {
  std::string data;
  std::getline(std::cin, data);
  vm->push(Value(data));
}
