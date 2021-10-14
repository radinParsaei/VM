#ifndef VM_exts_h
#define VM_exts_h
#include "VM_ext.h"
#if TARGET != Emscripten && TARGET != ARDUINO
#include "exts/ffi/VMFFI.h"
#endif
#include "exts/named-memory-and-function/named-memory-and-function.h"
#include "exts/function-parameters/function-parameters.h"
#include "exts/class-to-string-support/class-to-string-support.h"

EXTENSIONS
#if TARGET != Emscripten && TARGET != ARDUINO
CALL_EXT(vmffi)
#endif
CALL_EXT(vm_named_memory_and_function)
CALL_EXT(vm_function_pointers)
CALL_EXT(class_to_string_support)
END_EXTS
#endif
