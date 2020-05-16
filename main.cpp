#include "VM.h"
#include <fstream>

using namespace std;

int main(int argc, char const *argv[]){
  if(argc < 2){
    cerr << "please enter a binary file name\n";
    return 1;
  }
  VM *vm = new VM();
  vector<Value> mem;
  vm->attachMem(&mem);
  vm->autoKill = true;
  ifstream f(argv[1], ios::binary);
  if(!f){
    cerr << "can't open file" << '\n';
    return 1;
  }
  vector<Value> vals;
  VM::Record r;
  bool wait = false;
  while(f.read((char*)&r, sizeof(VM::Record))){
    wait = false;
    if(r.type == TYPE_TEXT){
      bool add = true;
      ostringstream stream;
      stream << (char)r.value;
      while(f.read((char*)&r, sizeof(VM::Record))){
        if(r.type != TYPE_NUM)
          stream << (char)r.value;
        else {
          vals.push_back(stream.str().c_str());
          add = false;
          vals.push_back(r.value);
          if(r.value == PUT){
            wait = true;
          }
          break;
        }
      }
      if(add)vals.push_back(stream.str().c_str());
    } else {
      vals.push_back(r.value);
      if(r.value == PUT){
        wait = true;
      }
    }
    if(!wait){
      vm->run(vals, false);
      vals.clear();
    }
  }
  f.close();
	return 0;
}
