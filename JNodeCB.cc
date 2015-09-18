#include "node.h"

#include <jni.h>

// XXX TEMP debugging via stdout:
#include <iostream>

// Made with *some* help from:
// - https://nodejs.org/api/addons.html
// - http://bespin.cz/~ondras/html/classv8_1_1Value.html
// - http://www.codeproject.com/Articles/993067/Calling-Java-from-Cplusplus-with-JNI

// FUTURE TBD: put this in a namespace??

/* exported by JNode.cc */
extern JNIEnv * jnode_jni_env;

void callVoidMethodWithNoParameters(const v8::FunctionCallbackInfo<v8::Value> & fci) {
  v8::Isolate * isolate = fci.GetIsolate();

  // XXX TEMP debugging via stdout:
  std::cout << "C++ got callVoidMethodWithNoParameters" << std::endl;

  if (fci.Length() < 2) {
    std::cerr << "JNodeCB.callVoidMethodWithNoParameters() called with missing parameter(s)" << std::endl;
    return;
  }

  if (!fci[0]->IsString() || !fci[1]->IsString()) {
    std::cerr << "JNodeCB.callVoidMethodWithNoParameters() called with incorrect parameter(s)" << std::endl;
    return;
  }

  if (fci.Length() > 2) {
    std::cerr << "JNodeCB.callVoidMethodWithNoParameters() called with extra parameter(s) IGNORED" << std::endl;
  }

  // ref: http://stackoverflow.com/a/10255816/1283667
  v8::String::Utf8Value v1(fci[0]->ToString());
  v8::String::Utf8Value v2(fci[1]->ToString());

  std::cout << "first arg: " << *v1 << std::endl;
  std::cout << "2nd arg: " << *v2 << std::endl;

  // XXX TODO these should be cached somehow
  JNIEnv * e = jnode_jni_env;
  jclass c = e->FindClass(*v1);
  if (c == nullptr) {
    std::cerr << "Could not find class: " << *v1 << std::endl;
    return;
  }

  jmethodID m = e->GetStaticMethodID(c, "callVoidTestMethodWithNoParameters", "()V");
  if (m == 0) {
    std::cerr << "Could not find method: " << *v2 << std::endl;
    return;
  }

  e->CallStaticVoidMethod(c, m);
}

void initJNodeCB(v8::Local<v8::Object> exports) {
  NODE_SET_METHOD(exports, "callVoidMethodWithNoParameters", callVoidMethodWithNoParameters);
}

NODE_MODULE(JNodeCB, initJNodeCB)

