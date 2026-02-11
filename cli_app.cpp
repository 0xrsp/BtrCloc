#include "pch.hpp"

struct Cli_Args {
  const s8* directory = nullptr;
  s32 threads = 6;
  bool8 print_time = 0;
  bool8 no_recurse = 0;
  bool8 no_mem_map = 0;
};

static void PrintOptionsHelp() {
  printf("Options help:\n");
  printf("\t-time\t\tPrint timing information for each file\n");
  printf("\t-nore\t\tDo not recurse into subdirectories\n");
  printf("\t-nomm\t\tDo not use mmap() to read files\n");
}

static void PrintUsage(const s8* exe_name, bool32 opts_help = 1) {
  printf("%s [-Options] <directory>\n", exe_name);
  if (opts_help) {
    PrintOptionsHelp();
  }
}

static void ParseArgs(Cli_Args* args, s32 argc, s8* argv[]) {
  for (s32 i = 1; i < argc; ++i) {
    const s8* arg = argv[i];
    if (arg[0] == '-') {
      const s8* arg1 = arg + 1;
      if (strcmp(arg1, "time")) {
        args->print_time = 1;
      }
      else if (strcmp(arg1, "nore")) {
        args->no_recurse = 1;
      }
      else if (strcmp(arg1, "nomm")) {
        args->no_mem_map = 1;
      }
      else {
        printf("Unrecognized option: %s\n", arg);
        PrintOptionsHelp();
        exit(1);
      }
    }
    else {
      if (args->directory) {
        printf("Invalid arg: %s\n", arg);
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



