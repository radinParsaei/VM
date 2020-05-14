#include "assembler_functions.h"
#include "VM.h"

using namespace std;

char* rtrim(char* s) {
  int i = 0;
  while (s[i] == ' ' || s[i] == '\t'){
    i++;
    char* pS = s;
    s = Utils::substring(s, Utils::stringLength(s) - i, i);
    free(pS);
  }
  return s;
}

char* freeAndDuplicate(const char* data) {
  char* res = Utils::stringDuplicate(data);
  delete[] data;
  return res;
}

vector<Value> assemble(char* line){
  vector<Value> prog;
  char* pline;
  line = rtrim(line);
  if(Utils::find(line, "PUT") == 0){
    line = Utils::substring(line, Utils::stringLength(line) - 3, 3);
    prog.push_back(PUT);
    line = rtrim(line);
    if(Utils::find(line, "NUM") == 0){
      line = rtrim(Utils::substring(line, Utils::stringLength(line) - 3, 3));
      int i = 0;
      while ((isdigit(line[i]) || line[i] == '.') || line[i] == '-')i++;
      prog.push_back(NUMBER(Utils::substring(line, i)));
    } else {
      pline = line;
      line = Utils::substring(line, Utils::stringLength(line) - 3, 3);
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\n", "\n"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\n", "\\n"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\t", "\t"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\t", "\\t"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\r", "\r"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\r", "\\r"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\a", "\a"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\a", "\\a"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\b", "\b"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\b", "\\b"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\f", "\f"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\f", "\\f"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\'", "\'"));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\"", "\""));
      free(pline);
      pline = line;
      line = freeAndDuplicate(Utils::replace(line, "\\\\", "\\"));
      free(pline);
      prog.push_back(line);
    }
  } else if(Utils::find(line, "ADD") == 0){
    prog.push_back(ADD);
  } else if(Utils::find(line, "SUB") == 0){
    prog.push_back(SUB);
  } else if(Utils::find(line, "MUL") == 0){
    prog.push_back(MUL);
  } else if(Utils::find(line, "PRINTLN") == 0){
    prog.push_back(PRINTLN);
  } else if(Utils::find(line, "DIV") == 0){
    prog.push_back(DIV);
  } else if(Utils::find(line, "MOD") == 0){
    prog.push_back(MOD);
  } else if(Utils::find(line, "PRINT") == 0){
    prog.push_back(PRINT);
  } else if(Utils::find(line, "DLCALL") == 0){
    prog.push_back(DLCALL);
  } else if(Utils::find(line, "REC") == 0){
    prog.push_back(REC);
  } else if(Utils::find(line, "END") == 0){
    prog.push_back(END);
  } else if(Utils::find(line, "RUN") == 0){
    prog.push_back(RUN);
  } else if(Utils::find(line, "POP") == 0){
    prog.push_back(POP);
  } else if(Utils::find(line, "LOGSTCK") == 0){
    prog.push_back(LOGSTCK);
  } else if(Utils::find(line, "REPEAT") == 0){
    prog.push_back(REPEAT);
  } else if(Utils::find(line, "EQ") == 0){
    prog.push_back(EQ);
  } else if(Utils::find(line, "FEQ") == 0){
    prog.push_back(FEQ);
  } else if(Utils::find(line, "GT") == 0){
    prog.push_back(GT);
  } else if(Utils::find(line, "GE") == 0){
    prog.push_back(GE);
  } else if(Utils::find(line, "LT") == 0){
    prog.push_back(LT);
  } else if(Utils::find(line, "LE") == 0){
    prog.push_back(LE);
  } else if(Utils::find(line, "LAND") == 0){
    prog.push_back(LAND);
  } else if(Utils::find(line, "AND") == 0){
    prog.push_back(AND);
  } else if(Utils::find(line, "LOR") == 0){
    prog.push_back(LOR);
  } else if(Utils::find(line, "OR") == 0){
    prog.push_back(OR);
  } else if(Utils::find(line, "NOT") == 0){
    prog.push_back(NOT);
  } else if(Utils::find(line, "LNOT") == 0){
    prog.push_back(LNOT);
  } else if(Utils::find(line, "LSHIFT") == 0){
    prog.push_back(LSHIFT);
  } else if(Utils::find(line, "RSHIFT") == 0){
    prog.push_back(RSHIFT);
  } else if(Utils::find(line, "XOR") == 0){
    prog.push_back(XOR);
  } else if(Utils::find(line, "NEG") == 0){
    prog.push_back(NEG);
  } else if(Utils::find(line, "BREAK") == 0){
    prog.push_back(BREAK);
  } else if(Utils::find(line, "WTRUN") == 0){
    prog.push_back(WTRUN);
  } else if(Utils::find(line, "WFRUN") == 0){
    prog.push_back(WFRUN);
  } else if(Utils::find(line, "IFFRUN") == 0){
    prog.push_back(IFFRUN);
  } else if(Utils::find(line, "IFTRUN") == 0){
    prog.push_back(IFTRUN);
  } else if(Utils::find(line, "THREAD") == 0){
    prog.push_back(THREAD);
  } else if(Utils::find(line, "MEMGET") == 0){
    prog.push_back(MEMGET);
  } else if(Utils::find(line, "MEMSET") == 0){
    prog.push_back(MEMSET);
  } else if(Utils::find(line, "MEMSIZE") == 0){
    prog.push_back(MEMSIZE);
  } else if(Utils::find(line, "MEMPUT") == 0){
    prog.push_back(MEMPUT);
  } else if(Utils::find(line, "MEMINS") == 0){
    prog.push_back(MEMINS);
  } else if(Utils::find(line, "MEMDEL") == 0){
    prog.push_back(MEMDEL);
  } else if(Utils::find(line, "TOTXT") == 0){
    prog.push_back(TOTXT);
  } else if(Utils::find(line, "TONUM") == 0){
    prog.push_back(TONUM);
  } else if(Utils::find(line, "CANNUM") == 0){
    prog.push_back(CANNUM);
  } else if(Utils::find(line, "ISNUM") == 0){
    prog.push_back(ISNUM);
  } else if(Utils::find(line, "EXIT") == 0){
    prog.push_back(EXIT);
  }
  return prog;
}

vector<Record> mkRec(vector<Value> vals){
  vector<Record> records;
  for(Value v : vals){
    if(v.getType() == TYPE_NUM){
      Record r;
      r.type = TYPE_NUM;
      r.value = v.getDouble();
      records.push_back(r);
    } else {
      Record r;
      const char* str = v.getString();
      r.type = TYPE_TEXT;
      for (size_t i = 0; i < Utils::stringLength(str); i++) {
        r.value = str[i];
        records.push_back(r);
      }
    }
  }
  return records;
}
