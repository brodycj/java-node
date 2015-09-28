public class JNodeTest {
  public static void main(String[] args) {
    // XXX TODO should be part of library:
    System.loadLibrary("JNode");

    // args[0] should _not_ have to be specified by the user:
    String[] node_args = new String[args.length + 1];
    node_args[0] = new String("node");
    System.arraycopy(args, 0, node_args, 1, args.length);
    JNodeNative.start(node_args);
  }
}
