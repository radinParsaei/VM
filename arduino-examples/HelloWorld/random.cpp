#include "random.h"
#include <time.h>

static bool _init = true;

#ifdef USE_GMP_LIB

gmp_randclass gmp_random(gmp_randinit_default);

LIB_FUNCTION(random_) {
  if (_init) {
    gmp_random.seed(time(NULL));
    _init = false;
  }
  vm->push(Value(gmp_random.get_f()));
}

LIB_FUNCTION(randint) {
  Value max = vm->pop();
  Value min = vm->pop();
  if (_init) {
    gmp_random.seed(time(NULL));
    _init = false;
  }
  vm->push(Value(mpf_class(gmp_random.get_z_range(mpz_class((max - min).toString())))) + min);
}

LIB_FUNCTION(seed) {
  Value seedValue = vm->pop();
  gmp_random.seed(seedValue.getNumber().get_d());
}

#else

LIB_FUNCTION(random_) {
  if (_init) {
    srand(time(NULL));
    _init = false;
  }
  char s[] = "0.0000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
  for (int i = 0; i < 310; i++) {
    s[2 + i] = (rand() % 10) + '0';
  }
  Value num = s;
  vm->push(num.toNum());
}

LIB_FUNCTION(randint) {
  Value max = vm->pop();
  Value min = vm->pop();
  if (init) {
    srand(time(NULL));
    _init = false;
  }
  Value gap = max - min;
  TEXT s = "";
  for (int i = 0; i <= gap.toString().length(); i++) {
    s += (rand() % 10) + '0';
  }
  vm->push((Value(s).toNum() % gap) + min);
}

LIB_FUNCTION(seed) {
  Value seedValue = vm->pop();
  srand(seedValue.getLong());
}

#endif
