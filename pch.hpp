#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winternl.h>

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include <vector>
#include <thread>
#include <string_view>

typedef char s8;
typedef short s16;
typedef int s32;
typedef long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;

typedef s32 bool32;
typedef s8 bool8;

#define ArrayCount(a) (sizeof(a) / sizeof(a[0]))
#define UNUSED_PARAM(x) (void)x
#define ASSERT_NOT_IMPL assert(0 && "This function has not been implemented")
#define SV_PRINTF_ARG(sv) (s32)(sv).size(),(sv).data()

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
