#include <benchmark/benchmark.h>
#include <algorithm>
#include <random>

#include "mydeque.hpp"

long getRandom(long from=0, long to=65536) {
  return (rand() % (to-from+1)) + from;
}

mydeque constructRandomDeque(long num) {
  mydeque deq;
  for (long i = 0; i < num; i++) {
    deq.push_back(i);
  }
  return deq;
}

static void BM_deqempty(benchmark::State& state) {
  for (auto _ : state) { mydeque deq; }
}


static void BM_deqPushBack(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    for (long i = 0; i < state.range(0); i++) {
      deq.push_back(i);
    }
  }
}

static void BM_deqPushfront(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    for (long i = 0; i < state.range(0); i++) {
      deq.push_front(i);
    }
  }
}

static void BM_deqTraverse(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    state.PauseTiming();
    deq = constructRandomDeque(state.range(0));
    state.ResumeTiming();
    for (auto it = deq.begin(); it != deq.end(); it++) {}
  }
}

static void BM_deqRtraverse(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    state.PauseTiming();
    deq = constructRandomDeque(state.range(0));
    state.ResumeTiming();
    for (auto it = deq.rbegin(); it != deq.rend(); it++) {}
  }
}

static void BM_deqInsert(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    state.PauseTiming();
    deq = constructRandomDeque(state.range(0));
    state.ResumeTiming();
    for (int j = 0; j < state.range(1); ++j)
      deq.insert(deq.begin()+getRandom(0, deq.size()-1), getRandom());
  }
}

static void BM_deqErase(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    state.PauseTiming();
    deq = constructRandomDeque(state.range(0));
    int max = state.range(0) > state.range(1) ? state.range(0) : state.range(1);
    state.ResumeTiming();
    for (long j = 0; j < state.range(1); ++j) {
        if (deq.size()==0) break;
        deq.erase(deq.begin()+getRandom(0, max-1-j));
    }
  }
}

static void BM_deqAt(benchmark::State& state) {
  mydeque deq;
  for (auto _ : state) {
    state.PauseTiming();
    deq = constructRandomDeque(state.range(0));
    state.ResumeTiming();
    for (long j = 0; j < state.range(1); ++j)
      deq[getRandom(0, state.range(0)-1)];
  }
}

// static void BM_deqSort(benchmark::State& state) {
//   mydeque deq;
//   for (auto _ : state) {
//     state.PauseTiming();
//     deq = constructRandomDeque(state.range(0));
//     state.ResumeTiming();
//     std::sort(deq.begin(),deq.end());
//   }
// }

// static void BM_deqShuffle(benchmark::State& state) {
//   mydeque deq;
//   for (auto _ : state) {
//     state.PauseTiming();
//     deq = constructRandomDeque(state.range(0));
//     state.ResumeTiming();
//     std::random_shuffle(deq.begin(),deq.end());
//   }
// }


// Register the function as a benchmark
BENCHMARK(BM_deqempty);
BENCHMARK(BM_deqPushBack)->Arg(32)->Arg(65536)->Arg(1L<<22);
BENCHMARK(BM_deqTraverse)->Arg(32)->Arg(65536)->Arg(1L<<22);
BENCHMARK(BM_deqRtraverse)->Arg(32)->Arg(65536)->Arg(1L<<22);
BENCHMARK(BM_deqInsert)->Args({32, 512})->Args({65536, 512})->Args({1L<<22, 512});
BENCHMARK(BM_deqErase)->Args({512, 512})->Args({65536, 512})->Args({1L<<22, 512});
BENCHMARK(BM_deqAt)->Args({32, 128})->Args({65536, 128});


// BENCHMARK(BM_deqPushfront)->RangeMultiplier(32)->Range(8L, 8L<<20);
// BENCHMARK(BM_deqTraverse)->RangeMultiplier(32)->Range(8L, 8L<<20);
// BENCHMARK(BM_deqRtraverse)->RangeMultiplier(32)->Range(8L, 8L<<20);
// BENCHMARK(BM_deqInsert)->Ranges({{32L, 8L<<22}, {128, 512}});
// BENCHMARK(BM_deqErase)->Ranges({{32L, 8L<<22}, {128, 512}});
// BENCHMARK(BM_deqAt)->Ranges({{32L, 8L<<22}, {128, 512}});
// BENCHMARK(BM_deqSort)->RangeMultiplier(32)->Range(8L, 8L<<20);
// BENCHMARK(BM_deqShuffle)->RangeMultiplier(32)->Range(8L, 8L<<22);

BENCHMARK_MAIN();