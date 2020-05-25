#include <VM.h>
#define LIB_FUNCTION(FN_NAME) extern "C" void FN_NAME(std::vector<Value>* stack)
#ifndef LIB_NAME
#error please define LIB_NAME
#endif
#ifndef LIB_VERSION
#error please define LIB_VERSION
#endif
const char* GET_LIB_INFO(int8_t i) {
  switch (i) {
    case 0:
      return LIB_NAME;
    case 1:
      return LIB_VERSION;
#ifdef LIB_AUTHOR
    case 2:
      return LIB_AUTHOR;
#endif
#ifdef LIB_LICENSE
    case 3:
      return LIB_LICENSE;
#endif
  }
  return "UNKNOWN";
}

LIB_FUNCTION(GET_INFO) {
  const char* a = GET_LIB_INFO(stack->at(stack->size() - 1).getLong());
  stack->pop_back();
  stack->push_back(a);
}
