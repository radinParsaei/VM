#define LIB_NAME "test"
#define LIB_AUTHOR "radinParsaei"
#define LIB_VERSION "1.0_test"
#define LIB_LICENSE "PUBLIC_DOMAIN"

#include "VMLib.h"

using namespace std;

LIB_FUNCTION(test) {
  cout << "test(): Hello World" << endl;
  return stack;
}
