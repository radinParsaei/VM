#include <random.h>
#include <time.h>

#ifdef USE_GMP_LIB

bool init = true;
gmp_randclass gmp_random(gmp_randinit_default);

LIB_FUNCTION(random) {
  if (init) {
    gmp_random.seed(time(NULL));
    init = false;
  }
  vm->push(Value(gmp_random.get_f(1024)));
}

LIB_FUNCTION(seed) {
  Value seedValue = vm->pop();
  gmp_random.seed(seedValue.getNumber().get_d());
}

#endif
