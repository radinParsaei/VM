#include "VM.h"
#include <fstream>
#include <sstream>
#include "assembler_functions.h"

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
  cout << "declare void @_Z8PUT_taskPKc(i8*)\n";
  cout << "declare void @_Z8PUT_taski(i32)\n";
  cout << "declare void @_Z8ADD_taskv()\n";
  cout << "declare void @_Z8MUL_taskv()\n";
  cout << "declare void @_Z8DIV_taskv()\n";
  cout << "declare void @_Z8MOD_taskv()\n";
  cout << "declare void @_Z7EQ_taskv()\n";
  cout << "declare void @_Z8FEQ_taskv()\n";
  cout << "declare void @_Z7GT_taskv()\n";
  cout << "declare void @_Z10PRINT_taskv()\n";
  cout << "declare void @_Z9EXIT_taskv()\n";
  cout << "declare void @_Z11DLCALL_taskv()\n";
  cout << "declare void @_Z8RUN_taskv()\n";
  cout << "declare void @_Z8POP_taskv()\n";
  cout << "declare void @_Z12LOGSTCK_taskv()\n";
  cout << "declare void @_Z12PRINTLN_taskv()\n";
  cout << "declare void @_Z11REPEAT_taskv()\n";
  vector<value> vals;
  Record r;
  bool wait = false;
  ostringstream strings;
  int strc = 0;
  int rec = 0;
  int recsize = 0;
  cout << "define i32 @main(i32, i8**) {\n";
  while(f.read((char*)&r, sizeof(Record))){
    wait = false;
    if(r.type == TYPE_TEXT){
      bool add = true;
      ostringstream stream;
      stream << (char)r.value;
      while(f.read((char*)&r, sizeof(Record))){
        if(r.type != TYPE_NUM)
          stream << (char)r.value;
        else {
          vals.push_back(stream.str());
          add = false;
          vals.push_back(r.value);
          if(r.value == PUT){
            wait = true;
          }
          break;
        }
      }
      if(add)vals.push_back(stream.str());
    } else {
      vals.push_back(r.value);
      if(r.value == PUT){
        wait = true;
      }
    }
    if(!wait){
      for(int c = 0; c < vals.size(); c++){
        value v = vals[c];
        if(rec){
          if(VM::getValType(vals[c]) == TYPE_NUM){
            if(std::get<double>(vals[c]) == END){
              rec--;
              if(rec == 0){
                cout << "call void @_Z8PUT_taski(i32 " << recsize << ")\n";
                recsize = 0;
                c--;
                continue;
              }
            }
            recsize++;
            cout << "call void @_Z8PUT_taski(i32 " << VM::val2str(vals[c]) << ")\n";
            if(std::get<double>(vals[c]) == PUT){
              c++;
              recsize++;
              if(VM::getValType(vals[c]) == TYPE_NUM){
                cout << "call void @_Z8PUT_taski(i32 " << VM::val2str(vals[c]) << ")\n";
              } else {
                strc++;
                strings << "@.str" << strc << " = private unnamed_addr constant [" << VM::val2str(vals[c]).size() + 1 << " x i8] c\"" << VM::val2str(vals[c]) << "\\00\"\n";
                cout << "call void @_Z8PUT_taskPKc(i8* getelementptr inbounds ([" << VM::val2str(vals[c]).size() + 1 << " x i8], [" << VM::val2str(vals[c]).size() + 1 << " x i8]* @.str" << strc << ", i32 0, i32 0))\n";
              }
            } else if(std::get<double>(vals[c]) == REC){
              rec++;
            }
          } else {
            strc++;
            strings << "@.str" << strc << " = private unnamed_addr constant [" << VM::val2str(vals[c]).size() + 1 << " x i8] c\"" << VM::val2str(vals[c]) << "\\00\"\n";
            cout << "call void @_Z8PUT_taskPKc(i8* getelementptr inbounds ([" << VM::val2str(vals[c]).size() + 1 << " x i8], [" << VM::val2str(vals[c]).size() + 1 << " x i8]* @.str" << strc << ", i32 0, i32 0))\n";
          }
        } else if (VM::getValType(v) == TYPE_NUM) {
            switch ((int)get<double>(v)) {
              case PUT:
                c++;
                if(VM::getValType(vals[c]) == TYPE_NUM){
                  cout << "call void @_Z8PUT_taski(i32 " << VM::val2str(vals[c]) << ")\n";
                } else {
                  strc++;
                  strings << "@.str" << strc << " = private unnamed_addr constant [" << VM::val2str(vals[c]).size() + 1 << " x i8] c\"" << VM::val2str(vals[c]) << "\\00\"\n";
                  cout << "call void @_Z8PUT_taskPKc(i8* getelementptr inbounds ([" << VM::val2str(vals[c]).size() + 1 << " x i8], [" << VM::val2str(vals[c]).size() + 1 << " x i8]* @.str" << strc << ", i32 0, i32 0))\n";
                }
                break;
              case EXIT:
                cout << "call void @_Z9EXIT_taskv()\n";
              case PRINT:
                cout << "call void @_Z10PRINT_taskv()\n";
                break;
              case ADD:
                cout << "call void @_Z8ADD_taskv()\n";
                break;
              case SUB:
                cout << "call void @_Z8SUB_taskv()\n";
                break;
              case MUL:
                cout << "call void @_Z8MUL_taskv()\n";
                break;
              case DIV:
                cout << "call void @_Z8DIV_taskv()\n";
                break;
              case MOD:
                cout << "call void @_Z8MOD_taskv()\n";
                break;
              case DLCALL:
                cout << "call void @_Z11DLCALL_taskv()\n";
                break;
              case REC:
                rec++;
                break;
              case RUN:
                cout << "call void @_Z8RUN_taskv()\n";
                break;
              case POP:
                cout << "call void @_Z8POP_taskv()\n";
                break;
              case LOGSTCK:
                cout << "call void @_Z12LOGSTCK_taskv()\n";
                break;
              case PRINTLN:
                cout << "call void @_Z12PRINTLN_taskv()\n";
                break;
              case REPEAT:
                cout << "call void @_Z11REPEAT_taskv()\n";
                break;
              case EQ:
                cout << "call void @_Z7EQ_taskv()\n";
                break;
              case FEQ:
                cout << "call void @_Z8FEQ_taskv()\n";
                break;
              case GT:
                cout << "call void @_Z7GT_taskv()\n";
                break;
            }
          }
        }
      vals.clear();
    }
  }
  cout << "ret i32 0\n";
  cout << "}\n";
  cout << strings.str();
  f.close();
	return 0;
}
