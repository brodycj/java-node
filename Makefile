#
# NOTE: _Some_ credit goes to:
# - https://github.com/nodejs/node-v0.x-archive/issues/7310#issuecomment-40280294 for suggestion to use the `-all_load` flag which was needed to get this working on OSX
# - https://github.com/jxcore/jxcore/blob/master/doc/native/Embedding_Basics.md for identification of _which_ v8 libraries to include

NODE_VERSION = 4.1.2

NODE_LIB = node-v$(NODE_VERSION)/out/Release/libnode.a
NODE_DIST = https://nodejs.org/dist/v4.1.2/node-v$(NODE_VERSION).tar.gz
NODE_DIR = node-v$(NODE_VERSION)
NODE_CONFIGURE = $(NODE_DIR)/configure

NODECB_ADDON = build/Release/JNodeCB.node

all: $(NODECB_ADDON) JNodeTestCB.class JNodeTest.class JNodeNative.class libJNode.jnilib

clean:
	rm -f libJNode.jnilib *.class

distclean:
	rm -f libJNode.jnilib *.class $(NODE_DIR)

test: all
	java JNodeTest jnodecbTest.js

$(NODECB_ADDON):
	node-gyp --target=v$(NODE_VERSION) configure build

JNodeTestCB.class: JNodeTestCB.java JNodeNative.java
	javac JNodeTestCB.java

simpletest: JNodeTest.class JNodeNative.class libJNode.jnilib
	java JNodeTest -e "console.log('3 + 4 = ' + (3+4))"

JNodeTest.class: JNodeTest.java JNode.java JNodeNative.java
	javac JNodeTest.java

JNode.class: JNode.java
	javac JNode.java

JNodeNative.class: JNodeNative.java
	javac JNodeNative.java

libJNode.jnilib: JNodeNative.cc JNodeNative.h $(NODE_LIB)
	c++ -all_load -dynamiclib -std=c++11 -stdlib=libstdc++ -olibJNode.jnilib JNodeNative.cc \
	  -I/System/Library/Frameworks/JavaVM.framework/Headers -I$(NODE_DIR)/src \
		-I$(NODE_DIR)/deps/v8/include -framework \
		JavaVM $(NODE_DIR)/out/Release/libnode.a \
		$(NODE_DIR)/out/Release/libuv.a \
		$(NODE_DIR)/out/Release/libv8_base.a \
		$(NODE_DIR)/out/Release/libv8_nosnapshot.a \
		$(NODE_DIR)/out/Release/libv8_libbase.a $(NODE_DIR)/out/Release/libv8_libplatform.a \
		$(NODE_DIR)/out/Release/libcares.a $(NODE_DIR)/out/Release/libzlib.a \
		$(NODE_DIR)/out/Release/libhttp_parser.a $(NODE_DIR)/out/Release/libopenssl.a

$(NODE_LIB): $(NODE_CONFIGURE)
	cd $(NODE_DIR) && ./configure --enable-static && make

$(NODE_CONFIGURE):
	curl $(NODE_DIST) | tar xzf -
