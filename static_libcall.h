#ifndef STATIC_LIBCALL
#define STATIC_LIBCALL
#include <VM.h>
#include "libs/random/random.h"
#include "libs/input/input.h"

bool _dlcall(VM *vm) {
	if (vm->getStack()[vm->getStack().size() - 1].toString() == "random") {
		if (vm->getStack()[vm->getStack().size() - 2].toString() == "random_") {
			vm->pop(); vm->pop();
			random_1_0_random_(vm);
			return true;
		}
		if (vm->getStack()[vm->getStack().size() - 2].toString() == "seed") {
			vm->pop(); vm->pop();
			random_1_0_seed(vm);
			return true;
		}
		if (vm->getStack()[vm->getStack().size() - 2].toString() == "randint") {
			vm->pop(); vm->pop();
			random_1_0_randint(vm);
			return true;
		}
	}
	if (vm->getStack()[vm->getStack().size() - 1].toString() == "input") {
		if (vm->getStack()[vm->getStack().size() - 2].toString() == "input") {
			vm->pop(); vm->pop();
			input_1_0_input(vm);
			return true;
		}
	}
	return false;
}
#endif
