#ifndef RWSDK_RWTYPES_H
#define RWSDK_RWTYPES_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int RwBool;
#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif

typedef float RwReal;
typedef char RwChar;
typedef char RwInt8;
typedef unsigned char RwUInt8;
typedef short RwInt16;
typedef unsigned short RwUInt16;
typedef int RwInt32;
typedef unsigned int RwUInt32;

#ifdef __cplusplus
}
#endif

#endif
