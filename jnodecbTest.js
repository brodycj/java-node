var JNodeCB = require('./build/Release/JNodeCB.node');

// Old:
var testMethodResult = JNodeCB.callMethod('JNodeTestCB', 'testMethod', 3, 4);
console.log('testMethod result (3+4): ' + testMethodResult);

JNodeCB.callMethod('JNodeTestCB', 'testMethodWithCallback', function() {
  console.log('Got empty callback from Java');
});

// New:
var staticMethodObject1 = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethod');
console.log('Issuing new static method call:');
// XXX TEMP extra arguments needed, will go away:
var testCallResult = staticMethodObject1.call('temp extra', 'args', 3, 4);
console.log('got test call result: ' + testCallResult);

var staticMethodObject2 = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethodWithCallback');
console.log('Issuing new static method call with callback Java --> JS:');
staticMethodObject2.call('temp extra', 'args', function() {
  console.log('Got empty callback from Java');
});

console.log('END OF TEST');
