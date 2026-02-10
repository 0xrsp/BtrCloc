#include "perf_clock.hpp"

static u64 clock_freq;

void InitHighResTimer() {
  QueryPerformanceFrequency((LARGE_INTEGER*)&clock_freq);
}

u64 GetHighResTimestamp() {
  u64 ticks_now;
  QueryPerformanceCounter((LARGE_INTEGER*)&ticks_now);
  return ticks_now;
}

u64 GetClockFreq() { return clock_freq; }
