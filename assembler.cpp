#include <string.h>
#include <iostream>
#include <vector>
#include <variant>
#include <fstream>
#include "VM.h"
#include "assembler_functions.h"

using namespace std;

int main(int argc, char const **argv) {
  if(argc < 2){
    cerr << "please enter a file name to assemble\n";
    return 1;
  }
  string filename = "out.bin";
  if(argc >= 3){
    filename = argv[2];
  }
  ofstream ofile;
  ofile.open(filename, ios::binary);
  ifstream fin;
  fin.open(argv[1]);
  vector<value> prog;
  string line;
  while(getline(fin, line)){
    for(value val : assemble(line)){
      prog.push_back(val);
    }
  }
  fin.close();
  cout << "----------SAVING TO " + filename + "----------" << endl;
  for (Record r : mkRec(prog)) {
    ofile.write(reinterpret_cast<char *>(&r), sizeof(Record));
  }
  ofile.close();
  return 0;
}
