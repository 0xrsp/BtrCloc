@echo off

set CONFIG=%1
if "%CONFIG%"=="" set CONFIG=debug
set OUT_EXE=bloc.exe

echo Building...

set CL_FLAGS=/EHsc /std:c++20 /W4
set PC_HEADER=pch.hpp
set DEBUG_FLAGS=/Zi /Od /D_DEBUG /MDd
set REL_FLAGS=/Os /DNDEBUG /MD
set SRC_FILES=main.cpp winapi.cpp
set CC=clang-cl.exe

if /I "%CONFIG%"=="debug" (
  %CC% %CL_FLAGS% %DEBUG_FLAGS% %SRC_FILES% /Fe:%OUT_EXE%
) else if /I "%CONFIG%"=="release" (
  %CC% %CL_FLAGS% %REL_FLAGS% %SRC_FILES% /Fe:%OUT_EXE%
) else (
  echo Bad argument, must be debug or release %CONFIG%
  exit /b 1
)

echo Done.
