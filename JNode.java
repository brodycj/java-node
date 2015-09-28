public class JNode {
  static {
    System.loadLibrary("JNodeNative");
  }

  public static void start(String[] args) {
    JNodeNative.start(args);
  }
}
