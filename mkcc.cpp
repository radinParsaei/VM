#include "VM.h"
#include <fstream>
#include <sstream>
#include "VM_binaries.h"

using namespace std;
using namespace VM_BINARIES;

int main(int argc, char const *argv[]){
  if(argc < 2){
    cerr << "usage: " << argv[0] << " <binary file> [<libraries config file>] [<includes>]" << endl;
    return 1;
  }
  ifstream f(argv[1], ios::binary);
  if(!f){
    cerr << "can\'t open file" << '\n';
    return 1;
  }
  cout << "#include \"VM.h\"\n";
  if (argc > 3) {
    ifstream f(argv[3]);
    if(!f){
      cerr << "can\'t open file" << '\n';
      return 1;
    }
    string line;
    while (getline(f, line)) {
      if (line.find("\"") == 0 || line.find("<") == 0) {
        cout << "#include " << line << endl;
      } else if (line.find("#") == 0) {
        cout << line << endl;
      } else {
        cout << "#include \"" << line << "\"\n";
      }
    }
  }
  if (argc > 2) {
    cout << "void _dlcall(VM vm) {\n";
    ifstream fconf(argv[2]);
    if (!fconf) {
      cerr << "can\'t open file" << '\n';
      return 1;
    }
    string line;
    while (getline(fconf, line)) {
      cout << "\tif (vm.getStack()[vm.getStack().size() - 1].toString() == \"" << Value(line.substr(0, line.find("|"))).trim() << "\") {\n";
      string fnName = line.substr(line.find("|") + 1);
      fnName = fnName.substr(0, fnName.find("|"));
      string libVer = line.substr(line.find("|") + 1 + fnName.size());
      libVer = libVer.substr(1);
      while (getline(fconf, line)) {
        if (Value(line).find(" ") != 0 && Value(line).find("\t") != 0) break;
        cout << "\t\tif (vm.getStack()[vm.getStack().size() - 2].toString() == \"" << Value(fnName).trim() << "\") {\n";
        cout << "\t\t\t" << Value(line).trim() << '_' << Value(libVer.c_str()).trim() << '_' << Value(line.c_str()).trim() << "(&vm);\n";
        cout << "\t\t\tvm.getStack().pop_back(); vm.getStack().pop_back(); return;\n\t\t}\n";
      }
      cout << "\t}\n";
    }
    cout << "\tvm.run1(DLCALL);\n";
    cout << "}\n";
  }
  cout << "int main(int argc, char** argv){\n";
  cout << "\tVM vm;\n\tstd::vector<Value>* mem = new std::vector<Value>();\n\tvm.attachMem(mem);\n";
  vector<Value> vals = parseBin(string((istreambuf_iterator<char>(f)), istreambuf_iterator<char>()));
  for (int c = 0; c < vals.size(); c++) {
    Value v = vals[c];
    if (v.getType() == VALUE_TYPE_NUMBER) {
      if (v.getLong() == DLCALL && (argc > 2)) {
        cout << "\t_dlcall(vm);" << endl;
      } else {
        cout << "\tvm.run1(" << v.toString();
        if (v.getLong() == PUT) {
          c++;
          cout << ", " << (vals[c].getType()? "\"":"") << vals[c].toString() << (vals[c].getType()? "\"":"");
        }
        cout << ");\n";
      }
    }
  }
  cout << "\treturn 0;\n";
  cout << "}\n";
  f.close();
	return 0;
}
