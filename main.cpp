#include "winapi.hpp"
#include "cliargs.cpp"

// TODO: Add more file types
enum Enum_File_Type : s32 {
  FILETYPE_UNKNOWN = 0,
  FILETYPE_H_HEADER,
  FILETYPE_HPP_HEADER,
  FILETYPE_C_SRC,
  FILETYPE_CXX_SRC,
  FILETYPE_JAVA,
  FILETYPES_COUNT,
};

#define FILETYPE_EXT_MACRO(ext,s,t) \
  else if (strcmp((ext), (s)) == 0) { \
    return t; \
  }

#define FILETYPE_NAME_MACRO(t,n) \
  case t: \
    return (n)


// TODO: Could use some kind of precomputed table
static Enum_File_Type MapExtToFileType(const s8* ext) {
  if (strcmp(ext, "c") == 0) {
    return FILETYPE_C_SRC;
  }
  FILETYPE_EXT_MACRO(ext, "cpp", FILETYPE_CXX_SRC)
    FILETYPE_EXT_MACRO(ext, "hpp", FILETYPE_HPP_HEADER)
    FILETYPE_EXT_MACRO(ext, "h", FILETYPE_H_HEADER)
    FILETYPE_EXT_MACRO(ext, "java", FILETYPE_JAVA)
  else {
    return FILETYPE_UNKNOWN;
  }
}

static const s8* FileTypeToStr(Enum_File_Type file_type) {
  switch (file_type) {
    FILETYPE_NAME_MACRO(FILETYPE_H_HEADER, "C/C++ Header");
    FILETYPE_NAME_MACRO(FILETYPE_HPP_HEADER, "C++ Header");
    FILETYPE_NAME_MACRO(FILETYPE_C_SRC, "C");
    FILETYPE_NAME_MACRO(FILETYPE_CXX_SRC, "C++");
    FILETYPE_NAME_MACRO(FILETYPE_JAVA, "Java");
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

static void FileWorker(Job_Data* job) {
  win32_Mapped_File file{};
  if (!win32_MapFileForRead(job->filename, &file)) {
    return;
  }

  s32 last_line = 0;
  for (s32 i = 0; i < (s32)file.size; ++i) {
    s8 c = file.view[i];
    if (c == '\r') continue;
    if (c == '\n') {
      job->result.code++;
      // TODO: Check if line was only whitespace or all comment
      last_line = i + 1;
    }
  }

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
  for (s32 typei = FILETYPE_UNKNOWN + 1; typei < FILETYPES_COUNT; ++typei) {
    File_Result result = results[typei];
    Enum_File_Type type = (Enum_File_Type)typei;
    const s8* str = FileTypeToStr(type);
    s32 tabs = 16 - strlen(str);
    printf("%s%*s%d\t%d\t%d\n", str, tabs, "", result.code, result.blank, result.comment);
  }

  return 0;
}
