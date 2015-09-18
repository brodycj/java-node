# java-node

Native interface between Java and Node (v4) library

Author: Christopher J. Brody

License: MIT

I can hereby testify that this project is completely my own work and not subject to agreements with any other parties.
In case of code written under direct guidance from sample code the link is given for reference.
In case I accept contributions from any others I will require CLA with similar statements.
I may offer this work under other licensing terms in the future.

Highlights:
- Java-driven startup
- Static function callback Javascript --> Java
- TODO:
  - Callbacks Java --> Javascript

Status:
- Builds on OSX (TODO put in shell script)
- Java-driven startup working
- JS --> Java test static function call with no parameters or return working

TODO:
- Load JNI library in the library class
- Javascript --> Java static function calls using reflection
- Javascript --> Java static function calls with parameters and return value
- Callback Java --> Javascript
- Support build on Linux
- automated build
- Use proper Java package IDs

For future consideration:
- integration with node-java which provides nice support for non-static Javascript --> Java function calls
- Windows
- Also target JXCore
- Support mobile platforms (with help from JXCore)

External requirements:
- Java JDK (tested with Oracle Java JDK 8)
- Node.js (4.0) source
- Build tools
- `node-gyp`

## Usage

To start Node.js library from Java:

```Java
public class JNodeTest {
  public static void main(String[] args) {
    System.loadLibrary("JNode");

    JNode.start(args);
  }
}
```

and run from command line:

```Java
java JNodeTest node -e "console.log('asfd')"
```

XXX TODO document JS --> Java calls

## Build

Extract Node 4.0.0 source code, configure with `--enable-static`, and build:

```shell
tar xzvf <path-to>node-v4.0.0.tar.gz
cd node-v4.0.0
./configure --enable-static
make
cd ..
```

Build JNode.class:

```shell
javac JNode.java
```

Build JNI library:

```shell
c++ -all_load -dynamiclib -std=c++11 -stdlib=libstdc++ -olibJNode.jnilib JNode.cc -I/System/Library/Frameworks/JavaVM.framework/Headers -Inode-v4.0.0/src -Inode-v4.0.0/deps/v8/include -framework JavaVM node-v4.0.0/out/Release/libnode.a node-v4.0.0/out/Release/libuv.a node-v4.0.0/out/Release/libv8_base.a node-v4.0.0/out/Release/libv8_nosnapshot.a node-v4.0.0/out/Release/libv8_libbase.a node-v4.0.0/out/Release/libv8_libplatform.a node-v4.0.0/out/Release/libcares.a node-v4.0.0/out/Release/libzlib.a node-v4.0.0/out/Release/libhttp_parser.a node-v4.0.0/out/Release/libopenssl.a
```

NOTE: _Some_ credit goes to:
- https://github.com/nodejs/node-v0.x-archive/issues/7310#issuecomment-40280294 for suggestion to use the `-all_load` flag which was needed to get this working on OSX
- https://github.com/jxcore/jxcore/blob/master/doc/native/Embedding_Basics.md for identification of _which_ v8 libraries to include

Build JNodeCB addon:

```shell
node-gyp configure build
```

should generate: `build/Release/JNodeCB.node`

## Testing

### Java-driven startup test

Build Java-driven startup test program:

```shell
javac JNodeTest.java
```

Run simple Java-driven startup test:

```shell
java JNodeTest node -e "console.log('3 + 4 = ' + (3+4))"
```

### Java callback test

Javascript in `jnodecbTest.js`:

```Javascript
var JNodeCB = require('./build/Release/JNodeCB.node');

JNodeCB.callVoidTestMethodWithNoParameters();
console.log('finished test');
```

Build Java callback test:

```shell
javac JNodeTestCB.java
```

To run Java callback test:

```shell
java JNodeTest node jnodecbTest.js
```


## To regenerate JNI header

First rebuild JNode.class:

```shell
javac JNode.java
```

Then regenerate JNode.h:

```shell
javah JNode
```

`JNode.h` should now be regenerated.

