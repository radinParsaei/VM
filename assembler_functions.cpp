#include "assembler_functions.h"
#include "VM.h"

std::vector<Record> mkRec(std::vector<value> vals){
  std::vector<Record> records;
  for(value v : vals){
    if(VM::getValType(v) == TYPE_NUM){
      Record r;
      r.type = TYPE_NUM;
      r.value = std::get<double>(v);
      records.push_back(r);
    } else {
      Record r;
      std::string str = std::get<std::string>(v);
      r.type = TYPE_TEXT;
      for (size_t i = 0; i < str.size(); i++) {
        r.value = str[i];
        records.push_back(r);
      }
    }
  }
  return records;
}
