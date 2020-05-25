#define LIB_NAME "test"
#define LIB_AUTHOR "radinParsaei"
#define LIB_VERSION "1.0_test"
#define LIB_LICENSE "PUBLIC_DOMAIN"

#include "VMLib.h"

using namespace std;

LIB_FUNCTION(test) {
  cout << stack->at(0).toString() << endl;
  stack->at(0) = 20;
}

int main() {
  vector<Value> a;
  a.push_back(10);
  test(&a);
  cout << a[0].toString() << endl;
}
