#ifndef VM_exts_h
#define VM_exts_h
#include "VM_ext.h"
#include "exts/ffi/VMFFI.h"
#include "exts/named-memory-and-function/named-memory-and-function.h"

EXTENSIONS
CALL_EXT(vmffi)
CALL_EXT(vm_named_memory_and_function)
END_EXTS
#endif
