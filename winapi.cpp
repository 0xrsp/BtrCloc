#include "winapi.hpp"

win32_Job win32_SubmitJob(win32_Thread_Pool* tp, PTP_WORK_CALLBACK work_cb, void* userdata) {
  TP_WORK* work = CreateThreadpoolWork(work_cb, userdata, &tp->env);
  if (work) {
    SubmitThreadpoolWork(work);
  }
  return work;
}

void win32_WaitJob(win32_Job& job) {
  if (job) {
    WaitForThreadpoolWorkCallbacks(job, 0);
    CloseThreadpoolWork(job);
    job = nullptr;
  }
}

bool32 win32_CreateThreadPool(win32_Thread_Pool* tp, s32 threads) {
  InitializeThreadpoolEnvironment(&tp->env);
  tp->pool = CreateThreadpool(nullptr);
  SetThreadpoolThreadMinimum(tp->pool, threads);
  SetThreadpoolThreadMaximum(tp->pool, threads);
  SetThreadpoolCallbackPool(&tp->env, tp->pool);
  return 0;
}

void win32_CloseThreadPool(win32_Thread_Pool* tp) {
  DestroyThreadpoolEnvironment(&tp->env);
  CloseThreadpool(tp->pool);
}

bool32 win32_MapFileForRead(const s8* filename,
  win32_Mapped_File* out) {

  out->handle =
    CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
  if (out->handle == INVALID_HANDLE_VALUE) {
    return 0;
  }

  out->size = GetFileSize(out->handle, NULL);
  if (out->size == INVALID_FILE_SIZE) {
    return 0;
  }

  out->map_handle =
    CreateFileMappingA(out->handle, nullptr, PAGE_READONLY, 0, 0, nullptr);
  if (out->map_handle == INVALID_HANDLE_VALUE) {
    return 0;
  }

  out->view = (s8*)MapViewOfFile(out->map_handle, FILE_MAP_READ, 0, 0, 0);
  return out->view ? 1 : 0;
}

void win32_CloseMapped(const win32_Mapped_File* file) {
  UnmapViewOfFile(file->view);
  CloseHandle(file->map_handle);
  CloseHandle(file->handle);
}

void win32_WalkDirTree(const s8* dir, File_Cb cb) {
  // TODO: Use wchar_t
  WIN32_FIND_DATAA fd;

  s8 search_filter[MAX_PATH];
  sprintf(search_filter, "%s\\*", dir);
  HANDLE handle = FindFirstFileA(search_filter, &fd);
  if (handle == INVALID_HANDLE_VALUE) {
    return;
  }
  do {
    if (strcmp(fd.cFileName, ".") == 0 ||
      strcmp(fd.cFileName, "..") == 0) {
      continue;
    }

    s8 full_path[MAX_PATH];
    sprintf(full_path, "%s\\%s", dir, fd.cFileName);

    if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      win32_WalkDirTree(full_path, cb);
    }
    else {
      cb(full_path);
    }

  } while (FindNextFileA(handle, &fd));

  FindClose(handle);
}

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

