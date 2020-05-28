#include <VM.h>
#ifndef LIB_NAME
#error please define LIB_NAME
#endif
#ifndef LIB_VERSION
#error please define LIB_VERSION
#endif
#ifdef STATIC_BUILD_MODE
#define APPEND2(data1 ,data2) data1 ## _ ## data2
#define APPEND1(data1 ,data2) APPEND2(data1 ,data2)
#define LIB_FUNCTION(FN_NAME) extern "C" std::vector<Value> APPEND1(APPEND1(LIB_NAME, LIB_VERSION), FN_NAME) (std::vector<Value> stack)
#define LIB_FUNCTION_INLINE(FN_NAME) extern "C" inline std::vector<Value> APPEND1(APPEND1(LIB_NAME, LIB_VERSION), FN_NAME) (std::vector<Value> stack)
#else
#define LIB_FUNCTION(FN_NAME) extern "C" std::vector<Value> FN_NAME(std::vector<Value> stack)
#define LIB_FUNCTION_INLINE(FN_NAME) extern "C" inline std::vector<Value> FN_NAME(std::vector<Value> stack)
#endif
#define str1(DATA) #DATA
#define str(DATA) str1(DATA)

LIB_FUNCTION_INLINE(GET_INFO) {
  int8_t i = stack[stack.size() - 1].getLong();
  stack.pop_back();
  switch (i) {
    case 0:
      stack.push_back(str(LIB_NAME));
      break;
    case 1:
      stack.push_back(str(LIB_VERSION));
      break;
#ifdef LIB_AUTHOR
    case 2:
      stack.push_back(str(LIB_AUTHOR));
      break;
#endif
#ifdef LIB_LICENSE
    case 3:
      stack.push_back(str(LIB_LICENSE));
      break;
#endif
#ifdef LIB_DESCRIPTION
    case 4:
      stack.push_back(LIB_DESCRIPTION);
      break;
#endif
    default:
      stack.push_back("UNKNOWN");
  }
  return stack;
}
