#include "VM.h"
#include <fstream>
#include <sstream>
#include <limits.h>
#include "VM_binaries.h"

using namespace std;

int main(int argc, char const *argv[]){
  if (argc > 1) {
    cout << "#ifndef STATIC_LIBCALL\n#define STATIC_LIBCALL\n#include <VM.h>\n";
    if (argc > 2) {
      ifstream f(argv[2]);
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
    cout << "\nbool _dlcall(VM *vm) {\n";
    ifstream fconf(argv[1]);
    if (!fconf) {
      cerr << "can\'t open file" << '\n';
      return 1;
    }
    string line;
    while (getline(fconf, line)) {
      Value name = Value(line.substr(0, line.find("|"))).trim();
      cout << "\tif (vm->getStack()[vm->getStack().size() - 1].toString() == \"" << name << "\") {\n";
      // string fnName = line.substr(line.find("|") + 1);
      // fnName = fnName.substr(0, fnName.find("|"));
      string libVer = line.substr(line.find("|")/* + 1 + fnName.size()*/);
      libVer = libVer.substr(1);
      while (getline(fconf, line)) {
        if (Value(line).find(" ") != 0 && Value(line).find("\t") != 0) break;
        cout << "\t\tif (vm->getStack()[vm->getStack().size() - 2].toString() == \"" << Value(line.c_str()).trim() << "\") {\n";
        cout << "\t\t\tvm->pop(); vm->pop();\n\t\t\t" << name.trim() << '_' << Value(libVer.c_str()).trim() << '_' << Value(line.c_str()).trim() << "(vm);\n";
        cout << "\t\t\treturn true;\n\t\t}\n";
      }
      cout << "\t}\n";
    }
    cout << "\treturn false;\n}\n#endif\n";
  } else {
    cerr << "Usage: " << argv[0] << " <config file>\n";
  }
}
