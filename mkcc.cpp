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
  cout << "#include \"VM_functions.h\"\n";
  vector<value> vals;
  Record r;
  bool wait = false;
  int rec = 0;
  int recsize = 0;
  cout << "int main(int argc, char** argv){\n";
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
                cout << "PUT_taski(" << recsize << ");\n";
                recsize = 0;
                c--;
                continue;
              }
            }
            recsize++;
            cout << "PUT_taski(" << VM::val2str(vals[c]) << ");\n";
            if(std::get<double>(vals[c]) == PUT){
              c++;
              recsize++;
              if(VM::getValType(vals[c]) == TYPE_NUM){
                cout << "PUT_taski(" << VM::val2str(vals[c]) << ");\n";
              } else {
                cout << "PUT_tasks(\"" << VM::val2str(vals[c]) << "\");\n";
              }
            } else if(std::get<double>(vals[c]) == REC){
              rec++;
            }
          } else {
            cout << "PUT_tasks(\"" << VM::val2str(vals[c]) << "\");\n";
          }
        } else if (VM::getValType(v) == TYPE_NUM) {
            switch ((int)get<double>(v)) {
              case PUT:
                c++;
                if(VM::getValType(vals[c]) == TYPE_NUM){
                  cout << "PUT_taski(" << VM::val2str(vals[c]) << ");\n";
                } else {
                  cout << "PUT_tasks(\"" << VM::val2str(vals[c]) << "\");\n";
                }
                break;
              case EXIT:
                cout << "EXIT_task();\n";
              case PRINT:
                cout << "PRINT_task();\n";
                break;
              case ADD:
                cout << "ADD_task();\n";
                break;
              case SUB:
                cout << "SUB_task();\n";
                break;
              case MUL:
                cout << "MUL_task();\n";
                break;
              case DIV:
                cout << "DIV_task();\n";
                break;
              case MOD:
                cout << "MOD_task();\n";
                break;
              case DLCALL:
                cout << "DLCALL_task();\n";
                break;
              case REC:
                rec++;
                break;
              case RUN:
                cout << "RUN_task();\n";
                break;
              case POP:
                cout << "POP_task();\n";
                break;
              case LOGSTCK:
                cout << "LOGSTCK_task();\n";
                break;
              case PRINTLN:
                cout << "PRINTLN_task();\n";
                break;
              case REPEAT:
                cout << "REPEAT_task();\n";
                break;
              case EQ:
                cout << "EQ_task();\n";
                break;
              case FEQ:
                cout << "FEQ_task();\n";
                break;
              case GT:
                cout << "GT_task();\n";
                break;
              case GE:
                cout << "GE_task();\n";
                break;
              case LT:
                cout << "LT_task();\n";
                break;
              case LE:
                cout << "LE_task();\n";
                break;
              case LAND:
                cout << "LAND_task();\n";
                break;
              case LOR:
                cout << "LOR_task();\n";
                break;
              case AND:
                cout << "AND_task();\n";
                break;
              case OR:
                cout << "OR_task();\n";
                break;
              case NOT:
                cout << "NOT_task();\n";
                break;
              case LNOT:
                cout << "LNOT_task();\n";
                break;
              case LSHIFT:
                cout << "LSHIFT_task();\n";
                break;
              case RSHIFT:
                cout << "RSHIFT_task();\n";
                break;
              case XOR:
                cout << "XOR_task();\n";
                break;
              case NEG:
                cout << "NEG_task();\n";
                break;
              case WTRUN:
                cout << "WTRUN_task();\n";
                break;
              case WFRUN:
                cout << "WFRUN_task();\n";
                break;
              case IFTRUN:
                cout << "IFTRUN_task();\n";
              case IFFRUN:
                cout << "IFFRUN_task();\n";
                break;
              case THREAD:
                cout << "THREAD_task();\n";
                break;
              case MEMSET:
                cout << "MEMSET_task();\n";
                break;
              case MEMGET:
                cout << "MEMGET_task();\n";
                break;
              case MEMSIZE:
                cout << "MEMSIZE_task();\n";
                break;
              case MEMPUT:
                cout << "MEMPUT_task();\n";
                break;
              case MEMINS:
                cout << "MEMINS_task();\n";
                break;
              case MEMDEL:
                cout << "MEMDEL_task();\n";
                break;
            }
          }
        }
      vals.clear();
    }
  }
  cout << "return 0;\n";
  cout << "}\n";
  f.close();
	return 0;
}
