#include "node.h"

#include <jni.h>

#include <iostream>

// Made with *some* help from:
// - https://nodejs.org/api/addons.html
// - http://bespin.cz/~ondras/html/classv8_1_1Value.html
// - http://www.codeproject.com/Articles/993067/Calling-Java-from-Cplusplus-with-JNI

// FUTURE TBD: put this in a namespace??

/* exported by JNode.cc */
extern JNIEnv * jnode_jni_env;

void callMethod(const v8::FunctionCallbackInfo<v8::Value> & fci) {
  v8::Isolate * isolate = fci.GetIsolate();

  // [TEMP] debugging via stdout:
  //std::cout << "C++ got callMethod() from Javascript" << std::endl;

  if (fci.Length() < 2) {
    std::cerr << "JNodeCB.callMethod() called with missing parameter(s)" << std::endl;
    return;
  }

  if (!fci[0]->IsString() || !fci[1]->IsString()) {
    std::cerr << "JNodeCB.callMethod() called with incorrect parameter(s)" << std::endl;
    return;
  }

  // ref: http://stackoverflow.com/a/10255816/1283667
  v8::String::Utf8Value v1(fci[0]->ToString());
  v8::String::Utf8Value v2(fci[1]->ToString());

  //std::cout << "first arg: " << *v1 << std::endl;
  //std::cout << "2nd arg: " << *v2 << std::endl;

  // XXX TODO these should be cached somehow [by using a wrapped C++ object]
  JNIEnv * e = jnode_jni_env;
  jclass c = e->FindClass(*v1);
  if (c == nullptr) {
    std::cerr << "Could not find class: " << *v1 << std::endl;
    return;
  }

  jmethodID m = e->GetStaticMethodID(c, *v2, "(J)V");
  if (m == 0) {
    std::cerr << "Could not find method: " << *v2 << std::endl;
    return;
  }

  long long fciHandle = (long long)&fci;

  e->CallStaticVoidMethod(c, m, fciHandle);
}

void initJNodeCB(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "callMethod", callMethod);
}

NODE_MODULE(JNodeCB, initJNodeCB)

