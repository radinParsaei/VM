#ifndef VM_BINARIES_H
#define VM_BINARIES_H

#include "VM.h"

namespace VM_BINARIES {
  std::vector<Value> parseBin(std::string data) {
    std::vector<Value> program;
    for (size_t i = 0; i < data.size(); i++) {
      program.push_back((int)data[i]);
      if (data[i] == PUT) {
        i++;
        if (data[i] == 0) {
          program.push_back(null);
        } else if (data[i] == 1) {
          program.push_back(True);
        } else if (data[i] == 2) {
          program.push_back(False);
        } else if ((data[i] & 0b00000111) == 3 || (data[i] & 0b00000111) == 4) { //small numbers
          bool isneg = (data[i] & 0b00000111) - 3;
          NUMBER n = (data[i] & 0b11111000) >> 3;
          while (data[++i] != 0) {
            n += (unsigned char)data[i];
          }
          if (isneg) n = -n;
          program.push_back(n);
        } else if (data[i] == 5) {
          std::string s;
          while (true) {
            i++;
            if (data[i - 1] != 0 && data[i] == 0 && data[i + 1] != 0) break;
            if (!data[i])i++;
            s += data[i];
          }
          program.push_back(s);
        } else if (data[i] == 6) {
          std::string s;
          while (data[++i] != 0) {
            s += data[i];
          }
          program.push_back(NUMBER(s));
        }
      }
    }
    return program;
  }

  void runBinFromStream(std::ifstream& in, VM vm) {
    std::vector<Value> program;
    char data = 0;
    bool dataAvailable = (bool)(bool)in.get(data);
    while (dataAvailable) {
      program.push_back((int)data);
      if (data == PUT) {
        if (!in.get(data)) break;
        if (data == 0) {
          program.push_back(null);
        } else if (data == 1) {
          program.push_back(True);
        } else if (data == 2) {
          program.push_back(False);
        } else if ((data & 0b00000111) == 3 || (data & 0b00000111) == 4) { //small numbers
          bool isneg = (data & 0b00000111) - 3;
          NUMBER n = (data & 0b11111000) >> 3;
          while (data != 0) {
            if (!in.get(data)) break;
            n += (unsigned char)data;
          }
          if (isneg) n = -n;
          program.push_back(n);
        } else if (data == 5) {
          std::string s;
          while (true) {
            if (!in.get(data)) break;
            if (!data) {
              if (!in.get(data)) break;
              if (data) break;
            }
            s += data;
          }
          program.push_back(s);
          vm.run(program);
          program.clear();
          continue;
        } else if (data == 6) {
          std::string s;
          while (data != 0) {
            if (!in.get(data)) break;
            s += data;
          }
          program.push_back(NUMBER(s));
        }
      }
      vm.run(program);
      program.clear();
      dataAvailable = (bool)in.get(data);
    }
  }

  std::string mkBin(std::vector<Value> program) {
    std::string res;
    bool PUT_DATA = false;
    for (Value i : program) {
      if (PUT_DATA) {
        PUT_DATA = false;
        switch (i.getType()) {
          case null:
            res += (char)0;
            continue;
          case True:
            res += (char)1;
            continue;
          case False:
            res += (char)2;
            continue;
          case VALUE_TYPE_NUMBER: {
            NUMBER j = i.getNumber();
            if (j < 1000) {
              char mode;
              if (j >= 0) {
                mode = 3;
              } else {
                mode = 4;
                j = -j;
              }
              if (j >= 31) {
                mode |= 31 << 3;
                j -= 31;
              } else {
  #ifdef USE_GMP_LIB
                mode |= j.get_si() << 3;
  #else
                mode |= j.getLong() << 3;
  #endif
                j = 0;
              }
              res += mode;
              for (; j > 0; j -= 255) {
                if (j >= 255) {
                  res += (char)255;
                } else {
    #ifdef USE_GMP_LIB
                  res += (char)j.get_si();
    #else
                  res += (char)j.toLong();
    #endif
                }
              }
              res += (char)0;
            } else {
              res += (char)6;
              for (char k : i.toString()) {
                res += k;
              }
              res += (char)0;
            }
            continue;
          }
          case VALUE_TYPE_TEXT:
            res += (char)5;
            for (char j : i.getString()) {
              if (j == 0) res += j;
              res += j;
            }
            res += (char)0;
            continue;
        }
      }
      if (i.getLong() == PUT) {
        PUT_DATA = true;
      }
      res += (char)i.getLong();
    }
    return res;
  }
}
#endif
