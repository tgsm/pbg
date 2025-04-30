#ifndef _MSL_SETJMP_H
#define _MSL_SETJMP_H

typedef long* jmp_buf[100];

#pragma internal on
int __setjmp(jmp_buf);
#pragma internal off

#define setjmp(jmp_buf) __setjmp(jmp_buf)

#endif
