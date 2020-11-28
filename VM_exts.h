#ifndef VM_exts_h
#define VM_exts_h
#include "VM_ext.h"
#if TARGET != Emscripten
#include "exts/ffi/VMFFI.h"
#endif
#include "exts/named-memory-and-function/named-memory-and-function.h"
#include "exts/function-parameters/function-parameters.h"

EXTENSIONS
#if TARGET != Emscripten
CALL_EXT(vmffi)
#endif
CALL_EXT(vm_named_memory_and_function)
CALL_EXT(vm_function_pointers)
END_EXTS
#endif
