#include "file_io.hpp"

win32_Mapped_File win32_MapFileForRead(const s8* filename) {
  Mapped_File file{};

  file.Handle =
    CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, nullptr,
      OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);

  file.Size = GetFileSize(file.Handle, NULL);

  file.MapHandle =
    CreateFileMappingA(file.Handle, nullptr, PAGE_READONLY, 0, 0, nullptr);

  file.View = (s8*)MapViewOfFile(file.MapHandle, FILE_MAP_READ, 0, 0, 0);
}

void win32_CloseMapped(win32_Mapped_File* file) {
  UnmapViewOfFile(file.BasePtr);
  CloseHandle(file.MappingHandle);
  CloseHandle(file.FileHandle);
}

