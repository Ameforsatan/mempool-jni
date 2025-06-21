# JNI Memory Pool for Java GC Bypass 🚀

一个使用 C 语言实现的内存池，通过 JNI 接入 Java，实现绕过 GC 的高速内存管理机制。

## 📌 项目背景

Java 内存由 GC 管理，但在大量短生命周期的内存分配中，GC 会造成性能抖动。本项目设计了一种用 JNI 调用 C 语言内存池的方式，绕过 GC，提升分配性能。

## 🔧 项目结构

- `csrc/mempool.c`：内存池核心实现
- `jsrc/MemPool.java`：JNI桥接类
- `jsrc/GCTest.java`：性能测试类
- `include/*.h`：JNI自动生成头文件

## 🚀 编译 & 运行

```bash
# 编译共享库（以Linux为例）
gcc -shared -fPIC -I${JAVA_HOME}/include -I${JAVA_HOME}/include/linux \
    -o build/libmempool.so csrc/mempool.c

# 编译Java
javac -d build jsrc/com/example/mempool/*.java

# 运行（添加native路径）
java --enable-native-access=ALL-UNNAMED \
     -Djava.library.path=build \
     -cp build com.example.mempool.GCTest
#先详情运行代码如下：
@echo off
set JAVA_HOME=C:\Program Files\Java\jdk-24

rem 一键确保 build 目录存在
if not exist build mkdir build

rem 生成 JNI 头文件
javac -h c -d build src\com\example\mempool\MemPool.java src\com\example\mempool\GCTest.java

rem 编译 C 动态库
gcc -I"%JAVA_HOME%\include" -I"%JAVA_HOME%\include\win32" -shared -o build\mempool.dll c\mempool.c

rem 运行测试
java --enable-native-access=ALL-UNNAMED -Djava.library.path=build -cp build com.example.mempool.GCTest
