#include <jni.h>
#include <VM.h>

#ifndef _Included_VM
#define _Included_VM
VM vm;
std::vector<Value>* mem = new std::vector<Value>();
#ifdef __cplusplus
extern "C" {
#endif
JNIEXPORT void JNICALL Java_VM_runWithString
  (JNIEnv *, jobject, jbyte, jstring);

JNIEXPORT void JNICALL Java_VM_runWithDouble
  (JNIEnv *, jobject, jbyte, jdouble);

JNIEXPORT void JNICALL Java_VM_runWithBoolean
  (JNIEnv *, jobject, jbyte, jboolean);

JNIEXPORT void JNICALL Java_VM_run
  (JNIEnv *, jobject, jbyte);

JNIEXPORT void JNICALL Java_VM_init
  (JNIEnv *, jobject);

#ifdef __cplusplus
}
#endif
#endif
