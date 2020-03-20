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
  cout << "declare void @PUT_tasks(i8*)\n";
  cout << "declare void @PUT_taski(i32)\n";
  cout << "declare void @ADD_task()\n";
  cout << "declare void @SUB_task()\n";
  cout << "declare void @MUL_task()\n";
  cout << "declare void @DIV_task()\n";
  cout << "declare void @MOD_task()\n";
  cout << "declare void @EQ_task()\n";
  cout << "declare void @FEQ_task()\n";
  cout << "declare void @GT_task()\n";
  cout << "declare void @GE_task()\n";
  cout << "declare void @LT_task()\n";
  cout << "declare void @LE_task()\n";
  cout << "declare void @LAND_task()\n";
  cout << "declare void @LOR_task()\n";
  cout << "declare void @AND_task()\n";
  cout << "declare void @OR_task()\n";
  cout << "declare void @NOT_task()\n";
  cout << "declare void @LNOT_task()\n";
  cout << "declare void @LSHIFT_task()\n";
  cout << "declare void @RSHIFT_task()\n";
  cout << "declare void @XOR_task()\n";
  cout << "declare void @PRINT_task()\n";
  cout << "declare void @EXIT_task()\n";
  cout << "declare void @DLCALL_task()\n";
  cout << "declare void @RUN_task()\n";
  cout << "declare void @POP_task()\n";
  cout << "declare void @LOGSTCK_task()\n";
  cout << "declare void @PRINTLN_task()\n";
  cout << "declare void @REPEAT_task()\n";
  cout << "declare void @WTRUN_task()\n";
  cout << "declare void @WFRUN_task()\n";
  cout << "declare void @IFTRUN_task()\n";
  cout << "declare void @IFFRUN_task()\n";
  cout << "declare void @NEG_task()\n";
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
                cout << "call void @PUT_taski(i32 " << recsize << ")\n";
                recsize = 0;
                c--;
                continue;
              }
            }
            recsize++;
            cout << "call void @PUT_taski(i32 " << VM::val2str(vals[c]) << ")\n";
            if(std::get<double>(vals[c]) == PUT){
              c++;
              recsize++;
              if(VM::getValType(vals[c]) == TYPE_NUM){
                cout << "call void @PUT_taski(i32 " << VM::val2str(vals[c]) << ")\n";
              } else {
                strc++;
                strings << "@.str" << strc << " = private unnamed_addr constant [" << VM::val2str(vals[c]).size() + 1 << " x i8] c\"" << VM::val2str(vals[c]) << "\\00\"\n";
                cout << "call void @PUT_tasks(i8* getelementptr inbounds ([" << VM::val2str(vals[c]).size() + 1 << " x i8], [" << VM::val2str(vals[c]).size() + 1 << " x i8]* @.str" << strc << ", i32 0, i32 0))\n";
              }
            } else if(std::get<double>(vals[c]) == REC){
              rec++;
            }
          } else {
            strc++;
            strings << "@.str" << strc << " = private unnamed_addr constant [" << VM::val2str(vals[c]).size() + 1 << " x i8] c\"" << VM::val2str(vals[c]) << "\\00\"\n";
            cout << "call void @PUT_tasks(i8* getelementptr inbounds ([" << VM::val2str(vals[c]).size() + 1 << " x i8], [" << VM::val2str(vals[c]).size() + 1 << " x i8]* @.str" << strc << ", i32 0, i32 0))\n";
          }
        } else if (VM::getValType(v) == TYPE_NUM) {
            switch ((int)get<double>(v)) {
              case PUT:
                c++;
                if(VM::getValType(vals[c]) == TYPE_NUM){
                  cout << "call void @PUT_taski(i32 " << VM::val2str(vals[c]) << ")\n";
                } else {
                  strc++;
                  strings << "@.str" << strc << " = private unnamed_addr constant [" << VM::val2str(vals[c]).size() + 1 << " x i8] c\"" << VM::val2str(vals[c]) << "\\00\"\n";
                  cout << "call void @PUT_tasks(i8* getelementptr inbounds ([" << VM::val2str(vals[c]).size() + 1 << " x i8], [" << VM::val2str(vals[c]).size() + 1 << " x i8]* @.str" << strc << ", i32 0, i32 0))\n";
                }
                break;
              case EXIT:
                cout << "call void @EXIT_task()\n";
              case PRINT:
                cout << "call void @PRINT_task()\n";
                break;
              case ADD:
                cout << "call void @ADD_task()\n";
                break;
              case SUB:
                cout << "call void @SUB_task()\n";
                break;
              case MUL:
                cout << "call void @MUL_task()\n";
                break;
              case DIV:
                cout << "call void @DIV_task()\n";
                break;
              case MOD:
                cout << "call void @MOD_task()\n";
                break;
              case DLCALL:
                cout << "call void @DLCALL_task()\n";
                break;
              case REC:
                rec++;
                break;
              case RUN:
                cout << "call void @RUN_task()\n";
                break;
              case POP:
                cout << "call void @POP_task()\n";
                break;
              case LOGSTCK:
                cout << "call void @LOGSTCK_task()\n";
                break;
              case PRINTLN:
                cout << "call void @PRINTLN_task()\n";
                break;
              case REPEAT:
                cout << "call void @REPEAT_task()\n";
                break;
              case EQ:
                cout << "call void @EQ_task()\n";
                break;
              case FEQ:
                cout << "call void @FEQ_task()\n";
                break;
              case GT:
                cout << "call void @GT_task()\n";
                break;
              case GE:
                cout << "call void @GE_task()\n";
                break;
              case LT:
                cout << "call void @LT_task()\n";
                break;
              case LE:
                cout << "call void @LE_task()\n";
                break;
              case LAND:
                cout << "call void @LAND_task()\n";
                break;
              case LOR:
                cout << "call void @LOR_task()\n";
                break;
              case AND:
                cout << "call void @AND_task()\n";
                break;
              case OR:
                cout << "call void @OR_task()\n";
                break;
              case NOT:
                cout << "call void @NOT_task()\n";
                break;
              case LNOT:
                cout << "call void @LNOT_task()\n";
                break;
              case LSHIFT:
                cout << "call void @LSHIFT_task()\n";
                break;
              case RSHIFT:
                cout << "call void @RSHIFT_task()\n";
                break;
              case XOR:
                cout << "call void @XOR_task()\n";
                break;
              case NEG:
                cout << "call void @NEG_task()\n";
                break;
              case WTRUN:
                cout << "call void @WTRUN_task()\n";
                break;
              case WFRUN:
                cout << "call void @WFRUN_task()\n";
                break;
              case IFTRUN:
                cout << "call void @IFTRUN_task()\n";
              case IFFRUN:
                cout << "call void @IFFRUN_task()\n";
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
