package cn.longmaster.pengpeng.ywlog;

/**
 * Created by 杨浩 on 2016/10/22.
 */
public class YWLog {
    static {
        System.loadLibrary("YWLog");
    }

    public static native int init(String filePath, long size);
    public static native boolean uninit();
    public static native void flush();
    public static native void d(String log);
}
