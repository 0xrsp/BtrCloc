#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <vector>
#include <unordered_map>
#include <unordered_set> 

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