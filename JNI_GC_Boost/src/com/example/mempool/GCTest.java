package com.example.mempool;

public class GCTest {
    // JNI方法调用
    public static native long allocate(int size);
    public static native void free(long ptr, int size);

    static {
        System.loadLibrary("mempool");
    }

    public static void main(String[] args) {
        MemPool.initPool();  // 初始化内存池

        int N = 100_000;          // 10万次分配
        int size = 16 * 1024;     // 16KB

        System.out.println("开始Java原生分配测试...");
        long start1 = System.currentTimeMillis();
        for (int i = 0; i < N; i++) {
            byte[] data = new byte[size];
        }
        long end1 = System.currentTimeMillis();
        System.out.println("Java原生分配耗时: " + (end1 - start1) + " ms");

        System.out.println("开始JNI内存池分配释放测试...");
        long[] ptrs = new long[N];

        long start2 = System.currentTimeMillis();
        for (int i = 0; i < N; i++) {
            ptrs[i] = MemPool.alloc(size);
        }
        for (int i = 0; i < N; i++) {
            MemPool.free(ptrs[i], size);
        }
        long end2 = System.currentTimeMillis();
        System.out.println("JNI内存池分配+释放耗时: " + (end2 - start2) + " ms");
    }
}
