public class JNodeTestCB {
  public static void stringTestMethod(long fciHandle) {
    //System.out.println("Java stringTestMethod() called");

    int argCount = JNodeNative.fciArgCount(fciHandle);
    //System.out.println("arg count: " + argCount);
    if (argCount < 2) {
      System.err.println("ERROR: not enough arguments");
      return;
    }
    if (!JNodeNative.fciArgIsString(fciHandle, 0) ||
        !JNodeNative.fciArgIsString(fciHandle, 1)) {
      System.err.println("ERROR: incorrect arguments, string arguments expected");
      return;
    }

    String a = JNodeNative.fciArgStringValue(fciHandle, 0);
    //System.out.println("argument a: " + a);
    String b = JNodeNative.fciArgStringValue(fciHandle, 1);
    //System.out.println("argument b: " + b);

    // return a concatenation:
    JNodeNative.fciSetReturnStringValue(fciHandle, a + "," + b);
  }

  public static void numberTestMethod(long fciHandle) {
    //System.out.println("Java numberTestMethod() called");

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
    JNodeNative.fcoAddDoubleParameter(fco, 123.456);
    JNodeNative.fcoAddStringParameter(fco, "abc");
    JNodeNative.fcoAddIntegerParameter(fco, 789);
    // TODO: support & test JNodeNative.fcoDoubleCallAndDestroy()
    JNodeNative.fcoVoidCallAndDestroy(fco);
    JNodeNative.functionPersistentHandleDestroy(fph);
  }

  // XXX TODO two-way Javascript-Java callback test
}
