# java-node

Native interface between Java and Node (v4.x) library

Author: Christopher J. Brody

License: MIT

I can hereby testify that this project is completely my own work and not subject to agreements with any other parties.
In case of code written under direct guidance from sample code the link is given for reference.
In case I accept contributions from any others I will require CLA with similar statements.
I may offer this work under other licensing terms in the future.

NOTE: This project is under development and should be considered experimental.
API is subject to change and some optimizations are expected to be required.

Highlights:
- Java-driven startup
- Static function callback Javascript --> Java
- Callback Java --> Javascript

Status:
- Builds on OSX
- Java-driven startup working
- JS --> Java static function call using reflection working with support for string and numeric parameters and return value
- Java --> JS callback now working with support for string and numeric parameters, with possible _integer_ return value

TODO:
- smart handling of multiple versions of Node.js
- build and test on Linux
- JS --> Java with string, double-precision numeric, and function return value
- array, "double-precision" and simple Object parameters and return value JS <--> Java
- JNI efficiency ref: http://www.ibm.com/developerworks/library/j-jni/
- Support build on Linux
- Use proper Java package IDs
- Java API to get function call parameters using a real object instead of a long (64-bit) pointer handle

For future consideration:
- True object factory on the Java side
- Non-static function calls JS --> Java
- Deal with 64-bit numbers (not truly supported by _standard_ JS)
- integration with node-java which provides nice support for non-static Javascript --> Java function calls
- Windows
- Also target JXCore
- Support mobile platforms (with help from JXCore)
- Support throwing exceptions in both directions JS <--> Java

External requirements:
- Java JDK (tested with Oracle Java JDK 8)
- Build tools
- `node-gyp`

Externally fetched:
- Node.js source (`4.2.3`)

## Usage

### Start from Java

To start Node.js library from Java:

```Java
public class MyJavaNodeProgram {
  public static void main(String[] args) {
    JNode.start(args);
  }
}
```

and run from command line:

```Java
java MyJavaNodeProgram node -e "console.log('asfd')"
```

### Javascript call to static Java function

Test Javascript code:

```Javascript
var JNodeCB = require('./build/Release/JNodeCB.node');

var stringTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'stringTestMethod');
var stringTestCallResult = stringTestMethodObject.call('first', 'second');
console.log('got static string test call result: ' + stringTestCallResult);

var staticTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'numberTestMethod');
var numericTestCallResult = staticTestMethodObject.call(3, 4);
console.log('got static numeric test call result: ' + numericTestCallResult);
```

Test Java class:

```Java
public class JNodeTestCB {
  public static void stringTestMethod(long fciHandle) {
    System.out.println("Java stringTestMethod() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    System.out.println("arg count: " + argCount);
    if (argCount < 2) {
      System.err.println("ERROR: not enough arguments");
      return;
    }
    if (!JNodeNative.fciArgIsString(fciHandle, 0) ||
        !JNodeNative.fciArgIsString(fciHandle, 1)) {
      System.err.println("ERROR: incorrect arguments, string arguments expected");
      return;
    }

    String a = JNodeNative.fciArgStringValue(fciHandle, 0);
    System.out.println("argument a: " + a);
    String b = JNodeNative.fciArgStringValue(fciHandle, 1);
    System.out.println("argument b: " + b);

    // return a concatenation:
    JNodeNative.fciSetReturnStringValue(fciHandle, a + "," + b);
  }

  public static void numberTestMethod(long fciHandle) {
    System.out.println("Java numberTestMethod() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    System.out.println("arg count: " + argCount);
    if (argCount < 2) {
      System.err.println("ERROR: not enough arguments");
      return;
    }
    if (!JNodeNative.fciArgIsNumber(fciHandle, 0) ||
        !JNodeNative.fciArgIsNumber(fciHandle, 1)) {
      System.err.println("ERROR: incorrect arguments, number arguments expected");
      return;
    }

    double a = JNodeNative.fciArgNumberValue(fciHandle, 0);
    System.out.println("number argument a: " + a);
    double b = JNodeNative.fciArgNumberValue(fciHandle, 1);
    System.out.println("number argument b: " + b);

    // return the sum:
    JNodeNative.fciSetReturnNumberValue(fciHandle, a + b);
  }
}
```

To run from command line:

```shell
java JNodeTest node jnodecbTest.js
```

### Callback from Java to Javascript

Javascript:

```Javascript
var JNodeCB = require('./build/Release/JNodeCB.node');

var staticTestMethodObjectWithCallback = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethodWithCallback');

staticTestMethodObjectWithCallback.call(function(a, b, c) {
  console.log('Got callback from Java');
  console.log('a: ' + a);
  console.log('b: ' + b);
  console.log('c: ' + c);
});
```

Java:

```Java
public class JNodeTestCB {
  public static void testMethodWithCallback(long fciHandle) {
    System.out.println("Java testMethodWithCallback() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    System.out.println("arg count: " + argCount);
    if (argCount < 1) {
      System.err.println("ERROR: not enough arguments");
      return;
    }

    if (!JNodeNative.fciArgIsFunction(fciHandle, 0)) {
      System.err.println("ERROR: incorrect argument, function argument expected");
      return;
    }

    long fph = JNodeNative.fciArgFunctionAsPersistentHandle(fciHandle, 0);
    long fco = JNodeNative.fcoFromHandle(fph);
    JNodeNative.fcoAddDoubleParameter(fco, 123.456);
    JNodeNative.fcoAddStringParameter(fco, "abc");
    JNodeNative.fcoAddIntegerParameter(fco, 789);
    // TODO: support & test JNodeNative.fcoDoubleCallAndDestroy()
    JNodeNative.fcoVoidCallAndDestroy(fco);
    JNodeNative.functionPersistentHandleDestroy(fph);
  }
}
```

## Build

```shell
make
```

NOTE: This will fetch and extract Node (`4.2.3`) source code if necessary.

## Testing

### Simple Java-driven startup test

Run simple Java-driven startup test:

```shell
java JNodeTest -e "console.log('3 + 4 = ' + (3+4))"
```

### Two-way Javascript/Java callback test

XXX TODO: two-way Javascript/Java callback working for integers only

Javascript in `jnodecbTest.js`:

```Javascript
var JNodeCB = require('./build/Release/JNodeCB.node');

var staticTestMethodObjectWithCallback = JNodeCB.getStaticMethodObject('JNodeTestCB', 'twoWayIntegerCallbackTest');

staticTestMethodObjectWithCallback.call(function(a, b) {
  console.log('Got callback from Java');
  console.log('a: ' + a);
  console.log('b: ' + b);
  return a+b;
});
```

Java:

```Java
public class JNodeTestCB {
  public static void twoWayIntegerCallbackTest(long fciHandle) {
    System.out.println("Java twoWayIntegerCallbackTest() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    System.out.println("arg count: " + argCount);
    if (argCount < 1) {
      System.err.println("ERROR: not enough arguments");
      return;
    }

    if (!JNodeNative.fciArgIsFunction(fciHandle, 0)) {
      System.err.println("ERROR: incorrect argument, function argument expected");
      return;
    }

    long fph = JNodeNative.fciArgFunctionAsPersistentHandle(fciHandle, 0);
    long fco = JNodeNative.fcoFromHandle(fph);

    JNodeNative.fcoAddIntegerParameter(fco, 123);
    JNodeNative.fcoAddIntegerParameter(fco, 456);
    int r = JNodeNative.fcoIntCallAndDestroy(fco);
    System.out.println("Got result back from JS callback: " + r);
  }
}
```

### Build and run Javascript/Java callback tests

Build Java callback test:

```shell
make all
```

To run Java callback test, with automatic result checking:

```shell
make test
```
or
```shell
java JNodeTest jnodecbTest.js
```

## To regenerate JNI header

First rebuild JNodeNative.class:

```shell
javac JNodeNative.java
```

Then regenerate JNodeNative.h:

```shell
javah JNodeNative
```

`JNodeNative.h` should now be regenerated.
