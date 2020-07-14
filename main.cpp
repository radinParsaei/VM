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
    if(r.type == VALUE_TYPE_TEXT){
      bool add = true;
      ostringstream stream;
      if (r.value >= 0) {
        stream << (char)r.value;
      } else if (r.value == -2) {
        vals.push_back(null);
        bool add = true;
        continue;
      } else if (r.value == -3) {
        vals.push_back(False);
        bool add = true;
        continue;
      } else if (r.value == -4) {
        vals.push_back(True);
        bool add = true;
        continue;
      }
      while(f.read((char*)&r, sizeof(VM::Record))){
        if(r.type != VALUE_TYPE_NUMBER) {
          if (r.value != -1) stream << (char)r.value;
          } else {
          vals.push_back(stream.str());
          add = false;
          vals.push_back(r.value);
          if(r.value == PUT){
            wait = true;
          }
          break;
        }
      }
      if(add)vals.push_back(stream.str());
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
  delete vm;
	return 0;
}
