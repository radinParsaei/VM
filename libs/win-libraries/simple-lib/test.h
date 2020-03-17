#ifndef TEST_H
#define TEST_H

#include <variant>
#include <vector>
#include <iostream>

using namespace std;
typedef variant<double, string> value;

vector<value> __declspec(dllexport) __stdcall test(vector<value> stack);

#endif // TEST_H
