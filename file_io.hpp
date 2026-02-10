#pragma once

#include "pch.hpp"

struct win32_Mapped_File {
  HANDLE Handle;
  HANDLE MapHandle;
  u32 Size;
  bool8 Error;
  s8* View;
};

win32_Mapped_File win32_MapFileForRead(const s8* filename);
void win32_CloseMapped(win32_Mapped_File* file);