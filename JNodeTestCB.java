public class JNodeTestCB {
  public static void testMethod(long fciHandle) {
    //System.out.println("Java testMethod() called");

    int argCount = JNode.fciArgCount(fciHandle);
    // NOTE: ignore first two (FUTURE TBD will be fixed)
    //System.out.println("arg count: " + argCount);
    if (argCount < 4) {
      System.err.println("ERROR: not enough arguments");
      return;
    }
    if (!JNode.fciArgIsNumber(fciHandle, 2) ||
        !JNode.fciArgIsNumber(fciHandle, 3)) {
      System.err.println("ERROR: incorrect arguments, number arguments expected");
      return;
    }

    double a = JNode.fciArgNumberValue(fciHandle, 2);
    //System.out.println("number argument a: " + a);
    double b = JNode.fciArgNumberValue(fciHandle, 3);
    //System.out.println("number argument b: " + b);

    // return the sum:
    JNode.fciSetReturnNumberValue(fciHandle, a + b);
  }

  public static void testMethodWithCallback(long fciHandle) {
    //System.out.println("Java testMethodWithCallback() called");

    int argCount = JNode.fciArgCount(fciHandle);
    // NOTE: ignore first two (FUTURE TBD will be fixed)
    //System.out.println("arg count: " + argCount);
    if (argCount < 3) {
      System.err.println("ERROR: not enough arguments");
      return;
    }

    if (!JNode.fciArgIsFunction(fciHandle, 2)) {
      System.err.println("ERROR: incorrect argument, function argument expected");
      return;
    }

    JNode.fciArgFunctionCallWithNoArguments(fciHandle, 2);
  }
}
