#include <VM_JNI.h>

JNIEXPORT void JNICALL Java_VM_run (JNIEnv *, jobject, jbyte opcode) {
  vm.run1((int)opcode);
}

JNIEXPORT void JNICALL Java_VM_runWithString (JNIEnv* env, jobject, jbyte opcode, jstring str) {
  vm.run1((char)opcode, env->GetStringUTFChars(str, new jboolean(1)));
}

JNIEXPORT void JNICALL Java_VM_runWithDouble (JNIEnv *, jobject, jbyte opcode, jdouble data) {
  vm.run1((char)opcode, (double)data);
}

JNIEXPORT void JNICALL Java_VM_init (JNIEnv *, jobject) {
  vm.attachMem(mem);
}
