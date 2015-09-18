public class JNode {
  public static native void start(String[] args);

  public static native int fciArgCount(long fciHandle);
  public static native boolean fciArgIsNumber(long fciHandle, int argIndex);
  public static native double fciArgNumberValue(long fciHandle, int argIndex);
  public static native void fciSetReturnNumberValue(long fciHandle, double numberValue);
}
