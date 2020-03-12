#include "assembler_functions.h"
#include "VM.h"

using namespace std;

string rtrim(string s) {
  int i = 0;
  while (s[i] == ' ' || s[i] == '\t'){
    i++;
    s = s.substr(i, s.size() - i);
  }
  return s;
}

vector<value> assemble(string line){
  vector<value> prog;
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
  } else if(line.find("GT") == 0){
    prog.push_back(GT);
  } else if(line.find("GE") == 0){
    prog.push_back(GE);
  } else if(line.find("EXIT") == 0){
    prog.push_back(EXIT);
  }
  return prog;
}

vector<Record> mkRec(vector<value> vals){
  vector<Record> records;
  for(value v : vals){
    if(VM::getValType(v) == TYPE_NUM){
      Record r;
      r.type = TYPE_NUM;
      r.value = get<double>(v);
      records.push_back(r);
    } else {
      Record r;
      string str = get<string>(v);
      r.type = TYPE_TEXT;
      for (size_t i = 0; i < str.size(); i++) {
        r.value = str[i];
        records.push_back(r);
      }
    }
  }
  return records;
}