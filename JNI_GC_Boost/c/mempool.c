#include <jni.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "com_example_mempool_MemPool.h"

#define BLOCK_TYPES 4
#define BLOCK_COUNTS 64
#define ALIGN(s,a) (((s)+(a-1)) & ~(a-1))

int blockSizes[BLOCK_TYPES] = {4096, 8192, 16384, 32768};  // 4KB、8KB、16KB、32KB

typedef struct FreeBlock {
    struct FreeBlock* next;
} FreeBlock;

typedef struct {
    FreeBlock* freeList;
    int blockSize;
} MemoryBlockPool;

static MemoryBlockPool pools[BLOCK_TYPES];

// 初始化内存池，预先分配固定数量的内存块
void initMemoryPool() {
    for (int i = 0; i < BLOCK_TYPES; i++) {
        pools[i].blockSize = blockSizes[i];
        pools[i].freeList = NULL;
        for (int j = 0; j < BLOCK_COUNTS; j++) {
            void* blk = malloc(pools[i].blockSize);
            ((FreeBlock*)blk)->next = pools[i].freeList;
            pools[i].freeList = (FreeBlock*)blk;
        }
    }
}

// 根据请求大小找到合适的池索引
static int findIdx(int size) {
    for (int i = 0; i < BLOCK_TYPES; i++) {
        if (blockSizes[i] >= size)
            return i;
    }
    return -1;
}

// 分配内存块，先从空闲链表取，取不到则 malloc
void* allocateBlock(int size) {
    size = ALIGN(size, 8);
    int idx = findIdx(size);
    if (idx < 0) return NULL;
    if (pools[idx].freeList) {
        void* block = pools[idx].freeList;
        pools[idx].freeList = pools[idx].freeList->next;
        return block;
    }
    return malloc(pools[idx].blockSize);
}

// 释放内存块，放回对应空闲链表
void deallocateBlock(void* ptr, int size) {
    size = ALIGN(size, 8);
    int idx = findIdx(size);
    if (idx < 0) return;
    FreeBlock* fb = (FreeBlock*)ptr;
    fb->next = pools[idx].freeList;
    pools[idx].freeList = fb;
}

// JNI接口实现

JNIEXPORT void JNICALL Java_com_example_mempool_MemPool_initPool
  (JNIEnv *env, jclass clazz) {
    initMemoryPool();
}

JNIEXPORT jlong JNICALL Java_com_example_mempool_MemPool_alloc
  (JNIEnv *env, jclass clazz, jint size) {
    void* ptr = allocateBlock(size);
    return (jlong)(intptr_t)ptr;
}

JNIEXPORT void JNICALL Java_com_example_mempool_MemPool_free
  (JNIEnv *env, jclass clazz, jlong addr, jint size) {
    void* ptr = (void*)(intptr_t)addr;
    deallocateBlock(ptr, size);
}
