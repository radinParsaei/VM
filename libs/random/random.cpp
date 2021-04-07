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
  vm->push(Value(gmp_random.get_f()));
}

LIB_FUNCTION(randint) {
  Value max = vm->pop();
  Value min = vm->pop();
  if (init) {
    gmp_random.seed(time(NULL));
    init = false;
  }
  vm->push(Value(mpf_class(gmp_random.get_z_range(mpz_class((max - min).toString())))) + min);
}

LIB_FUNCTION(seed) {
  Value seedValue = vm->pop();
  gmp_random.seed(seedValue.getNumber().get_d());
}

#endif
