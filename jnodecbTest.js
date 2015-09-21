var JNodeCB = require('./build/Release/JNodeCB.node');

var staticTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethod');

var testCallResult = staticTestMethodObject.call(3, 4);
console.log('got static test call result: ' + testCallResult);

var staticTestMethodObjectWithCallback = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethodWithCallback');

staticTestMethodObjectWithCallback.call(function() {
  console.log('Got empty callback from Java');
});

console.log('END OF TEST');
