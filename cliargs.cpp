#include "pch.hpp"

#define CLI_DEFAULT_THREADS 6

struct Cli_Args {
  const s8* directory = nullptr;
  s32 threads = CLI_DEFAULT_THREADS;
  bool8 print_time = 0;
  bool8 no_recurse = 0;
  bool8 no_mmap = 0;
};

static void PrintOptionsHelp() {
  printf("Options help:\n");
  printf("\t-nore\t\tDo not recurse into subdirectories\n");
  printf("\t-t<num>\t\tSpecify amount of threads (default:%d)\n", CLI_DEFAULT_THREADS);

  //TODO: -time
  //printf("\t-time\t\tPrint timing information for each file\n");

  //TODO: -nomm
  //printf("\t-nomm\t\tDo not use mmap() to read files\n");
}

static void PrintUsage(const s8* exe_name, bool32 opts_help = 1) {
  printf("%s [-Options] <directory>\n", exe_name);
  if (opts_help) {
    PrintOptionsHelp();
  }
}

static bool32 IsAsciiDigit(s8 c) {
  return c >= '0' && c <= '9';
}

static void ParseArgs(Cli_Args* args, s32 argc, s8* argv[]) {
  for (s32 i = 1; i < argc; ++i) {
    const s8* arg = argv[i];
    if (arg[0] == '-') {
      const s8* arg1 = arg + 1;
      if (arg1[0] == 't' && IsAsciiDigit(arg1[1])) {
        s32 threads = atoi(arg1+1);
        threads = max(1, min(32, threads));
        args->threads = threads;
      }
      else if (strcmp(arg1, "time") == 0) {
        args->print_time = 1;
      }
      else if (strcmp(arg1, "nore") == 0) {
        args->no_recurse = 1;
      }
      else if (strcmp(arg1, "nomm") == 0) {
        args->no_mmap = 1;
      }
      else {
        printf("Unrecognized option: %s\n", arg);
        PrintOptionsHelp();
        exit(1);
      }
    }
    else {
      if (args->directory) {
        printf("Invalid argument: %s\n", arg);
        PrintUsage(argv[0], 0);
        exit(1);
      }
      args->directory = arg;
    }
  }

  if (!args->directory) {
    printf("Missing <directory>\n");
    PrintUsage(argv[0]);
    exit(1);
  }
}



