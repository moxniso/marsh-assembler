// File created: October 21 2020
// Written by moxniso
// label.c, label preparing/parsing code

#include <stdio.h>
#include <stdlib.h>
#include "libmarsh.h"
#include "vector.h"

short replaceLabels(vector* ldatabase, char* sourceBuffer, char* ROM, long* ROMcounter, long pc_s, long linecounter)
{
   label* lnew = malloc(sizeof(label));
  
   for (int i = 0;i < vector_total(ldatabase);i++) {
     lnew = vector_get(ldatabase, i);
     debug_printf("lnew->len: %d  lnew->name: %s\n", lnew->len, lnew->name);
     if (sourceCmp(sourceBuffer, pc_s, lnew->name, lnew->len) == true) {
         ROM[(*ROMcounter)] = lnew->address;
         (*ROMcounter)++;
	 
	 short llen = lnew->len;
	 lnew = NULL;
	 return llen;
     }
   }

   char* lundef = sourceStrAm(sourceBuffer, pc_s, 10);
   printf("Line %d: Undefined label: %s\n", linecounter, lundef);

   lnew = NULL;
   free(lundef);
   free(lnew);
   return 0;
}

void freeLabels(vector* ldatabase)
{
    label* lnew = malloc(sizeof(label));
	
    for (int i=0;i<vector_total(ldatabase);i++) {
	lnew = vector_get(ldatabase, i);
	if (lnew != NULL) {
		debug_printf("in freeLabels\n");
		free(lnew->name);
		free(lnew);
	}
    }
    
}			
  
long lassign(vector* ldatabase, char* sourceBuffer, long ROMcounter, long pc_s)
{
	label* lnew = malloc(sizeof(label));
	lnew->name = malloc(20);
	long i = 0;

	for (i = 0;i<=30;i++) {
	  if (sourceBuffer[pc_s+i] == ':') {
	      	lnew->len = i;
	      	lnew->name[i] = '\0';
	      	debug_printf("lnew->name: %s\n", lnew->name);
	      	break;
	  }

	  lnew->name[i] = sourceBuffer[pc_s+i];
	}
	  
	lnew->address = (char)ROMcounter;
	debug_printf("ldatabase->current_total:%d\n", ldatabase->current_total);
	vector_add(ldatabase, lnew);
	return i;
}


















  
       

