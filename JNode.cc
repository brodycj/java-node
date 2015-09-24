#include "JNode.h"
#include "node.h"

#include <string>
#include <vector>

// Made with *some* help from:
// - https://nodejs.org/api/addons.html

/* exported for use by JNodeCB addon: */
// XXX FUTURE TBD find a smarter way to export this:
JNIEnv * jnode_jni_env;

struct PersistentFunctionStruct {
  PersistentFunctionStruct(v8::Isolate * isolate, v8::Handle<v8::Function> & f) :
    isolate(isolate), f(isolate, f) { }

  v8::Isolate * isolate;
  v8::Persistent<v8::Function> f;
};

/*
 * Class:     JNode
 * Method:    start
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_JNode_start
  (JNIEnv * e, jclass, jobjectArray a)
{
  std::string s;
  std::vector<int> v;

  jnode_jni_env = e;

  // http://stackoverflow.com/questions/19591873/get-an-array-of-strings-from-java-to-c-jni
  int l = e->GetArrayLength(a);
  for (int i=0; i<l; ++i) {
    v.push_back(s.length());
    jstring js = (jstring)e->GetObjectArrayElement(a, i);
    const char * cs = e->GetStringUTFChars(js, 0);
    s += cs;
    s += '\0';
    e->ReleaseStringUTFChars(js, cs);
    e->DeleteLocalRef(js);
  }

  int argc = v.size();
  const char * all_args = s.c_str();
  const char * argv[argc];
  for (int i=0; i<argc; ++i)
    argv[i] = all_args + v[i];

  node::Start(argc, (char **)argv);
}

/*
 * Class:     JNode
 * Method:    fciArgCount
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_JNode_fciArgCount
  (JNIEnv *, jclass, jlong fciHandle)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci.Length();
}

/*
 * Class:     JNode
 * Method:    fciArgIsNumber
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_JNode_fciArgIsNumber
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->IsNumber();
}

/*
 * Class:     JNode
 * Method:    fciArgIsFunction
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_JNode_fciArgIsFunction
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->IsFunction();
}

/*
 * Class:     JNode
 * Method:    fciArgNumberValue
 * Signature: (JI)D
 */
JNIEXPORT jdouble JNICALL Java_JNode_fciArgNumberValue
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->NumberValue();
}

/*
 * Class:     JNode
 * Method:    fciArgFunctionAsPersistentHandle
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_JNode_fciArgFunctionAsPersistentHandle
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;
  v8::Isolate * isolate = fci.GetIsolate();

  v8::Handle<v8::Function> f = v8::Handle<v8::Function>::Cast(fci[argIndex]);

  return reinterpret_cast<long>(new PersistentFunctionStruct(isolate, f));
}

/*
 * Class:     JNode
 * Method:    fciArgFunctionCallWithNoArguments
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_JNode_fciArgFunctionCallWithNoArguments
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  v8::Local<v8::Function> f = v8::Local<v8::Function>::Cast(fci[argIndex]);
  v8::Local<v8::Value> av[0] = {};

  f->Call(v8::Null(fci.GetIsolate()), 0, av);
}

/*
 * Class:     JNode
 * Method:    fciSetReturnNumberValue
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_JNode_fciSetReturnNumberValue
  (JNIEnv *, jclass, jlong fciHandle, jdouble numberValue)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  v8::Local<v8::Number> n = v8::Number::New(fci.GetIsolate(), numberValue);
  fci.GetReturnValue().Set(n);
}

/*
 * Class:     JNode
 * Method:    functionHandleCallWithNoArguments
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_JNode_functionHandleCallWithNoArguments
  (JNIEnv *, jclass, jlong functionPerisstentHandle)
{
  PersistentFunctionStruct * pfs = reinterpret_cast<PersistentFunctionStruct *>(
    reinterpret_cast<void *>(functionPerisstentHandle));

  // NOTE: discovered by reading:
  // https://groups.google.com/forum/#!topic/v8-users/6kSAbnUb-rQ
  // Especially the message at:
  // https://groups.google.com/d/msg/v8-users/6kSAbnUb-rQ/T2BS0O-LuGAJ
  v8::Local<v8::Function> f = *reinterpret_cast<v8::Local<v8::Function> *>(&pfs->f);

  v8::Local<v8::Value> av[0] = {};

  f->Call(v8::Null(pfs->isolate), 0, av);
}

/*
 * Class:     JNode
 * Method:    functionPersistentHandleDestroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_JNode_functionPersistentHandleDestroy
  (JNIEnv *, jclass, jlong functionPerisstentHandle)
{
  PersistentFunctionStruct * pfs = reinterpret_cast<PersistentFunctionStruct *>(
    reinterpret_cast<void *>(functionPerisstentHandle));

  pfs->f.Reset();

  delete pfs;
}
