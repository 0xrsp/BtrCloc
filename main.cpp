#include "winapi.hpp"
#include "cli_app.cpp"

//types
enum Enum_File_Type : s32 {
  FILETYPE_UNKNOWN = 0,
  FILETYPE_H_HEADER,
  FILETYPE_HPP_HEADER,
  FILETYPE_C_SRC,
  FILETYPE_CXX_SRC,
  FILETYPE_JAVA,
  FILETYPES_COUNT,
};

struct File_Result {
  s32 code = 0;
  s32 blank = 0;
  s32 comment = 0;
};

struct Job_Data {
  s8* filename;
  Enum_File_Type type;
  File_Result result;
  win32_Job wjob;
};

//data
static thread_local s8 file_read_buf[4 * 1024];//4kb

static win32_Thread_Pool workers{};
static Cli_Args args{};
static std::vector<Job_Data*> jobs;

//functions

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

static Enum_File_Type MapExtToFileType(const s8* ext) {
  if (strcmp(ext, "c") == 0) {
    return FILETYPE_C_SRC;
  }
  else if (strcmp(ext, "cpp") == 0) {
    return FILETYPE_CXX_SRC;
  }
  else if (strcmp(ext, "hpp") == 0) {
    return FILETYPE_HPP_HEADER;
  }
  else if (strcmp(ext, "h") == 0) {
    return FILETYPE_H_HEADER;
  }
  else if (strcmp(ext, "java") == 0) {
    return FILETYPE_JAVA;
  }
  else {
    return FILETYPE_UNKNOWN;
  }
}

static const s8* FileTypeToStr(Enum_File_Type file_type) {
  switch (file_type) {
  default:
    return "Unknown";
  case FILETYPE_H_HEADER:
    return "C/C++ Header";
  case FILETYPE_HPP_HEADER:
    return "C++ Header";
  case FILETYPE_C_SRC:
    return "C";
  case FILETYPE_CXX_SRC:
    return "C++";
  case FILETYPE_JAVA:
    return "Java";
  }
}

static void FileWorker(TP_CALLBACK_INSTANCE* tp, void* userdata, TP_WORK* work) {
  UNUSED_PARAM(tp);
  UNUSED_PARAM(work);
  Job_Data* job = (Job_Data*)userdata;

  win32_Mapped_File file{};
  if (!win32_MapFileForRead(job->filename, &file)) {
    return;
  }

  // TODO: Read lines
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

  //cleanup
  win32_CloseMapped(&file);
}

static void SpawnFileJob(const s8* filename) {
  const s8* ext = GetFileExt(filename);
  if (!ext) {
    return;
  }
  Enum_File_Type type = MapExtToFileType(ext);
  if (type == FILETYPE_UNKNOWN) {
    return;
  }

  Job_Data* data = new Job_Data{};
  data->filename = _strdup(filename);
  data->type = type;

  //printf("spawning task: %s\n", filename);
  win32_Job job = win32_SubmitJob(&workers, FileWorker, data);

  if (job) {
    jobs.push_back(data);
  }
  else {
    delete data;
  }
}

static void MergeResult(File_Result* dest, File_Result x) {
  dest->code += x.code;
  dest->blank += x.blank;
  dest->comment += x.comment;
}

s32 main(s32 argc, s8* argv[]) {
  InitHighResTimer();

  //TODO: Testing args
  //ParseArgs(&args, argc, argv);
  args.directory = ".";

  win32_CreateThreadPool(&workers, args.threads);
  win32_WalkDirTree(args.directory, SpawnFileJob);

  File_Result results[FILETYPES_COUNT] = { };

  for (Job_Data* job : jobs) {
    win32_WaitJob(job->wjob);
    job->wjob = nullptr;
  }

  win32_CloseThreadPool(&workers);

  for (Job_Data* job : jobs) {
    //printf("type=%d code=%d\n", job->type, job->result.code);
    MergeResult(&results[job->type], job->result);
  }

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
