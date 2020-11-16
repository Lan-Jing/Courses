# 大作业-实现一个STL deque

## 描述
[std::deque](http://www.cplusplus.com/reference/deque/deque/) 是一个双端队列,双端队列是具有动态大小的序列容器，可以在两端（前端或后端）进行扩展或收缩。

特定的库可以以不同的方式实现deques，通常是某种形式的动态数组。但是在任何情况下，它们都允许通过随机访问迭代器直接访问单个元素，并根据需要通过扩展和收缩容器来自动处理存储。

因此，它们提供了一种类似于向量的功能，但是在序列的开头，不仅在序列的末尾，还可以有效地插入和删除元素。但是，与向量不同，deques不能保证将其所有元素存储在连续的存储位置：通过偏移指向另一个元素的指针来访问deque中的元素会导致未定义的行为。

向量和deques都提供了一个非常相似的接口，可以用于类似的目的，但在内部它们的工作方式却截然不同：虽然向量使用一个需要偶尔重新分配以供增长的数组，但deque的元素可以分散在不同的存储块中，容器保留必要的信息。在内部进行格式化，以在恒定时间内直接访问其任何元素，并使用统一的顺序接口（通过迭代器）。因此，deques的内部比向量复杂一些，但这使得它们在某些情况下更有效地增长，特别是在重新分配变得更昂贵的非常长的序列中。

对于频繁插入或删除除开始或结束位置以外的元素的操作，deques的性能比list和forward list更差，并且具有的迭代器和引用的一致性也比list和forward list差。

## 作业要求
1. 实现给定的双端队列头文件,不限制实现方式(禁止使用STL中的数据结构)
```c++
class mydeque {
public:
  mydeque();
  ~mydeque();

  // Add element at the end
  void push_back(int num);
  // Add element at the front
  void push_front(int num);
  // Insert elements at pos
  void insert(myIterator& pos, int num);
  // Erase elements
  void earse(myIterator& pos);
  // Clear content
  void clear();


  // Access element
  int& operator[](int pos);
  // Access first element
  int& front();
  // Access last element
  int& back();

  // Return iterator to beginning
  myIterator begin();
  // Return iterator to end
  myIterator end();
  // Return reverse iterator to reverse beginning
  myIterator rbegin();
  // Return reverse iterator to reverse end
  myIterator rend();
};

class myIterator() {
public:
  myIterator& operator= (const myIterator &iter);
  myIterator& operator!= (const myIterator &iter);
  myIterator& operator== (const myIterator &iter);
  myIterator& operator++ ();
  myIterator& operator++ (int);
  myIterator& operator+ (int num);
}
```


## 评分细则
1. 打包上传工程, 服务器统一测试代码的效率,根据运行效率评分
1. 能使用`make`编译，`./benchmark`运行程序
1. 程序正确运行,没有崩溃,内存泄露等错误(60分)
1. 程序运行效率达到基准实验的80%(90分)
1. 程序运行效率达到基准实验的100%(100分)

## mydeque工程目录结构
```
mydeque/
├── include
│   ├── mydeque.hpp
│   └── myIterator.hpp
├── Makefile
├── README.md
└── src
    ├── main.cpp
    ├── mydeque.cpp
    └── myIterator.cpp
```

允许根据需求增加代码文件和函数,但不能删除已有的代码文件和函数,最终能通过`make`编译成功.
不允许修改 `main.cpp`.

## 提交作业
1. 提交前先`make clean`, 删除不必要的中间文件
2. 打包整个工程,命名为 `mydeque.zip`

## 环境
Ubuntu 16.04 及以上版本

## 安装 Google Benchmark 依赖库
本次大作业需要先安装 Google Benchmark 库到系统文件中.
打包工程中请勿包含该文件夹
```sh
$ cd ~
# Check out the library.
$ git clone https://github.com/google/benchmark.git
# Benchmark requires Google Test as a dependency. Add the source tree as a subdirectory.
$ git clone https://github.com/google/googletest.git benchmark/googletest
# Make a build directory to place the build output.
$ mkdir build && cd build
# Generate a Makefile with cmake.
# Use cmake -G <generator> to generate a different file type.
$ cmake ../benchmark -DCMAKE_BUILD_TYPE=Release
# Build the library.
$ make
# run the tests to check the build
$ make test
# install the library globally
$ sudo make install
```

## 运行基准实验
```sh
# 在mydeque下
$ make
$ ./benchmark
```

## 基准实验输出样例
```
2019-06-12 10:38:24
Running ./benchmark
Run on (4 X 3500 MHz CPU s)
CPU Caches:
  L1 Data 32K (x4)
  L1 Instruction 32K (x4)
  L2 Unified 256K (x4)
  L3 Unified 6144K (x1)
Load Average: 0.51, 0.66, 0.86
***WARNING*** CPU scaling is enabled, the benchmark real time measurements may be noisy and will incur extra overhead.
-------------------------------------------------------------------
Benchmark                         Time             CPU   Iterations
-------------------------------------------------------------------
BM_deqempty                    75.7 ns         75.7 ns      9213578
BM_deqPushBack/32               103 ns          103 ns      6737622
BM_deqPushBack/65536         210187 ns       210185 ns         3455
BM_deqPushBack/4194304     13316903 ns     13317165 ns           54
BM_deqTraverse/32               514 ns          515 ns      1357530
BM_deqTraverse/65536          25646 ns        25546 ns        27908
BM_deqTraverse/4194304      1618940 ns      1618144 ns          444
BM_deqRtraverse/32              526 ns          527 ns      1342739
BM_deqRtraverse/65536         26276 ns        26194 ns        26929
BM_deqRtraverse/4194304     1611323 ns      1610737 ns          435
BM_deqInsert/32/512           35485 ns        35487 ns        19763
BM_deqInsert/65536/512      1372201 ns      1372240 ns          509
BM_deqInsert/4194304/512  215611151 ns    215615403 ns            3
BM_deqErase/512/512           29732 ns        29737 ns        23504
BM_deqErase/65536/512       1422695 ns      1422724 ns          491
BM_deqErase/4194304/512   218809594 ns    218806390 ns            3
BM_deqAt/32/128                1262 ns         1266 ns       555180
BM_deqAt/65536/128             1516 ns         1419 ns       491989
```
