#include "VM.h"
#include <fstream>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]){
  if(argc < 2){
    cerr << "please enter a binary file name\n";
    return 1;
  }
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
      for(int c = 0; c < vals.size(); c++){
        if (vals[c].getType() == TYPE_NUM) {
          cout << VM::disassemble(vals[c].getLong(), (vals.size() - 1 == c)? 0:vals[c + 1]) << endl;
          if (vals[c].getNumber() == PUT) c++;
        }
      }
      vals.clear();
    }
  }
  cout << endl;
  f.close();
	return 0;
}
