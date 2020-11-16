//VMFFI
#if TARGET != Emscripten
#include <ffi.h>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <stdint.h>
#include <sstream>

inline char* stringDuplicate(const char* data) {
		size_t len = strlen(data) + 1;
		void *dup = malloc(len);
		return (char*)memcpy(dup, data, len);
	}

inline bool VM_ext_run_vmffi(char opcode, Value arg, VM* vm) {
  if (opcode == DLCALL) {
    Value tmp = vm->getStack()[vm->getStack().size() - 1];
    if (tmp.startsWith("ffi:")) {
      vm->pop();
      Value tmp2 = vm->pop();
#ifndef DYNAMIC_LIBS_NOT_AVAILABLE
      void* libhandle = dlopen(tmp.substring(4).getString().c_str(), RTLD_LAZY);
      if (!libhandle) {
        fprintf(stderr, "dlopen error: %s\n", dlerror());
        return true;
      }
      Value tmp3 = tmp2;
      void* fn = dlsym(libhandle, tmp2.substring(0, tmp2.find(":")).getString().c_str());
      char* err = dlerror();
      if (err || !fn) {
        fprintf(stderr, "dlsym failed: %s\n", err);
        return true;
      }

      void *values[128];
      ffi_type *args[128];
			bool isPointer[128];
      Value args2 = tmp3.substring(tmp3.find(":") + 1);
      int i = 0;
      while (args2 != "") {
        if (args2.startsWith("n")) {
          args[i] = &ffi_type_pointer;
          void** n = new void*;
					*n = NULL;
          values[i] = n;
          args2 = args2.substring(1);
					isPointer[i] = true;
        } else if (args2.startsWith("t")) {
          args[i] = &ffi_type_pointer;
					char** data = new char*;
					*data = stringDuplicate(vm->pop().toString().c_str());
          values[i] = data;
          args2 = args2.substring(1);
					isPointer[i] = true;
        } else if (args2.startsWith("f")) {
          float *data = new float;
					*data = vm->pop().getDouble();
          args[i] = &ffi_type_float;
          values[i] = data;
          args2 = args2.substring(1);
					isPointer[i] = false;
        } else if (args2.startsWith("d")) {
          double *data = new double;
					*data = vm->pop().getDouble();
          args[i] = &ffi_type_double;
          values[i] = data;
          args2 = args2.substring(1);
					isPointer[i] = false;
        } else if (args2.startsWith("u8")) {
          uint8_t *data = new uint8_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_uint8;
          values[i] = data;
          args2 = args2.substring(2);
					isPointer[i] = false;
        } else if (args2.startsWith("s8")) {
          int8_t *data = new int8_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_sint8;
          values[i] = data;
          args2 = args2.substring(2);
					isPointer[i] = false;
        } else if (args2.startsWith("u16")) {
          uint16_t *data = new uint16_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_uint16;
          values[i] = data;
          args2 = args2.substring(3);
					isPointer[i] = false;
        } else if (args2.startsWith("s16")) {
          int16_t *data = new int16_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_sint16;
          values[i] = data;
          args2 = args2.substring(3);
					isPointer[i] = false;
        } else if (args2.startsWith("u32")) {
          uint32_t *data = new uint32_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_uint32;
          values[i] = data;
          args2 = args2.substring(3);
					isPointer[i] = false;
        } else if (args2.startsWith("s32")) {
          int32_t *data = new int32_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_sint32;
          values[i] = data;
          args2 = args2.substring(3);
					isPointer[i] = false;
        } else if (args2.startsWith("u64")) {
          uint64_t *data = new uint64_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_uint64;
          values[i] = data;
          args2 = args2.substring(3);
					isPointer[i] = false;
        } else if (args2.startsWith("s64")) {
          int64_t *data = new int64_t;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_sint64;
          values[i] = data;
          args2 = args2.substring(3);
					isPointer[i] = false;
        } else if (args2.startsWith("us")) {
          signed short *data = new signed short;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_ushort;
          values[i] = data;
          args2 = args2.substring(2);
					isPointer[i] = false;
        } else if (args2.startsWith("ss")) {
          unsigned short *data = new unsigned short;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_sshort;
          values[i] = data;
          args2 = args2.substring(2);
					isPointer[i] = false;
        } else if (args2.startsWith("ui")) {
          unsigned int *data = new unsigned int;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_uint;
          values[i] = data;
          args2 = args2.substring(2);
					isPointer[i] = false;
        } else if (args2.startsWith("si")) {
          int *data = new int;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_sint;
          values[i] = data;
          args2 = args2.substring(2);
					isPointer[i] = false;
        } else if (args2.startsWith("l")) {
          long *data = new long;
					*data = vm->pop().getLong();
          args[i] = &ffi_type_slong;
          values[i] = data;
          args2 = args2.substring(1);
					isPointer[i] = false;
        } else {
          break;
        }
        i++;
      }
      ffi_cif cif;
			tmp3.substring(tmp3.find(":"));
			if (tmp3.startsWith(":")) {
				tmp3.substring(1);
			}

			ffi_type *retType;

			if (tmp3.startsWith("v") || tmp3.startsWith("n")) {
				retType = &ffi_type_void;
			} else if (tmp3.startsWith("t")) {
				retType = &ffi_type_pointer;
      } else if (tmp3.startsWith("f")) {
        retType = &ffi_type_float;
      } else if (tmp3.startsWith("d")) {
				retType = &ffi_type_double;
      } else if (tmp3.startsWith("u8")) {
        retType = &ffi_type_uint8;
      } else if (tmp3.startsWith("s8")) {
				retType = &ffi_type_sint8;
      } else if (tmp3.startsWith("u16")) {
				retType = &ffi_type_uint16;
      } else if (tmp3.startsWith("s16")) {
        retType = &ffi_type_sint16;
      } else if (tmp3.startsWith("u32")) {
        retType = &ffi_type_uint32;
      } else if (tmp3.startsWith("s32")) {
        retType = &ffi_type_sint32;
      } else if (tmp3.startsWith("u64")) {
        retType = &ffi_type_uint64;
      } else if (tmp3.startsWith("s64")) {
        retType = &ffi_type_sint64;
      } else if (tmp3.startsWith("us")) {
        retType = &ffi_type_ushort;
      } else if (tmp3.startsWith("ss")) {
        retType = &ffi_type_sshort;
      } else if (tmp3.startsWith("ui")) {
        retType = &ffi_type_uint;
      } else if (tmp3.startsWith("si")) {
        retType = &ffi_type_sint;
      } else if (tmp3.startsWith("l")) {
	      retType = &ffi_type_slong;
      }

      ffi_status status = ffi_prep_cif(&cif, FFI_DEFAULT_ABI, i, retType, args);
      if (status != FFI_OK) {
          fprintf(stderr, "ffi_prep_cif failed: %d\n", status);
          return true;
      }

      void* tmp = new void*;
      ffi_call(&cif, FFI_FN(fn), tmp, values);
			i--;
			for (; i >= 0; i--) {
				if (isPointer[i]) {
					free(*(void**)values[i]);
				}
				delete values[i];
			}

			if (!(tmp3.startsWith("v") || tmp3.startsWith("n"))) {
				if (tmp3.startsWith("t")) {
					vm->push(*(char**)tmp);
        } else if (tmp3.startsWith("f")) {
          vm->push(*(float*)tmp);
        } else if (tmp3.startsWith("d")) {
					vm->push(*(double*)tmp);
        } else if (tmp3.startsWith("u8")) {
					vm->push(*(uint8_t*)tmp);
        } else if (tmp3.startsWith("s8")) {
					vm->push(*(int8_t*)tmp);
        } else if (tmp3.startsWith("u16")) {
					vm->push(*(uint16_t*)tmp);
        } else if (tmp3.startsWith("s16")) {
					vm->push(*(int16_t*)tmp);
        } else if (tmp3.startsWith("s32")) {
					vm->push(*(int32_t*)tmp);
        } else if (tmp3.startsWith("u64")) {
					uint64_t data = *(uint64_t*)tmp;
					std::ostringstream dataStream;
					dataStream << data;
					Value v = dataStream.str();
					vm->push(v.toNum());
        } else if (tmp3.startsWith("s64")) {
					vm->push(*(int64_t*)tmp);
        } else if (tmp3.startsWith("us")) {
					vm->push(*(unsigned short*)tmp);
        } else if (tmp3.startsWith("ss")) {
					vm->push(*(signed short*)tmp);
        } else if (tmp3.startsWith("si")) {
					vm->push(*(int*)tmp);
        } else if (tmp3.startsWith("l") || tmp3.startsWith("ui") || tmp3.startsWith("u32")) {
					vm->push(*(long*)tmp);
        }
			}
#endif
      return true;
    }
  }
  return false;
}
#else
#warning VMFFI not available web
#endif
