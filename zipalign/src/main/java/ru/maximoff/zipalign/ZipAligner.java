package ru.maximoff.zipalign;

public class ZipAligner {
    public static final int DEFAULT_LEVEL = 4;
    
    static {
        System.loadLibrary("zipalign");
    }

    public static native boolean align(String srcZip, String destZip, int alignLevel, boolean pageAlignSharedLibs);
    public static native boolean isAligned(String srcZip, int alignLevel, boolean pageAlignSharedLibs);
}
