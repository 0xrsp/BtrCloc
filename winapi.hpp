#pragma once

#include "pch.hpp"

struct win32_Mapped_File {
  s8* view;
  HANDLE handle;
  HANDLE map_handle;
  u32 size;
};

typedef void(*File_Cb)(const s8* filename);

bool32 win32_MapFileForRead(const s8* filename,
  win32_Mapped_File* out);
void win32_CloseMapped(const win32_Mapped_File* file);
void win32_WalkDirTree(const s8* dir, File_Cb cb);

void InitHighResTimer();
u64 GetHighResTimestamp();
u64 GetClockFreq();
