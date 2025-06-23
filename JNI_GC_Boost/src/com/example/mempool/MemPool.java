package com.example.mempool;

public class MemPool {
    static {
        System.loadLibrary("mempool");
    }

    public static native void initPool();
    public static native long alloc(int size);
    public static native void free(long addr, int size);
}
