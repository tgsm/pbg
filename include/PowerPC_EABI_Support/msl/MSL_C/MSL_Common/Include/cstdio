#ifndef _MSL_CSTDIO
#define _MSL_CSTDIO

#include "ansi_files.h"
#include "stdarg.h"
#include "stddef.h"

#ifdef __cplusplus
namespace std {
extern "C" {
#endif

#define stdin &__files._stdin
#define stdout &__files._stdout
#define stderr &__files._stderr

// FIXME: FILE
void* fopen(const char* path, const char* mode);
int fclose(void* stream);
size_t fread(void* ptr, size_t size, size_t n, void* stream);
size_t fwrite(const void* ptr, size_t size, size_t n, void* stream);
char* fgets(char* s, int size, void* stream);
int fputs(const char* s, void* stream);
int feof(void* stream);
int fseek(void* stream, long offset, int whence);
int fflush(void* stream);
long ftell(void* stream);

int fprintf(FILE* stream, const char* format, ...);
int sprintf(char* str, const char* fmt, ...);
int vsprintf(char* str, const char* format, va_list ap);
int sscanf(const char* str, const char* format, ...);

#ifdef __cplusplus
}
}

using std::fopen;
using std::fclose;
using std::fread;
using std::fwrite;
using std::fgets;
using std::fputs;
using std::feof;
using std::fseek;
using std::fflush;
using std::ftell;
using std::fprintf;
using std::sprintf;
using std::vsprintf;
using std::sscanf;
#endif

#endif
