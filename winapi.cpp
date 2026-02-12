#include "winapi.hpp"

static NtQueryDirectoryFile_t NtQueryDirectoryFile_imp;

void win32_LoadImports() {
  HMODULE ntdll = LoadLibraryW(L"ntdll.dll");
  NtQueryDirectoryFile_imp = (NtQueryDirectoryFile_t)GetProcAddress(ntdll, "NtQueryDirectoryFile");
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

void win32_WalkDirTree(s8* dir, File_Cb cb, bool32 recurse) {
  s8 dir_info_buf[4 * 1024];//4kb
  s32 dir_length;
  FILE_DIRECTORY_INFORMATION info{};
  u32 entry_offset = 0x0;
  IO_STATUS_BLOCK status;
  HANDLE dhandle = CreateFileA(dir, FILE_LIST_DIRECTORY, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE,
    nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);

  if (dhandle == INVALID_HANDLE_VALUE) {
    goto err_any;
  }

  NtQueryDirectoryFile_imp(dhandle, nullptr, nullptr, nullptr, &status, dir_info_buf, sizeof(dir_info_buf),
    FileDirectoryInformation, 0, nullptr, 1);

  if (!NT_SUCCESS(status.Status)) {
    goto err_close;
  }

  dir_length = strlen(dir);
  do {
    info = *(FILE_DIRECTORY_INFORMATION*)(dir_info_buf + entry_offset);
    wchar_t* name_p = (wchar_t*)(dir_info_buf + entry_offset + offsetof(FILE_DIRECTORY_INFORMATION, name));
    u32 wlength = info.name_length / 2;
    entry_offset += info.next_entry_off;

    if (entry_offset + sizeof(FILE_DIRECTORY_INFORMATION) > sizeof(dir_info_buf)) {
      printf("directory too large to enumerate: %s\n", dir);
      break;
    }

    if (
      wcsncmp(name_p, L".", info.name_length) == 0 ||
      wcsncmp(name_p, L"..", info.name_length) == 0
      ) {
      continue;
    }

    //TODO: Utf-8 stuff here just assuming length is the same
    //s32 length = WideCharToMultiByte(CP_ACP, 0, name_p, wlength, nullptr, 0, nullptr, nullptr);
    s32 length = wlength;

    s32 nullpad = 4;
    // TODO: Pool allocations?
    s8* filepath = (s8*)malloc(dir_length + 1 + length + nullpad);
    filepath[0] = 0;
    strcat(filepath, dir);
    strcat(filepath, "\\");
    length = WideCharToMultiByte(CP_ACP, 0, name_p, wlength, filepath + dir_length + 1, MAX_PATH, nullptr, nullptr);
    memset(filepath + dir_length + 1 + length, 0, nullpad);

    if (info.attribs & FILE_ATTRIBUTE_DIRECTORY) {
      if (recurse) {
        win32_WalkDirTree(filepath, cb, 1);
      }
    }
    else {
      cb(filepath);
    }
  } while (info.next_entry_off != 0);

err_close:
  CloseHandle(dhandle);
err_any:
  free(dir);
}
