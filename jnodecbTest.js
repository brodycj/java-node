var JNodeCB = require('./build/Release/JNodeCB.node');

var testMethodResult = JNodeCB.callMethod("JNodeTestCB", "testMethod", 3, 4);
console.log("testMethod result (3+4): " + testMethodResult);

JNodeCB.callMethod("JNodeTestCB", "testMethodWithCallback", function() {
  console.log("Got empty callback from Java");
});

console.log('END OF TEST');
