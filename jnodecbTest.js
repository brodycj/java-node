var JNodeCB = require('./build/Release/JNodeCB.node');
var assert = require('assert');

var stringTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'stringTestMethod');
var stringTestCallResult = stringTestMethodObject.call('first', 'second');
console.log('got static string test call result: ' + stringTestCallResult);
assert.equal(stringTestCallResult, 'first,second');

var staticTestMethodObject = JNodeCB.getStaticMethodObject('JNodeTestCB', 'numberTestMethod');
var numericTestCallResult = staticTestMethodObject.call(3, 4);
console.log('got static numeric test call result: ' + numericTestCallResult);
assert.equal(numericTestCallResult, 7);

var staticTestMethodObjectWithCallback = JNodeCB.getStaticMethodObject('JNodeTestCB', 'testMethodWithCallback');

var storea = null;
var storeb = null;
var storec = null;

staticTestMethodObjectWithCallback.call(function(a, b, c) {
  console.log('Got callback from Java');
  console.log('a: ' + a);
  console.log('b: ' + b);
  console.log('c: ' + c);

  storea = a;
  storeb = b;
  storec = c;
});

assert.equal(storea, 123.456);
assert.equal(storeb, 'abc');
assert.equal(storec, 789);

// XXX TODO two-way Javascript-Java callback test

console.log('END OF TEST');
