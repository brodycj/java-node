#include "node.h"
#include "node_object_wrap.h"

#include <jni.h>

#include <iostream>

// Made with *some* help from:
// - https://nodejs.org/api/addons.html
// - http://bespin.cz/~ondras/html/classv8_1_1Value.html
// - http://www.codeproject.com/Articles/993067/Calling-Java-from-Cplusplus-with-JNI

// FUTURE TBD: put this in a namespace??

/* exported by JNode.cc */
extern JNIEnv * jnode_jni_env;

class MethodCallObject : public node::ObjectWrap {
public:
  static void init(v8::Isolate *);
  static void newInstance(const v8::FunctionCallbackInfo<v8::Value> &);

private:
  MethodCallObject(JNIEnv *e, jclass c, jmethodID m) : e(e), c(c), m(m) { }
  ~MethodCallObject() { }

  static void _new(const v8::FunctionCallbackInfo<v8::Value> &);
  static void call(const v8::FunctionCallbackInfo<v8::Value> &);

  static v8::Persistent<v8::Function> ctor;

  JNIEnv * e;
  jclass c;
  jmethodID m;
};

v8::Persistent<v8::Function> MethodCallObject::ctor;

void MethodCallObject::init(v8::Isolate * isolate) {
  v8::Local<v8::FunctionTemplate> ft = v8::FunctionTemplate::New(isolate, _new);
  ft->SetClassName(v8::String::NewFromUtf8(isolate, "MethodCallObject"));
  ft->InstanceTemplate()->SetInternalFieldCount(1);

  NODE_SET_PROTOTYPE_METHOD(ft, "call", call);

  ctor.Reset(isolate, ft->GetFunction());
}

void MethodCallObject::_new(const v8::FunctionCallbackInfo<v8::Value> & fci) {
  // not expected to be false:
  if (fci.IsConstructCall()) {
    // ref: http://stackoverflow.com/a/10255816/1283667
    v8::String::Utf8Value v1(fci[0]->ToString());
    v8::String::Utf8Value v2(fci[1]->ToString());

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

    MethodCallObject * o = new MethodCallObject(e, c, m);
    o->Wrap(fci.This());

    fci.GetReturnValue().Set(fci.This());
  }
}

void MethodCallObject::newInstance(const v8::FunctionCallbackInfo<v8::Value> & fci) {
  v8::Isolate * isolate = fci.GetIsolate();

  // XXX TODO: if class or static function does not really exist,
  // should throw exception and stop!

  v8::Handle<v8::Value> args[2] = { fci[0], fci[1] };

  v8::Local<v8::Function> myctor = v8::Local<v8::Function>::New(isolate, ctor);
  v8::Local<v8::Object> inst = myctor->NewInstance(2, args);

  fci.GetReturnValue().Set(inst);
}

void MethodCallObject::call(const v8::FunctionCallbackInfo<v8::Value> & fci) {
  MethodCallObject * o = ObjectWrap::Unwrap<MethodCallObject>(fci.Holder());
  //std::cout <<  "call() called" << std::endl;

  long long fciHandle = (long long)&fci;

  o->e->CallStaticVoidMethod(o->c, o->m, fciHandle);
}

void getStaticMethodObject(const v8::FunctionCallbackInfo<v8::Value> & fci) {
  if (fci.Length() < 2) {
    std::cerr << "JNodeCB.getStaticMethodObject() called with missing parameter(s)" << std::endl;
    // XXX TODO should throw here!
    return;
  }

  if (!fci[0]->IsString() || !fci[1]->IsString()) {
    std::cerr << "JNodeCB.getStaticMethodObject() called with incorrect parameter(s)" << std::endl;
    return;
  }

  MethodCallObject::newInstance(fci);
}

void initJNodeCB(v8::Local<v8::Object> exports) {
  MethodCallObject::init(exports->GetIsolate());
  NODE_SET_METHOD(exports, "getStaticMethodObject", getStaticMethodObject);
}

NODE_MODULE(JNodeCB, initJNodeCB)

