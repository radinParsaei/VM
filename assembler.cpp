#include <string.h>
#include <iostream>
#include <vector>
#include <variant>
#include <fstream>
#include "VM.h"
#include "assembler_functions.h"

using namespace std;

string rtrim(string s) {
  int i = 0;
  while (s[i] == ' ' || s[i] == '\t'){
    i++;
    s = s.substr(i, s.size() - i);
  }
  return s;
}

int main(int argc, char const **argv) {
  if(argc < 2){
    cerr << "please enter a file name to assemble\n";
    return 1;
  }
  string filename = "out.bin";
  if(argc >= 3){
    filename = argv[2];
  }
  VM *vm = new VM();
  ofstream ofile;
  ofile.open(filename, ios::binary);
  vector<value> prog;
  ifstream fin;
  fin.open(argv[1]);
  string line;
  while(fin){
    getline(fin, line);
    line = rtrim(line);
    if(line.find("PUT") == 0){
      line = line.substr(3, line.size() - 3);
      prog.push_back(PUT);
      line = rtrim(line);
      if(line.find("NUM") == 0){
        line = rtrim(line.substr(3, line.size() - 3));
        int i = 0;
        while (isdigit(line[i++]));
        prog.push_back(stof(line.substr(0, i)));
      } else {
        prog.push_back(line.substr(3, line.size() - 3));
      }
    } else if(line.find("ADD") == 0){
      prog.push_back(ADD);
    } else if(line.find("SUB") == 0){
      prog.push_back(SUB);
    } else if(line.find("MUL") == 0){
      prog.push_back(MUL);
    } else if(line.find("PRINTLN") == 0){
      prog.push_back(PRINTLN);
    } else if(line.find("DIV") == 0){
      prog.push_back(DIV);
    } else if(line.find("MOD") == 0){
      prog.push_back(MOD);
    } else if(line.find("PRINT") == 0){
      prog.push_back(PRINT);
    } else if(line.find("DLCALL") == 0){
      prog.push_back(DLCALL);
    } else if(line.find("REC") == 0){
      prog.push_back(REC);
    } else if(line.find("END") == 0){
      prog.push_back(END);
    } else if(line.find("RUN") == 0){
      prog.push_back(RUN);
    } else if(line.find("POP") == 0){
      prog.push_back(POP);
    } else if(line.find("LOGSTCK") == 0){
      prog.push_back(LOGSTCK);
    } else if(line.find("REPEAT") == 0){
      prog.push_back(REPEAT);
    } else if(line.find("EQ") == 0){
      prog.push_back(EQ);
    } else if(line.find("FEQ") == 0){
      prog.push_back(FEQ);
    } else if(line.find("EXIT") == 0){
      prog.push_back(EXIT);
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
