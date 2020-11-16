#ifndef STATIC_LIBCALL
#define STATIC_LIBCALL
#include <VM.h>
#include "libs/test/test.h"

bool _dlcall(VM *vm) {
	if (vm->getStack()[vm->getStack().size() - 1].toString() == "./test.vmso") {
		if (vm->getStack()[vm->getStack().size() - 2].toString() == "test") {
			vm->pop(); vm->pop();
			test_1_1_test_test(vm);
			return true;
		}
	}
	return false;
}
#endif
