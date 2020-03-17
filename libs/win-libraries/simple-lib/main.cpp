#include "test.h"

vector<value> __declspec(dllexport) __stdcall test(vector<value> stack){
    cout << "Hello";
    stack.push_back("Hello World");
    return stack;
}
