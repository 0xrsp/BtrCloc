#include "winapi.hpp"
#include "cliargs.cpp"

enum Enum_File_Type : s32 {
  FILETYPE_UNKNOWN = 0,
  //data formats
  FILETYPE_XML,
  FILETYPE_JSON,
  FILETYPE_TOML,
  FILETYPE_YAML,
  FILETYPE_PROPERTIES,
  FILETYPE_INI,

  //c
  FILETYPE_H_HEADER,
  FILETYPE_C_SRC,
  FILETYPE_CXX_SRC,
  FILETYPE_ASM,

  //random langs
  FILETYPE_PYTHON,
  FILETYPE_PHP,
  FILETYPE_CSHARP,
  FILETYPE_GOLANG,
  FILETYPE_RUST,
  FILETYPE_RUBY,
  FILETYPE_ZIG,
  FILETYPE_SWIFT,
  FILETYPE_JAVA,
  FILETYPE_GRADLE,
  FILETYPE_KOTLIN,
  FILETYPE_HASKELL,
  FILETYPE_JULIA,

  //web stuff
  FILETYPE_JAVASCRIPT,
  FILETYPE_TYPESCRIPT,
  FILETYPE_HTML,
  FILETYPE_CSS,
  FILETYPE_VUE,
  FILETYPE_SVELTE,

  //other
  FILETYPE_BAT,
  FILETYPE_POWERSHELL,
  FILETYPE_MARKDOWN,
  FILETYPE_BASH,

  FILETYPES_COUNT,
};

#define FILETYPE_EXT_MACRO(ext,s,t) \
  else if (strcmp((ext), (s)) == 0) { \
    return t; \
  }

#define FILETYPE_NAME_MACRO(t,n) \
  case t: \
    return (n)

typedef bool(*Line_Proc)(std::string_view);

#include "lineprocs.cpp"

static Line_Proc LookupLineProc(Enum_File_Type filetype) {
  //TODO: Add more line procs
  switch (filetype) {
  case FILETYPE_H_HEADER:
  case FILETYPE_C_SRC:
  case FILETYPE_CXX_SRC:
  case FILETYPE_JAVA:
  case FILETYPE_CSHARP:
    return CstyleLineProc;
  default:
    return nullptr;
  }
}

//NOTE: Increase this if longer exts are used
#define MAX_EXT_LENGTH 12

struct Table_Entry {
  s8 key[MAX_EXT_LENGTH];
  Enum_File_Type value;
};

//NOTE: Increase table size when more are added
#define FILETYPE_TABLE_SIZE 256
static Table_Entry filetype_table[FILETYPE_TABLE_SIZE];

static u32 fmix32(u32 h)
{
  h ^= h >> 16;
  h *= 0x85ebca6b;
  h ^= h >> 13;
  h *= 0xc2b2ae35;
  h ^= h >> 16;
  return h;
}

static u32 ComputeExtIndex(const s8* ext) {
  u32 hash = (ext[0] |
    (ext[1] << 8) |
    (ext[2] << 16) |
    (ext[3] << 24));
  return fmix32(hash) % FILETYPE_TABLE_SIZE;
}

static void PutExtInTable(const s8* ext, Enum_File_Type filetype) {
  u32 idx = ComputeExtIndex(ext);
  while (filetype_table[idx].value != FILETYPE_UNKNOWN) {
    if (strncmp(filetype_table[idx].key, ext, MAX_EXT_LENGTH) == 0) {
      assert(0 && "Ext table already contains entry");
      return;
    }
    idx = (idx + 1) % FILETYPE_TABLE_SIZE;
  }

  strcpy(filetype_table[idx].key, ext);
  filetype_table[idx].value = filetype;
}

static Enum_File_Type LookupExtInTable(const s8* ext) {
  u32 idx = ComputeExtIndex(ext);
  while (strncmp(filetype_table[idx].key, ext, MAX_EXT_LENGTH) != 0) {
    if (filetype_table[idx].value == FILETYPE_UNKNOWN) {
      return FILETYPE_UNKNOWN;
    }
    idx = (idx + 1) % FILETYPE_TABLE_SIZE;
  }
  return filetype_table[idx].value;
}

static void BuildExtHashTable() {
  PutExtInTable("asm", FILETYPE_ASM);
  PutExtInTable("bash", FILETYPE_BASH);
  PutExtInTable("bat", FILETYPE_BAT);
  PutExtInTable("cpp", FILETYPE_CXX_SRC);
  PutExtInTable("cxx", FILETYPE_CXX_SRC);
  PutExtInTable("c++", FILETYPE_CXX_SRC);
  PutExtInTable("cc", FILETYPE_CXX_SRC);
  PutExtInTable("json", FILETYPE_JSON);
  PutExtInTable("toml", FILETYPE_TOML);
  PutExtInTable("properties", FILETYPE_PROPERTIES);
  PutExtInTable("ini", FILETYPE_INI);
  PutExtInTable("hpp", FILETYPE_CXX_SRC);
  PutExtInTable("h", FILETYPE_H_HEADER);
  PutExtInTable("s", FILETYPE_ASM);
  PutExtInTable("py", FILETYPE_PYTHON);
  PutExtInTable("cs", FILETYPE_CSHARP);
  PutExtInTable("go", FILETYPE_GOLANG);
  PutExtInTable("rs", FILETYPE_RUST);
  PutExtInTable("rb", FILETYPE_RUBY);
  PutExtInTable("swift", FILETYPE_SWIFT);
  PutExtInTable("gradle", FILETYPE_GRADLE);
  PutExtInTable("java", FILETYPE_JAVA);
  PutExtInTable("kt", FILETYPE_KOTLIN);
  PutExtInTable("kts", FILETYPE_KOTLIN);
  PutExtInTable("hs", FILETYPE_HASKELL);
  PutExtInTable("jl", FILETYPE_JULIA);
  PutExtInTable("js", FILETYPE_JAVASCRIPT);
  PutExtInTable("ts", FILETYPE_TYPESCRIPT);
  PutExtInTable("jsx", FILETYPE_JAVASCRIPT);
  PutExtInTable("tsx", FILETYPE_TYPESCRIPT);
  PutExtInTable("html", FILETYPE_HTML);
  PutExtInTable("hbs", FILETYPE_HTML);
  PutExtInTable("css", FILETYPE_CSS);
  PutExtInTable("scss", FILETYPE_CSS);
  PutExtInTable("sass", FILETYPE_CSS);
  PutExtInTable("less", FILETYPE_CSS);
  PutExtInTable("svelte", FILETYPE_SVELTE);
  PutExtInTable("vue", FILETYPE_VUE);
  PutExtInTable("ps1", FILETYPE_POWERSHELL);
  PutExtInTable("md", FILETYPE_MARKDOWN);
  PutExtInTable("sh", FILETYPE_BASH);
  PutExtInTable("xml", FILETYPE_XML);
  PutExtInTable("yaml", FILETYPE_YAML);
  PutExtInTable("yml", FILETYPE_YAML);
  PutExtInTable("zig", FILETYPE_ZIG);
}

static const s8* FileTypeToStr(Enum_File_Type file_type) {
  switch (file_type) {
    FILETYPE_NAME_MACRO(FILETYPE_XML, "XML");
    FILETYPE_NAME_MACRO(FILETYPE_JSON, "JSON");
    FILETYPE_NAME_MACRO(FILETYPE_TOML, "TOML");
    FILETYPE_NAME_MACRO(FILETYPE_YAML, "YAML");
    FILETYPE_NAME_MACRO(FILETYPE_PROPERTIES, "Properties");
    FILETYPE_NAME_MACRO(FILETYPE_INI, "INI");

    FILETYPE_NAME_MACRO(FILETYPE_H_HEADER, "C/C++ Header");
    FILETYPE_NAME_MACRO(FILETYPE_C_SRC, "C");
    FILETYPE_NAME_MACRO(FILETYPE_CXX_SRC, "C++");
    FILETYPE_NAME_MACRO(FILETYPE_ASM, "ASM");

    FILETYPE_NAME_MACRO(FILETYPE_PYTHON, "Python");
    FILETYPE_NAME_MACRO(FILETYPE_CSHARP, "C#");
    FILETYPE_NAME_MACRO(FILETYPE_GOLANG, "Go");
    FILETYPE_NAME_MACRO(FILETYPE_RUST, "Rust");
    FILETYPE_NAME_MACRO(FILETYPE_RUBY, "Ruby");
    FILETYPE_NAME_MACRO(FILETYPE_ZIG, "Zig");
    FILETYPE_NAME_MACRO(FILETYPE_SWIFT, "Swift");
    FILETYPE_NAME_MACRO(FILETYPE_GRADLE, "Gradle");
    FILETYPE_NAME_MACRO(FILETYPE_JAVA, "Java");
    FILETYPE_NAME_MACRO(FILETYPE_KOTLIN, "Kotlin");
    FILETYPE_NAME_MACRO(FILETYPE_HASKELL, "Haskell");
    FILETYPE_NAME_MACRO(FILETYPE_JULIA, "Julia");

    FILETYPE_NAME_MACRO(FILETYPE_JAVASCRIPT, "JavaScript");
    FILETYPE_NAME_MACRO(FILETYPE_TYPESCRIPT, "TypeScript");
    FILETYPE_NAME_MACRO(FILETYPE_HTML, "HTML");
    FILETYPE_NAME_MACRO(FILETYPE_CSS, "CSS");
    FILETYPE_NAME_MACRO(FILETYPE_SVELTE, "Svelte");
    FILETYPE_NAME_MACRO(FILETYPE_VUE, "Vue");
    FILETYPE_NAME_MACRO(FILETYPE_BAT, "Batch");
    FILETYPE_NAME_MACRO(FILETYPE_POWERSHELL, "PowerShell");
    FILETYPE_NAME_MACRO(FILETYPE_MARKDOWN, "Markdown");
    FILETYPE_NAME_MACRO(FILETYPE_BASH, "Bash");
  default:
    return "Unknown";
  }
}

struct File_Result {
  s32 code = 0;
  s32 blank = 0;
  s32 comment = 0;
};

struct Job_Data {
  s8* filename;
  Enum_File_Type type;
  File_Result result;
};

struct Job_Span {
  Job_Data** begin;
  s32 count;
};

static Cli_Args args{};
static std::vector<Job_Data*> jobs;

static void DivideJobs(Job_Span* spans, s32 groups) {
  s32 total = (s32)jobs.size();
  s32 base = total / groups;
  s32 rem = total % groups;

  s32 off = 0;
  for (s32 i = 0; i < groups; ++i) {
    s32 count = base + (i < rem ? 1 : 0);
    spans[i].begin = jobs.data() + off;
    spans[i].count = count;
    off += count;
  }
}

static const s8* GetFileExt(const s8* filename) {
  size_t len = strlen(filename);
  if (len == 0 ||
    filename[len - 1] == '.') {
    return nullptr;
  }
  for (s32 i = len - 1; i >= 0; --i) {
    if (filename[i] == '.') {
      return filename + i + 1;
    }
  }

  return nullptr;
}

static bool IsWhitespace(s8 c) {
  return c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == '\f' || c == '\v';
}

static bool IsLineBlank(std::string_view line) {
  for (s8 c : line) {
    if (!IsWhitespace(c)) {
      return 0;
    }
  }
  return 1;
}

static void FileWorker(Job_Data* job) {
  win32_Mapped_File file{};
  if (!win32_MapFileForRead(job->filename, &file)) {
    return;
  }

  // TODO: File is memory mapped there is no 
  // text decoding done

  size_t last_line = 0;
  for (size_t i = 1; i < file.size; ++i) {
    s8 c = file.view[i];
    if (//file.view[i - 1] == '\r' &&
      c == '\n') {
      std::string_view line{ file.view + last_line, i - last_line };
      //printf("Line: %.*s\n", SV_PRINTF_ARG(line));
      if (IsLineBlank(line)) {
        job->result.blank++;
      }
      else {
        Line_Proc lp = LookupLineProc(job->type);
        if (lp && lp(line))
        {
          job->result.comment++;
        }
        else {
          job->result.code++;
        }
      }
      last_line = i + 1;
    }
  }

  LineProcEofCb();

  win32_CloseMapped(&file);
}

static void CreateFileJob(const s8* filename) {
  const s8* ext = GetFileExt(filename);
  if (!ext) {
    return;
  }
  Enum_File_Type type = LookupExtInTable(ext);
  if (type == FILETYPE_UNKNOWN) {
    return;
  }

  Job_Data* job = new Job_Data{};
  job->filename = _strdup(filename);
  job->type = type;
  jobs.push_back(job);
}

static void MergeResult(File_Result* dest, File_Result x) {
  dest->code += x.code;
  dest->blank += x.blank;
  dest->comment += x.comment;
}

static void WorkerThread(Job_Span jobs) {
  for (s32 i = 0; i < jobs.count; ++i) {
    FileWorker(jobs.begin[i]);
  }
}

s32 main(s32 argc, s8* argv[]) {
  BuildExtHashTable();
  InitHighResTimer();
  win32_LoadImports();

  ParseArgs(&args, argc, argv);
  //NOTE: Debug arg
  //args.directory = ".";

  win32_WalkDirTree(args.directory, CreateFileJob);

  Job_Span* spans = new Job_Span[args.threads];
  DivideJobs(spans, args.threads);

  std::vector<std::thread> workers{};
  for (s32 id = 0; id < args.threads; ++id) {
    workers.emplace_back(WorkerThread, spans[id]);
  }
  for (std::thread& t : workers) {
    t.join();
  }
  //delete[] spans;

  File_Result results[FILETYPES_COUNT] = {};
  for (Job_Data* job : jobs) {
    MergeResult(&results[job->type], job->result);
  }

  // TODO: Cleanup printing logic
  printf("Type\t\tCode\tBlank\tComment\n");
  File_Result sum{};
  for (s32 typei = FILETYPE_UNKNOWN + 1; typei < FILETYPES_COUNT; ++typei) {
    File_Result result = results[typei];
    if (result.blank + result.code + result.comment > 0) {
      MergeResult(&sum, result);
      Enum_File_Type type = (Enum_File_Type)typei;
      const s8* str = FileTypeToStr(type);
      s32 tabs = 16 - strlen(str);
      printf("%s%*s%d\t%d\t%d\n", str, tabs, "", result.code, result.blank, result.comment);
    }
  }
  printf("---------------------------------------\n");
  printf("SUM\t\t%d\t%d\t%d", sum.code, sum.blank, sum.comment);

  return 0;
}
