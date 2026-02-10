@echo off

set CONFIG=%1
if "%CONFIG%"=="" set CONFIG=debug
set OUT_EXE=Compiler.exe

echo Building...

set CL_FLAGS=-std=c++23 ^
 -Wall -Wextra -Wno-deprecated-declarations -Wno-unused-parameter ^
 -fno-exceptions -fno-rtti -fno-unwind-tables -fno-asynchronous-unwind-tables
set PC_HEADER=pch.hpp
set INCLUDE_PCH=-include-pch %PC_HEADER%.pch
set DEBUG_FLAGS=-g -O0 -D_DEBUG
set REL_FLAGS=-O2 -DNDEBUG
set SRC_FILES=main.cpp file_io.cpp perf_clock.cpp
set CC=clang++.exe

if /I "%CONFIG%"=="debug" (
  %CC% %CL_FLAGS% %DEBUG_FLAGS% %INCLUDE_PCH% %SRC_FILES% -o %OUT_EXE%
) else if /I "%CONFIG%"=="pch" (
  %CC% -x c++-header %CL_FLAGS% %DEBUG_FLAGS% %PC_HEADER% -o %PC_HEADER%.pch
) else if /I "%CONFIG%"=="release" (
  %CC% -x c++-header %CL_FLAGS% %REL_FLAGS% %PC_HEADER% -o %PC_HEADER%.pch
  %CC% %CL_FLAGS% %REL_FLAGS% %INCLUDE_PCH% %SRC_FILES% -o %OUT_EXE%
) else (
  echo Bad argument, must be debug, release or pch %CONFIG%
  exit /b 1
)

echo Done.
