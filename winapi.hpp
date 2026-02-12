#pragma once

#include "pch.hpp"

struct win32_Mapped_File {
  s8* view;
  HANDLE handle;
  HANDLE map_handle;
  u32 size;
};

typedef void(*File_Cb)(s8* filename);

void win32_LoadImports();

bool32 win32_MapFileForRead(const s8* filename,
  win32_Mapped_File* out);
void win32_CloseMapped(const win32_Mapped_File* file);
void win32_WalkDirTree(s8* dir, File_Cb cb, bool32 recurse);

//winnt stuff
typedef
NTSTATUS
NTAPI
(*NtQueryDirectoryFile_t)(
  IN HANDLE               FileHandle,
  IN HANDLE               Event OPTIONAL,
  IN PIO_APC_ROUTINE      ApcRoutine OPTIONAL,
  IN PVOID                ApcContext OPTIONAL,
  OUT PIO_STATUS_BLOCK    IoStatusBlock,
  OUT PVOID               FileInformation,
  IN ULONG                Length,
  IN FILE_INFORMATION_CLASS FileInformationClass,
  IN BOOLEAN              ReturnSingleEntry,
  IN PUNICODE_STRING      FileMask OPTIONAL,
  IN BOOLEAN              RestartScan);

struct FILE_DIRECTORY_INFORMATION {
  u32 next_entry_off;
  u32 file_index;
  s64 creation_time;
  s64 access_time;
  s64 write_time;
  s64 change_time;
  s64 eof;
  s64 alloc_size;
  u32 attribs;
  u32 name_length;
  wchar_t name[1];
};

