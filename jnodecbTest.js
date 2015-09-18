var JNodeCB = require('./build/Release/JNodeCB.node');

var testMethodResult = JNodeCB.callMethod("JNodeTestCB", "testMethod", 3, 4);
console.log("testMethodResult: " + testMethodResult);
console.log('END OF TEST');
