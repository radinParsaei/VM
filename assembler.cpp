#include <fstream>
#include "VM.h"
#include "VM_binaries.h"

using namespace std;
using namespace VM_BINARIES;

int main(int argc, char const **argv) {
  if(argc < 2){
    cerr << "please enter a file name to assemble\n";
    return 1;
  }
  string filename = "out.bin";
  if(argc >= 3){
    filename = argv[2];
  }
  mpf_set_default_prec(1024);
  ofstream ofile;
  ofile.open(filename);
  ifstream fin;
  fin.open(argv[1]);
  vector<Value> prog;
  string line;
  while(getline(fin, line)){
    for(Value val : VM::assemble(line)){
      prog.push_back(val);
    }
  }
  fin.close();
  cout << "----------SAVING TO " + filename + "----------" << endl;
  ofile << mkBin(prog);
  ofile.close();
  return 0;
}
