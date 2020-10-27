// libmarsh.c
// file created: May 31 2020
// written by moxniso

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdarg.h>
#include "marshdefs.h"
#include "libmarsh.h"

char toLiteralNibble(char a)
{
	switch(a) {
		case '0':
			return 0x0;
		case '1':
			return 0x1;
		case '2':
			return 0x2;
		case '3':
			return 0x3;
		case '4':
			return 0x4;
		case '5':
			return 0x5;
		case '6':
			return 0x6;
		case '7':
			return 0x7;
		case '8':
			return 0x8;
		case '9':
			return 0x9;
		default:
			return -1; 
	}
}

BOOL opcodeCheck_three(char* sourceBuffer, long pc_s, char a, char b, char c)
{
	if ((sourceBuffer[pc_s] == a) &&
	    (sourceBuffer[pc_s+1] == b) &&
	    (sourceBuffer[pc_s+2] == c)) return true;
	else return false;
}

BOOL opcodeCheck_four(char* sourceBuffer, long pc_s, char a, char b, char c, char d)
{
	if ((sourceBuffer[pc_s] == a) &&
	    (sourceBuffer[pc_s+1] == b) &&
	    (sourceBuffer[pc_s+2] == c) &&
	    (sourceBuffer[pc_s+3] == d)) return true;
	else return false;
}

BOOL opcodeCheck_syscall(char* sourceBuffer, long pc_s)
{
	if ((sourceBuffer[pc_s] == 'S') &&
            (sourceBuffer[pc_s+1] == 'Y') &&
	    (sourceBuffer[pc_s+2] == 'S') &&
	    (sourceBuffer[pc_s+3] == 'C') &&
	    (sourceBuffer[pc_s+4] == 'A') &&
	    (sourceBuffer[pc_s+5] == 'L') &&
	    (sourceBuffer[pc_s+6] == 'L')) return true;
	 else return false;
}

BOOL sourceCmp(char* sourceBuffer, long pc_s, char* str, int len)
{
	for (int i = 0;i<=len;i++) {
		debug_printf("sourceCmp: %c %c\n", sourceBuffer[pc_s+i], str[i]);
		if (sourceBuffer[pc_s+i] != str[i]) 
			return false;
	}
	return true;
}

char* sourceStrAm(char* sourceBuffer, long pc_s, int len)
{
	char* str = malloc(len+1);
	for (int i=0;i<=len;i++) 
		str[i] = sourceBuffer[pc_s+i];
	
	str[len+1] = '\0';
	return str;
}
		 