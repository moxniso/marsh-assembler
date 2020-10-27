#ifndef LIBMARSH_H
#define LIBMARSH_H

#include "marshdefs.h"

char toLiteralNibble(char a);
char* sourceStrAm(char* sourceBuffer, long pc_s, int len);
BOOL opcodeCheck_three(char* sourceBuffer, long pc_s, char a, char b, char c);
BOOL opcodeCheck_four(char* sourceBuffer, long pc_s, char a, char b, char c, char d);
BOOL opcodeCheck_syscall(char* sourceBuffer, long pc_s);
BOOL sourceCmp(char* sourceBuffer, long pc_s, char* str1, int len);

typedef struct {
    char* name;
    int len;
    char address;
} label;

#ifdef DEBUG
#define debug_printf(...) printf(__VA_ARGS__);
#else
#define debug_printf(...) NULL;
#endif

#endif
