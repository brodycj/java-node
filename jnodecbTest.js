var JNodeCB = require('./build/Release/JNodeCB.node');
var assert = require('assert');

var staticTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethod');

var testCallResult = staticTestMethodObject.call(3, 4);
console.log('got static test call result: ' + testCallResult);
assert.equal(testCallResult, 7);

var staticTestMethodObjectWithCallback = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethodWithCallback');

var storea = 0;
var storeb = 0;

staticTestMethodObjectWithCallback.call(function(a, b) {
  console.log('Got callback from Java');
  console.log('a: ' + a);
  console.log('b: ' + b);

  storea = a;
  storeb = b;

  return a+b;
});

assert.equal(storea, 123);
assert.equal(storeb, 456);

console.log('END OF TEST');
