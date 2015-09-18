#include "node.h"

#include <jni.h>

// XXX TEMP debugging via stdout:
#include <iostream>

// Made with *some* help from:
// - https://nodejs.org/api/addons.html
// - http://www.codeproject.com/Articles/993067/Calling-Java-from-Cplusplus-with-JNI

// FUTURE TBD: put this in a namespace??

/* exported by JNode.cc */
extern JNIEnv * jnode_jni_env;

void callVoidTestMethodWithNoParameters(const v8::FunctionCallbackInfo<v8::Value> &) {
  // XXX TEMP debugging via stdout:
  std::cout << "C++ got callVoidTestMethodWithNoParameters" << std::endl;

  JNIEnv * e = jnode_jni_env;
  jclass c = e->FindClass("JNodeTestCB");
  if (c == nullptr) return;

  jmethodID m = e->GetStaticMethodID(c, "callVoidTestMethodWithNoParameters", "()V");
  if (m == 0) return;

  e->CallStaticVoidMethod(c, m);
}

void initJNodeCB(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "callVoidTestMethodWithNoParameters", callVoidTestMethodWithNoParameters);
}

NODE_MODULE(JNodeCB, initJNodeCB)

