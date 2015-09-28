public class JNodeTestCB {
  public static void testMethod(long fciHandle) {
    //System.out.println("Java testMethod() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    //System.out.println("arg count: " + argCount);
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
    //System.out.println("number argument a: " + a);
    double b = JNodeNative.fciArgNumberValue(fciHandle, 1);
    //System.out.println("number argument b: " + b);

    // return the sum:
    JNodeNative.fciSetReturnNumberValue(fciHandle, a + b);
  }

  public static void testMethodWithCallback(long fciHandle) {
    //System.out.println("Java testMethodWithCallback() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    //System.out.println("arg count: " + argCount);
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
    JNodeNative.functionPersistentHandleDestroy(fph);
    System.out.println("Got result back from JS callback: " + r);
  }
}
