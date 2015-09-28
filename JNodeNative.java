public class JNodeNative {
  public static native void start(String[] args);

  public static native int fciArgCount(long fciHandle);
  public static native boolean fciArgIsNumber(long fciHandle, int argIndex);
  public static native boolean fciArgIsFunction(long fciHandle, int argIndex);
  public static native double fciArgNumberValue(long fciHandle, int argIndex);
  public static native long fciArgFunctionAsPersistentHandle(long fciHandle, int argIndex);
  public static native void fciSetReturnNumberValue(long fciHandle, double numberValue);
  public static native long fcoFromHandle(long functionHandle);
  public static native void fcoAddIntegerParameter(long fcoHandle, int value);
  public static native void fcoVoidCallAndDestroy(long fcoHandle);
  public static native int fcoIntCallAndDestroy(long fcoHandle);
  public static native void functionPersistentHandleDestroy(long functionHandle);
}
