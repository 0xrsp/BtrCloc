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
  FILETYPE_HPP_HEADER,
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
  switch (filetype) {
  case FILETYPE_H_HEADER:
  case FILETYPE_HPP_HEADER:
  case FILETYPE_C_SRC:
  case FILETYPE_CXX_SRC:
  case FILETYPE_JAVA:
  case FILETYPE_CSHARP:
    return CstyleLineProc;
    /*
      FILETYPE_XML,
  FILETYPE_JSON,
  FILETYPE_TOML,
  FILETYPE_YAML,
  FILETYPE_PROPERTIES,
  FILETYPE_INI,

  //c
  FILETYPE_ASM,

  //random langs
  FILETYPE_PYTHON,
  FILETYPE_PHP,
  FILETYPE_GOLANG,
  FILETYPE_RUST,
  FILETYPE_RUBY,
  FILETYPE_ZIG,
  FILETYPE_SWIFT,
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

*/

  default:
    return nullptr;

  }
}

static Enum_File_Type MapExtToFileType(const s8* ext) {
  if (strcmp(ext, "c") == 0) {
    return FILETYPE_C_SRC;
  }
  // TODO: this should be some kind of hashtable
  FILETYPE_EXT_MACRO(ext, "xml", FILETYPE_XML)
    FILETYPE_EXT_MACRO(ext, "json", FILETYPE_JSON)
    FILETYPE_EXT_MACRO(ext, "toml", FILETYPE_TOML)
    FILETYPE_EXT_MACRO(ext, "yaml", FILETYPE_YAML)
    FILETYPE_EXT_MACRO(ext, "yml", FILETYPE_YAML)
    FILETYPE_EXT_MACRO(ext, "properties", FILETYPE_PROPERTIES)
    FILETYPE_EXT_MACRO(ext, "ini", FILETYPE_INI)

    FILETYPE_EXT_MACRO(ext, "cpp", FILETYPE_CXX_SRC)
    FILETYPE_EXT_MACRO(ext, "cxx", FILETYPE_CXX_SRC)
    FILETYPE_EXT_MACRO(ext, "c++", FILETYPE_CXX_SRC)
    FILETYPE_EXT_MACRO(ext, "cc", FILETYPE_CXX_SRC)
    FILETYPE_EXT_MACRO(ext, "hpp", FILETYPE_HPP_HEADER)
    FILETYPE_EXT_MACRO(ext, "h", FILETYPE_H_HEADER)
    FILETYPE_EXT_MACRO(ext, "s", FILETYPE_ASM)
    FILETYPE_EXT_MACRO(ext, "asm", FILETYPE_ASM)

    FILETYPE_EXT_MACRO(ext, "py", FILETYPE_PYTHON)
    FILETYPE_EXT_MACRO(ext, "pyi", FILETYPE_PYTHON)
    FILETYPE_EXT_MACRO(ext, "cs", FILETYPE_CSHARP)
    FILETYPE_EXT_MACRO(ext, "go", FILETYPE_GOLANG)
    FILETYPE_EXT_MACRO(ext, "rs", FILETYPE_RUST)
    FILETYPE_EXT_MACRO(ext, "rb", FILETYPE_RUBY)
    FILETYPE_EXT_MACRO(ext, "zig", FILETYPE_ZIG)
    FILETYPE_EXT_MACRO(ext, "swift", FILETYPE_SWIFT)
    FILETYPE_EXT_MACRO(ext, "gradle", FILETYPE_GRADLE)
    FILETYPE_EXT_MACRO(ext, "java", FILETYPE_JAVA)
    FILETYPE_EXT_MACRO(ext, "kt", FILETYPE_KOTLIN)
    FILETYPE_EXT_MACRO(ext, "kts", FILETYPE_KOTLIN)
    FILETYPE_EXT_MACRO(ext, "hs", FILETYPE_HASKELL)
    FILETYPE_EXT_MACRO(ext, "jl", FILETYPE_JULIA)

    FILETYPE_EXT_MACRO(ext, "js", FILETYPE_JAVASCRIPT)
    FILETYPE_EXT_MACRO(ext, "ts", FILETYPE_TYPESCRIPT)
    FILETYPE_EXT_MACRO(ext, "jsx", FILETYPE_JAVASCRIPT)
    FILETYPE_EXT_MACRO(ext, "tsx", FILETYPE_TYPESCRIPT)
    FILETYPE_EXT_MACRO(ext, "html", FILETYPE_HTML)
    FILETYPE_EXT_MACRO(ext, "hbs", FILETYPE_HTML)
    FILETYPE_EXT_MACRO(ext, "css", FILETYPE_CSS)
    FILETYPE_EXT_MACRO(ext, "scss", FILETYPE_CSS)
    FILETYPE_EXT_MACRO(ext, "sass", FILETYPE_CSS)
    FILETYPE_EXT_MACRO(ext, "less", FILETYPE_CSS)
    FILETYPE_EXT_MACRO(ext, "svelte", FILETYPE_SVELTE)
    FILETYPE_EXT_MACRO(ext, "vue", FILETYPE_VUE)

    FILETYPE_EXT_MACRO(ext, "bat", FILETYPE_BAT)
    FILETYPE_EXT_MACRO(ext, "ps1", FILETYPE_POWERSHELL)
    FILETYPE_EXT_MACRO(ext, "md", FILETYPE_MARKDOWN)
    FILETYPE_EXT_MACRO(ext, "sh", FILETYPE_BASH)
    FILETYPE_EXT_MACRO(ext, "bash", FILETYPE_BASH)
  else {
    return FILETYPE_UNKNOWN;
  }
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
    FILETYPE_NAME_MACRO(FILETYPE_HPP_HEADER, "C++ Header");
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
  Enum_File_Type type = MapExtToFileType(ext);
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
  InitHighResTimer();

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
