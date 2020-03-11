#include "../../VM.h"

using namespace std;

vector<value> strReplace(vector<value> stack){
  string str = VM::val2str(stack[stack.size() - 1]);
  stack.pop_back();
  string from = VM::val2str(stack[stack.size() - 1]);
  stack.pop_back();
  string to = VM::val2str(stack[stack.size() - 1]);
  stack.pop_back();
  if(from.empty()){
    stack.push_back(str);
    return stack;
  }
  size_t start_pos = 0;
  while((start_pos = str.find(from, start_pos)) != string::npos) {
    str.replace(start_pos, from.length(), to);
    start_pos += to.length();
  }
  stack.push_back(str);
  return stack;
}
