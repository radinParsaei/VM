#include "VM.h"
#include <fstream>
#include <sstream>
#include "VM_binaries.h"

using namespace std;
using namespace VM_BINARIES;

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
  mpf_set_default_prec(1024);
  vector<Value> vals = parseBin(string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>()));
  for(int c = 0; c < vals.size(); c++){
    if (vals[c].getType() == VALUE_TYPE_NUMBER) {
      cout << VM::disassemble(vals[c].getLong(), (vals.size() - 1 == c)? 0:vals[c + 1]) << endl;
      if (vals[c].getNumber() == PUT) c++;
    }
  }
  f.close();
	return 0;
}
