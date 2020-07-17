#include "VM.h"
#include <fstream>
#include "VM_binaries.h"

using namespace std;
using namespace VM_BINARIES;

int main(int argc, char const *argv[]){
  if(argc < 2){
    cerr << "please enter a binary file name\n";
    return 1;
  }
  VM vm;
  vector<Value> mem;
  vm.attachMem(&mem);
  vm.autoKill = true;
  ifstream f (argv[1], ios::binary);
  if(!f){
    cerr << "can't open file" << '\n';
    return 1;
  }
  runBinFromStream(f, vm);
	return 0;
}
