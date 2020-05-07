#include "assembler_functions.h"
#include "VM.h"

using namespace std;

char* rtrim(char* s) {
  int i = 0;
  while (s[i] == ' ' || s[i] == '\t'){
    i++;
    s = Utils::substring(s, strlen(s) - i, i);
  }
  return s;
}

vector<Value> assemble(char* line){
  vector<Value> prog;
  line = rtrim(line);
  if(Utils::find(line, Utils::stringDuplicate("PUT")) == 0){
    line = Utils::substring(line, strlen(line) - 3, 3);
    prog.push_back(PUT);
    line = rtrim(line);
    if(Utils::find(line, Utils::stringDuplicate("NUM")) == 0){
      line = rtrim(Utils::substring(line, strlen(line) - 3, 3));
      int i = 0;
      while ((isdigit(line[i]) || line[i] == '.') || line[i] == '-')i++;
      prog.push_back(NUMBER(Utils::substring(line, i)));
    } else {
      line = Utils::stringDuplicate(Utils::substring(line, strlen(line) - 3, 3));
      line = Utils::stringDuplicate(Utils::replace(line, "\\n", "\n"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\n", "\\n"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\t", "\t"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\t", "\\t"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\r", "\r"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\r", "\\r"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\a", "\a"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\a", "\\a"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\b", "\b"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\b", "\\b"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\f", "\f"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\f", "\\f"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\'", "\'"));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\"", "\""));
      line = Utils::stringDuplicate(Utils::replace(line, "\\\\", "\\"));
      prog.push_back(line);
    }
  } else if(Utils::find(line, Utils::stringDuplicate("ADD")) == 0){
    prog.push_back(ADD);
  } else if(Utils::find(line, Utils::stringDuplicate("SUB")) == 0){
    prog.push_back(SUB);
  } else if(Utils::find(line, Utils::stringDuplicate("MUL")) == 0){
    prog.push_back(MUL);
  } else if(Utils::find(line, Utils::stringDuplicate("PRINTLN")) == 0){
    prog.push_back(PRINTLN);
  } else if(Utils::find(line, Utils::stringDuplicate("DIV")) == 0){
    prog.push_back(DIV);
  } else if(Utils::find(line, Utils::stringDuplicate("MOD")) == 0){
    prog.push_back(MOD);
  } else if(Utils::find(line, Utils::stringDuplicate("PRINT")) == 0){
    prog.push_back(PRINT);
  } else if(Utils::find(line, Utils::stringDuplicate("DLCALL")) == 0){
    prog.push_back(DLCALL);
  } else if(Utils::find(line, Utils::stringDuplicate("REC")) == 0){
    prog.push_back(REC);
  } else if(Utils::find(line, Utils::stringDuplicate("END")) == 0){
    prog.push_back(END);
  } else if(Utils::find(line, Utils::stringDuplicate("RUN")) == 0){
    prog.push_back(RUN);
  } else if(Utils::find(line, Utils::stringDuplicate("POP")) == 0){
    prog.push_back(POP);
  } else if(Utils::find(line, Utils::stringDuplicate("LOGSTCK")) == 0){
    prog.push_back(LOGSTCK);
  } else if(Utils::find(line, Utils::stringDuplicate("REPEAT")) == 0){
    prog.push_back(REPEAT);
  } else if(Utils::find(line, Utils::stringDuplicate("EQ")) == 0){
    prog.push_back(EQ);
  } else if(Utils::find(line, Utils::stringDuplicate("FEQ")) == 0){
    prog.push_back(FEQ);
  } else if(Utils::find(line, Utils::stringDuplicate("GT")) == 0){
    prog.push_back(GT);
  } else if(Utils::find(line, Utils::stringDuplicate("GE")) == 0){
    prog.push_back(GE);
  } else if(Utils::find(line, Utils::stringDuplicate("LT")) == 0){
    prog.push_back(LT);
  } else if(Utils::find(line, Utils::stringDuplicate("LE")) == 0){
    prog.push_back(LE);
  } else if(Utils::find(line, Utils::stringDuplicate("LAND")) == 0){
    prog.push_back(LAND);
  } else if(Utils::find(line, Utils::stringDuplicate("AND")) == 0){
    prog.push_back(AND);
  } else if(Utils::find(line, Utils::stringDuplicate("LOR")) == 0){
    prog.push_back(LOR);
  } else if(Utils::find(line, Utils::stringDuplicate("OR")) == 0){
    prog.push_back(OR);
  } else if(Utils::find(line, Utils::stringDuplicate("NOT")) == 0){
    prog.push_back(NOT);
  } else if(Utils::find(line, Utils::stringDuplicate("LNOT")) == 0){
    prog.push_back(LNOT);
  } else if(Utils::find(line, Utils::stringDuplicate("LSHIFT")) == 0){
    prog.push_back(LSHIFT);
  } else if(Utils::find(line, Utils::stringDuplicate("RSHIFT")) == 0){
    prog.push_back(RSHIFT);
  } else if(Utils::find(line, Utils::stringDuplicate("XOR")) == 0){
    prog.push_back(XOR);
  } else if(Utils::find(line, Utils::stringDuplicate("NEG")) == 0){
    prog.push_back(NEG);
  } else if(Utils::find(line, Utils::stringDuplicate("BREAK")) == 0){
    prog.push_back(BREAK);
  } else if(Utils::find(line, Utils::stringDuplicate("WTRUN")) == 0){
    prog.push_back(WTRUN);
  } else if(Utils::find(line, Utils::stringDuplicate("WFRUN")) == 0){
    prog.push_back(WFRUN);
  } else if(Utils::find(line, Utils::stringDuplicate("IFFRUN")) == 0){
    prog.push_back(IFFRUN);
  } else if(Utils::find(line, Utils::stringDuplicate("IFTRUN")) == 0){
    prog.push_back(IFTRUN);
  } else if(Utils::find(line, Utils::stringDuplicate("THREAD")) == 0){
    prog.push_back(THREAD);
  } else if(Utils::find(line, Utils::stringDuplicate("MEMGET")) == 0){
    prog.push_back(MEMGET);
  } else if(Utils::find(line, Utils::stringDuplicate("MEMSET")) == 0){
    prog.push_back(MEMSET);
  } else if(Utils::find(line, Utils::stringDuplicate("MEMSIZE")) == 0){
    prog.push_back(MEMSIZE);
  } else if(Utils::find(line, Utils::stringDuplicate("MEMPUT")) == 0){
    prog.push_back(MEMPUT);
  } else if(Utils::find(line, Utils::stringDuplicate("MEMINS")) == 0){
    prog.push_back(MEMINS);
  } else if(Utils::find(line, Utils::stringDuplicate("MEMDEL")) == 0){
    prog.push_back(MEMDEL);
  } else if(Utils::find(line, Utils::stringDuplicate("TOTXT")) == 0){
    prog.push_back(TOTXT);
  } else if(Utils::find(line, Utils::stringDuplicate("TONUM")) == 0){
    prog.push_back(TONUM);
  } else if(Utils::find(line, Utils::stringDuplicate("CANNUM")) == 0){
    prog.push_back(CANNUM);
  } else if(Utils::find(line, Utils::stringDuplicate("ISNUM")) == 0){
    prog.push_back(ISNUM);
  } else if(Utils::find(line, Utils::stringDuplicate("EXIT")) == 0){
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
      r.value = stof(v.getNumber().toString());
      records.push_back(r);
    } else {
      Record r;
      string str = v.getString();
      r.type = TYPE_TEXT;
      for (size_t i = 0; i < str.size(); i++) {
        r.value = str[i];
        records.push_back(r);
      }
    }
  }
  return records;
}
