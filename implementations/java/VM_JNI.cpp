#include <VM_JNI.h>

JNIEXPORT void JNICALL Java_VM_run (JNIEnv *, jobject, jbyte opcode) {
  vm.run1((int)opcode);
}

JNIEXPORT void JNICALL Java_VM_runWithString (JNIEnv* env, jobject, jbyte opcode, jstring str) {
  if (str == NULL) {
    vm.run1((char)opcode, null);
  } else {
    vm.run1((char)opcode, env->GetStringUTFChars(str, new jboolean(1)));
  }
}

JNIEXPORT void JNICALL Java_VM_runWithBoolean (JNIEnv *, jobject, jbyte opcode, jboolean boolean) {
  vm.run1((char)opcode, (bool) boolean);
}

JNIEXPORT void JNICALL Java_VM_runWithDouble (JNIEnv *, jobject, jbyte opcode, jdouble data) {
  vm.run1((char)opcode, (double)data);
}

JNIEXPORT void JNICALL Java_VM_init (JNIEnv *, jobject) {
  vm.attachMem(mem);
}
