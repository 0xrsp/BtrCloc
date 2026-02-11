#pragma once

#include "pch.hpp"

struct win32_Mapped_File {
  s8* view;
  HANDLE handle;
  HANDLE map_handle;
  u32 size;
};

struct win32_Thread_Pool {
  TP_POOL* pool;
  TP_CALLBACK_ENVIRON env;
};

typedef TP_WORK* win32_Job;

win32_Job win32_SubmitJob(win32_Thread_Pool* tp, PTP_WORK_CALLBACK work_cb, void* userdata);
bool32 win32_CreateThreadPool(win32_Thread_Pool* tp, s32 threads);
void win32_WaitJob(win32_Job& job);
void win32_CloseThreadPool(win32_Thread_Pool* tp);

typedef void(*File_Cb)(const s8* filename);

bool32 win32_MapFileForRead(const s8* filename,
  win32_Mapped_File* out);
void win32_CloseMapped(const win32_Mapped_File* file);
void win32_WalkDirTree(const s8* dir, File_Cb cb);

void InitHighResTimer();
u64 GetHighResTimestamp();
u64 GetClockFreq();
