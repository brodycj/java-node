public class JNodeTestCB {
  public static void testMethod(long fciHandle) {
    //System.out.println("Java testMethod() called");

    int argCount = JNode.fciArgCount(fciHandle);
    //System.out.println("arg count: " + argCount);
    if (argCount < 2) {
      System.err.println("ERROR: not enough arguments");
      return;
    }
    if (!JNode.fciArgIsNumber(fciHandle, 0) ||
        !JNode.fciArgIsNumber(fciHandle, 1)) {
      System.err.println("ERROR: incorrect arguments, number arguments expected");
      return;
    }

    double a = JNode.fciArgNumberValue(fciHandle, 0);
    //System.out.println("number argument a: " + a);
    double b = JNode.fciArgNumberValue(fciHandle, 1);
    //System.out.println("number argument b: " + b);

    // return the sum:
    JNode.fciSetReturnNumberValue(fciHandle, a + b);
  }

  public static void testMethodWithCallback(long fciHandle) {
    //System.out.println("Java testMethodWithCallback() called");

    int argCount = JNode.fciArgCount(fciHandle);
    //System.out.println("arg count: " + argCount);
    if (argCount < 1) {
      System.err.println("ERROR: not enough arguments");
      return;
    }

    if (!JNode.fciArgIsFunction(fciHandle, 0)) {
      System.err.println("ERROR: incorrect argument, function argument expected");
      return;
    }

    long fph = JNode.fciArgFunctionAsPersistentHandle(fciHandle, 0);
    JNode.functionHandleCallWithNoArguments(fph);
    JNode.functionPersistentHandleDestroy(fph);
  }
}
