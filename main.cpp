#include "perf_clock.hpp"

static void PrintOptionsHelp() {

}

static void PrintUsage(const s8* exe_name) {
  printf("%s [-Options] <Source file> [Output file]\n", exe_name);
  PrintOptionsHelp();
}

s32 main(s32 argc, s8* argv[]) {
  InitHighResTimer();
  return 0;
}
