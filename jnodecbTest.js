var JNodeCB = require('./build/Release/JNodeCB.node');

var staticTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethod');

var testCallResult = staticTestMethodObject.call(3, 4);
console.log('got static test call result: ' + testCallResult);

var staticTestMethodObjectWithCallback = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethodWithCallback');

staticTestMethodObjectWithCallback.call(function(a, b) {
  console.log('Got callback from Java');
  console.log('a: ' + a);
  console.log('b: ' + b);
});

console.log('END OF TEST');
