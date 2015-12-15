#include "JNodeNative.h"
#include "node.h"

#include <string>
#include <vector>

// Made with *some* help from:
// - https://nodejs.org/api/addons.html

/* exported for use by JNodeCB addon: */
// XXX FUTURE TBD find a smarter way to export this:
JNIEnv * jnode_jni_env;

struct PersistentFunctionStruct {
  PersistentFunctionStruct(v8::Isolate * isolate, v8::Local<v8::Function> & f) :
    isolate(isolate), f(isolate, f) { }

  v8::Isolate * isolate;
  v8::Persistent<v8::Function> f;
};

enum fco_type {
  FCO_NONE = 0,
  FCO_STRING,
  FCO_DOUBLE,
  FCO_INT,
  FCO_TYPE_COUNT
};

struct fco_param {
  fco_param(const char * value) : type(FCO_STRING), stringValue(strdup(value)) { }

  fco_param(double value) : type(FCO_DOUBLE), doubleValue(value) { }

  fco_param(int value) : type(FCO_INT), intValue(value) { }

  fco_param(const fco_param & rhs) : type(rhs.type) {
    switch(rhs.type) {
    case FCO_STRING:
      // XXX TODO: extra strdup/free should not be necessary!
      stringValue = strdup(rhs.stringValue);
      break;
    case FCO_DOUBLE:
      doubleValue = rhs.doubleValue;
      break;
    case FCO_INT:
      intValue = rhs.intValue;
      break;
    // XXX TODO other cases
    }
  }

  fco_param & operator=(const fco_param & rhs) {
    type = rhs.type;
    // XXX TODO REPEATED CODE
    switch(rhs.type) {
    case FCO_STRING:
      // XXX TODO: extra strdup/free should not be necessary!
      stringValue = strdup(rhs.stringValue);
      break;
    case FCO_DOUBLE:
      doubleValue = rhs.doubleValue;
      break;
    case FCO_INT:
      intValue = rhs.intValue;
      break;
    // XXX TODO other cases
    }
    return *this;
  }

  ~fco_param() {
    if (type == FCO_STRING) {
      // XXX TODO
      free(stringValue);
    }
  }

  fco_type type;
  union {
    char * stringValue;
    double doubleValue;
    int intValue;
  };
};

struct FunctionCallObject {
  FunctionCallObject(PersistentFunctionStruct * pfs) : pfs(pfs) { }

  PersistentFunctionStruct * pfs;
  std::vector<fco_param> params;
};

/*
 * Class:     JNodeNative
 * Method:    start
 * Signature: ([Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_start
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
 * Class:     JNodeNative
 * Method:    fciArgCount
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_JNodeNative_fciArgCount
  (JNIEnv *, jclass, jlong fciHandle)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci.Length();
}

/*
 * Class:     JNodeNative
 * Method:    fciArgIsString
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_JNodeNative_fciArgIsString
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->IsString();
}
/*
 * Class:     JNodeNative
 * Method:    fciArgIsNumber
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_JNodeNative_fciArgIsNumber
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->IsNumber();
}

/*
 * Class:     JNodeNative
 * Method:    fciArgIsFunction
 * Signature: (JI)Z
 */
JNIEXPORT jboolean JNICALL Java_JNodeNative_fciArgIsFunction
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->IsFunction();
}

/*
 * Class:     JNodeNative
 * Method:    fciArgStringValue
 * Signature: (JI)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_JNodeNative_fciArgStringValue
  (JNIEnv * env, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return env->NewStringUTF(*v8::String::Utf8Value(fci[argIndex]->ToString()));
}

/*
 * Class:     JNodeNative
 * Method:    fciArgNumberValue
 * Signature: (JI)D
 */
JNIEXPORT jdouble JNICALL Java_JNodeNative_fciArgNumberValue
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  return fci[argIndex]->NumberValue();
}

/*
 * Class:     JNodeNative
 * Method:    fciArgFunctionAsPersistentHandle
 * Signature: (JI)J
 */
JNIEXPORT jlong JNICALL Java_JNodeNative_fciArgFunctionAsPersistentHandle
  (JNIEnv *, jclass, jlong fciHandle, jint argIndex)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;
  v8::Isolate * isolate = fci.GetIsolate();

  v8::Local<v8::Function> f = v8::Local<v8::Function>::Cast(fci[argIndex]);

  return reinterpret_cast<long>(new PersistentFunctionStruct(isolate, f));
}

/*
 * Class:     JNodeNative
 * Method:    fciSetReturnStringValue
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_fciSetReturnStringValue
  (JNIEnv * e, jclass, jlong fciHandle, jstring stringValue)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  const char * cs = e->GetStringUTFChars(stringValue, 0);

  v8::Local<v8::String> s = v8::String::NewFromUtf8(fci.GetIsolate(), cs);
  fci.GetReturnValue().Set(s);

  e->ReleaseStringUTFChars(stringValue, cs);
  e->DeleteLocalRef(stringValue);
}

/*
 * Class:     JNodeNative
 * Method:    fciSetReturnNumberValue
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_fciSetReturnNumberValue
  (JNIEnv *, jclass, jlong fciHandle, jdouble numberValue)
{
  v8::FunctionCallbackInfo<v8::Value> & fci = *(v8::FunctionCallbackInfo<v8::Value> *)fciHandle;

  v8::Local<v8::Number> n = v8::Number::New(fci.GetIsolate(), numberValue);
  fci.GetReturnValue().Set(n);
}

/*
 * Class:     JNodeNative
 * Method:    fcoFromHandle
 * Signature: (J)J
 */
JNIEXPORT jlong JNICALL Java_JNodeNative_fcoFromHandle
  (JNIEnv *, jclass, jlong functionPerisstentHandle)
{
  PersistentFunctionStruct * pfs = reinterpret_cast<PersistentFunctionStruct *>(
    reinterpret_cast<void *>(functionPerisstentHandle));

  return reinterpret_cast<long>(new FunctionCallObject(pfs));
}

/*
 * Class:     JNodeNative
 * Method:    fcoAddStringParameter
 * Signature: (JLjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_fcoAddStringParameter
  (JNIEnv * e, jclass, jlong fcoHandle, jstring stringValue)
{
  FunctionCallObject * fco = reinterpret_cast<FunctionCallObject *>(
    reinterpret_cast<void *>(fcoHandle));

  const char * cs = e->GetStringUTFChars(stringValue, 0);

  fco->params.push_back(fco_param(cs));

  e->ReleaseStringUTFChars(stringValue, cs);
  e->DeleteLocalRef(stringValue);
}

/*
 * Class:     JNodeNative
 * Method:    fcoAddDoubleParameter
 * Signature: (JD)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_fcoAddDoubleParameter
  (JNIEnv *, jclass, jlong fcoHandle, jdouble value)
{
  FunctionCallObject * fco = reinterpret_cast<FunctionCallObject *>(
    reinterpret_cast<void *>(fcoHandle));

  fco->params.push_back(fco_param(value));
}

/*
 * Class:     JNodeNative
 * Method:    fcoAddIntegerParameter
 * Signature: (JI)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_fcoAddIntegerParameter
  (JNIEnv *, jclass, jlong fcoHandle, jint value)
{
  FunctionCallObject * fco = reinterpret_cast<FunctionCallObject *>(
    reinterpret_cast<void *>(fcoHandle));

  fco->params.push_back(fco_param(value));
}

/*
 * Class:     JNodeNative
 * Method:    fcoVoidCallAndDestroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_fcoVoidCallAndDestroy
  (JNIEnv *, jclass, jlong fcoHandle)
{
  FunctionCallObject * fco = reinterpret_cast<FunctionCallObject *>(
    reinterpret_cast<void *>(fcoHandle));
  PersistentFunctionStruct * pfs = fco->pfs;

  v8::Local<v8::Function> f = v8::Local<v8::Function>::New(pfs->isolate, pfs->f);

  // XXX TODO factor out repeated code:
  std::vector<v8::Local<v8::Value>> av;
  for (std::vector<fco_param>::iterator iter = fco->params.begin();
       iter != fco->params.end(); ++iter) {
    switch(iter->type) {
    case FCO_STRING:
      av.push_back(v8::String::NewFromUtf8(pfs->isolate, iter->stringValue));
      break;
    case FCO_DOUBLE:
      av.push_back(v8::Number::New(pfs->isolate, iter->doubleValue));
      break;
    case FCO_INT:
      av.push_back(v8::Number::New(pfs->isolate, iter->intValue));
      break;
    }
  }

  // trick from: http://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array-c
  f->Call(v8::Null(pfs->isolate), av.size(), &av[0]);

  delete fco;
}

/*
 * Class:     JNodeNative
 * Method:    fcoIntCallAndDestroy
 * Signature: (J)I
 */
JNIEXPORT jint JNICALL Java_JNodeNative_fcoIntCallAndDestroy
  (JNIEnv *, jclass, jlong fcoHandle)
{
  FunctionCallObject * fco = reinterpret_cast<FunctionCallObject *>(
    reinterpret_cast<void *>(fcoHandle));
  PersistentFunctionStruct * pfs = fco->pfs;

  v8::Local<v8::Function> f = v8::Local<v8::Function>::New(pfs->isolate, pfs->f);

  // XXX TODO factor out repeated code:
  std::vector<v8::Local<v8::Value>> av;
  for (std::vector<fco_param>::iterator iter = fco->params.begin();
       iter != fco->params.end(); ++iter) {
    switch(iter->type) {
    case FCO_STRING:
      av.push_back(v8::String::NewFromUtf8(pfs->isolate, iter->stringValue));
      break;
    case FCO_DOUBLE:
      av.push_back(v8::Number::New(pfs->isolate, iter->doubleValue));
      break;
    case FCO_INT:
      av.push_back(v8::Number::New(pfs->isolate, iter->intValue));
      break;
    }
  }

  // trick from: http://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array-c
  v8::Local<v8::Value> rv = f->Call(v8::Null(pfs->isolate), av.size(), &av[0]);

  delete fco;

  // ref: http://stackoverflow.com/a/11387695/1283667
  return rv->IsInt32() ? rv->ToInt32()->Value() : 0;
}

/*
 * Class:     JNodeNative
 * Method:    functionPersistentHandleDestroy
 * Signature: (J)V
 */
JNIEXPORT void JNICALL Java_JNodeNative_functionPersistentHandleDestroy
  (JNIEnv *, jclass, jlong functionPerisstentHandle)
{
  PersistentFunctionStruct * pfs = reinterpret_cast<PersistentFunctionStruct *>(
    reinterpret_cast<void *>(functionPerisstentHandle));

  pfs->f.Reset();

  delete pfs;
}
