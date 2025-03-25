#ifndef PIGLET_TYPES_H
#define PIGLET_TYPES_H

// Note: only include this header inside PBG-related headers/source code files.
// Don't include this in any RenderWare, system, etc. files

#ifdef GAMECUBE
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef signed long long s64;

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef float f32;
typedef double f64;
#endif

#ifdef NULL
#undef NULL
#endif
#define NULL 0

#ifdef TRUE
#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
#undef FALSE
#endif
#define FALSE 0

typedef int BOOL;

#ifdef __MWERKS__
#define __declspec(x)
// #define asm
#endif

#define WEAK __declspec(weak)

#endif
