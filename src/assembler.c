// The Marsh 8-bit assembler
//
// assembler.c
// File created: May 20 2020
// written by moxniso

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "libmarsh.h"
#include "vector.h"
#include "label.h"

extern vector* ldatabase;

char* main_assembler(char* sourceBuffer, long sourceSize, char ROM[MAX_ROMSIZE])
{
	// initialize label databse
	ldatabase = vector_init(5);

        char* noSpace = malloc(sizeof(char) * sourceSize); 
	char* withza = malloc(sizeof(char) * sourceSize);
	long sourceSize_withza = sourceSize;

	// Removes whitespace and newline
	for (long i = 0,j = 0;j<sourceSize;i++) {
	        if ((!isblank(sourceBuffer[i])) && (sourceBuffer[i] != 0x0a)) { 
		 	noSpace[j] = sourceBuffer[i];
			debug_printf("i: %d j: %d noSpace[j]:%c\n", i, j, noSpace[j]);
		}
	        else j--;
		j++;
	}

	// Removes just whitespace for withza
	for (long i = 0,j = 0;j<sourceSize_withza;i++) {
		if (!isblank(sourceBuffer[i])) {
	                withza[j] = sourceBuffer[i];
	                debug_printf("i1: %d j1: %d withza[j]:%c\n", i, j, withza[j]);
	        }
	        else {
	                j--;
	                sourceSize_withza--;
	        }
	        j++;
	}
	sourceSize = strnlen(noSpace, 7000);
	debug_printf("sourceSize: %ld sourceSize_withza: %ld\n", sourceSize, sourceSize_withza);
	
	strcpy(sourceBuffer, noSpace);
	sourceBuffer[sourceSize] = '\0';
	free(noSpace);
	
	long pc_s = 0,
	     old_pc_s = 0,
	     ROMcounter = 0,
	     linecounter = 2;
	
	// Ignore comments before start symbol
	for(long i = 0;i<=sourceSize;i++) {
	      if (sourceBuffer[i] == '$') {
		      pc_s = i+1;
		      break;
    	      }
	      else if (withza[i] == 0x0a)
		             linecounter++;
	}

	if ((sourceBuffer[pc_s] == 'S') &&
	    (sourceBuffer[pc_s+1] == 'T') &&
	    (sourceBuffer[pc_s+2] == 'A') &&
	    (sourceBuffer[pc_s+3] == 'R') &&
	    (sourceBuffer[pc_s+4] == 'T')) {

	   debug_printf("START declaration found.\n");

	   pc_s += 5;
	   int p = 0;
	   while(pc_s < sourceSize) { // breaks once the index reaches '\0'
	        while(1) { // breaks after each instruction is fully parsed, so newline counting can work
		  
		        old_pc_s = pc_s;
		        debug_printf("pc_s = %ld %c %c %c\n", pc_s, sourceBuffer[pc_s], sourceBuffer[pc_s+1], sourceBuffer[pc_s+2]);
			
			if (ROMcounter >= MAX_ROMSIZE) {
			    printf("Program too large. Must compile down to under 7K bytes\n");
			    free(withza);
			    return NULL;
			}
			
			if (opcodeCheck_four(sourceBuffer, pc_s, 'M', 'O', 'V', 'I') == true) {
			   if (isdigit(sourceBuffer[pc_s+6])) {
						if (sourceBuffer[pc_s+4] == 'X') ROM[ROMcounter] = 0xC0;
						else if (sourceBuffer[pc_s+4] == 'Y') ROM[ROMcounter] = 0xD0;
						else ROM[ROMcounter] = 0xB1;
						ROM[ROMcounter+1] = toLiteralNibble(sourceBuffer[pc_s+6]);
						if ((ROM[ROMcounter+1]) < 0) {
						        printf("Line %ld\nInvalid number: %c\n\n", linecounter, sourceBuffer[pc_s+6]);
							free(withza);
							return NULL;
						}
						// TODO: make it so the second nibble in the ROM byte is edited.
						ROM[(ROMcounter+1) << 4] = toLiteralNibble(sourceBuffer[pc_s+7]);
						debug_printf("second nibble: %x %x\n", ROM[(ROMcounter+1) << 4], ROM[(ROMcounter+1) >> 4]);
						if ((ROM[(ROMcounter+1) << 4]) < 0) {
						        printf("Line %ld\nInvalid number: %c\nMake sure your numbers are two digits long (01 instead of just 1).\n\n", 
							                                                                        linecounter, sourceBuffer[pc_s+7]);
							free(withza);
							return NULL;
						}
			  }

			  else {  // Assume register to register MOVI
				     debug_printf("fell torwards else\n");
					  switch(sourceBuffer[pc_s+6]) {
					          case('X'): 
						           ROM[ROMcounter] = 0xFD;
						           break;
					          case('Y'):
						           ROM[ROMcounter] = 0xFE;
						           break;
					          case('S'):
						           ROM[ROMcounter] = 0xFF;
							   break;
					  }
					  switch(sourceBuffer[pc_s+4]) {
						  case('X'):
							   ROM[ROMcounter+1] = 0xC0;
							   break;
						  case('Y'):
						           ROM[ROMcounter+1] = 0xD0;
						           break;
					          case('S'):
						           ROM[ROMcounter+1] = 0xB1;
							   break;
					  }
					  if (!ROM[ROMcounter+1]) {
					         printf("Line %ld\nInvalid dest register: %c\n\n", linecounter, sourceBuffer[pc_s+4]);
					         free(withza);
					         return NULL;
					  }
					  else if (!ROM[ROMcounter]) {
						 printf("Line %ld\nInvalid src register: %c\n\n", linecounter, sourceBuffer[pc_s+6]);
						 free(withza);
						 return NULL;
					  }
					  ROMcounter += 2;
	     				  pc_s += 7;
					  printf("else jumped to %c\n", sourceBuffer[pc_s]);
					  break;
				}	
				ROMcounter += 2;	
				pc_s += 8;
				break;
		        }
			
			else if (opcodeCheck_four(sourceBuffer, pc_s, 'M', 'O', 'V', 'C') == true) {
		              if ((sourceBuffer[pc_s+6] == '~') || (isalpha(sourceBuffer[pc_s+6]))) {
			        switch(sourceBuffer[pc_s+4]) {
					case('X'):
					case('Y'):
				        case('S'):
						if (sourceBuffer[pc_s+4] == 'X') ROM[ROMcounter] = 0xC0;
						else if (sourceBuffer[pc_s+4] == 'Y') ROM[ROMcounter] = 0xD0;
						else ROM[ROMcounter] = 0xB1;
						
						if (sourceBuffer[pc_s+6] == '~') ROM[ROMcounter+1] = 0x0a;
						// Maybe turn this into either the Lisp ~% or C \n?	

						else if (sourceBuffer[pc_s+6] == '^') ROM[ROMcounter+1] = ' ';
						else ROM[ROMcounter+1] = sourceBuffer[pc_s+6];
						break;
					default:
					        printf("Line %ld\n Invalid register: %c %d\n\n", linecounter, sourceBuffer[pc_s+4], sourceBuffer[pc_s+4]);
					        free(withza);
						return NULL;
				}
				ROMcounter += 2;
				pc_s += 7;
			     }
			     else {
			           printf("Line %ld\n Invalid operand (%c), must be alphanumeric char or newline (~).\n", linecounter, sourceBuffer[pc_s+6]);
				   free(withza);
				   return NULL;
			     }
			     break;
			       
			}
			else if ((opcodeCheck_three(sourceBuffer, pc_s, 'A', 'D', 'D')) == true ||
				 (opcodeCheck_three(sourceBuffer, pc_s, 'S', 'U', 'B')) == true) {
				if (sourceBuffer[pc_s] == 'S') ROM[ROMcounter] = 0xA0;
				else ROM[ROMcounter] = 0xB0;
				ROMcounter++;
				pc_s += 3;
				break;
			}
			else if (opcodeCheck_four(sourceBuffer, pc_s, 'D', 'U', 'M', 'P') == true) {
				ROM[ROMcounter] = 0x40;
				ROMcounter++;
				pc_s += 4;
				break;
			}
			else if ((opcodeCheck_three(sourceBuffer, pc_s, 'P', 'O', 'P')) == true) {
				ROM[ROMcounter] = 0x60;		
				switch(sourceBuffer[pc_s+3]) {
					case('X'):
						ROM[ROMcounter+1] = 0x0C;
						break;
					case('Y'):
						ROM[ROMcounter+1] = 0x0D;
						break;
				        case('S'):
					        ROM[ROMcounter+1] = 0x0B;
					        break;
					default:
					        printf("Line %ld\nUnknown register to pop to: %c\n", linecounter,  sourceBuffer[pc_s+3]);
						free(withza);
						return NULL;
				}
				pc_s += 4;
				ROMcounter += 2;
				break;
			}
			else if ((opcodeCheck_four(sourceBuffer, pc_s, 'P', 'U', 'S', 'H')) == true) {
				ROM[ROMcounter] = 0x50;
				switch(sourceBuffer[pc_s+4]) {
					case('X'):
						ROM[ROMcounter+1] = 0x0C;
						break;
					case('Y'):
						ROM[ROMcounter+1] = 0x0D;
						break;
				        case('S'):
					        ROM[ROMcounter+1] = 0x0B;
						break;
				        default:
					        printf("Line %ld\nUnknown register to push: %c\n", linecounter, sourceBuffer[pc_s+4]);
						free(withza);
						return NULL;
				}
				pc_s += 5;
				ROMcounter += 2;
				break;
			}
			else if ((opcodeCheck_four(sourceBuffer, pc_s, 'H', 'A', 'L', 'T')) == true ||
				 (opcodeCheck_three(sourceBuffer, pc_s, 'E', 'N', 'D')) == true) {

			        if (sourceBuffer[pc_s] == 'H') {
				      ROM[ROMcounter] = 0xF0;
				      pc_s += 4;
				}
				else { 
				     ROM[ROMcounter] = 0xFA;
				     pc_s += 3;
				     ROMcounter++;
				     pc_s = sourceSize;
				}
				ROMcounter++;
				break;
			}
			else if (opcodeCheck_three(sourceBuffer, pc_s, 'C', 'M', 'P') == true) { 
			        ROM[ROMcounter] = 0xEC;
				ROMcounter++;
				pc_s += 3;
				break;
			}
			else if (opcodeCheck_three(sourceBuffer, pc_s, 'J', 'M', 'P') == true) {
				ROM[ROMcounter] = 0xA1;
				ROMcounter++;
				pc_s += 3;
				short llen = replaceLabels(ldatabase, sourceBuffer, ROM, &ROMcounter, pc_s, linecounter);
				if (llen <= 0) {
					free(withza);
					return NULL;
				}
			
				ROMcounter++;
				debug_printf("llen: %d %c %c %c\n", llen, sourceBuffer[pc_s], sourceBuffer[pc_s+1], sourceBuffer[pc_s+2]);
				pc_s += llen;
				break;
			}
			else if (opcodeCheck_three(sourceBuffer, pc_s, 'L', 'A', 'B') == true) {
				pc_s += 3;
				pc_s = (pc_s + (lassign(ldatabase, sourceBuffer, ROMcounter, pc_s)) + 1);
				break;
			}
			else if (opcodeCheck_syscall(sourceBuffer, pc_s) == true) {
				ROM[ROMcounter] = 0xE0;
				ROMcounter++;
				pc_s += 7;
				break;
			}
			else {
				char error[] = {sourceBuffer[pc_s], sourceBuffer[pc_s+1],
						sourceBuffer[pc_s+2], sourceBuffer[pc_s+3]};
				printf("Line %ld\nwtf, unknown instructions: %s\n\n", linecounter, error);
				free(withza);
				return NULL;
			}
	        }
		
	        for (long i = old_pc_s;i<=pc_s;i++)
	                if (withza[i] == 0x0a) linecounter++;
	     }
		
	     printf("\nCompilation finished.\nCreating ROM...\n");
	     ROM[ROMcounter+1] = '\0'; // just to be safe and set the new terminating end of ROM[]
             // freeLabels(ldatabase);
             free(withza);
             return ROM;

	}
	else {
	        printf("START declaration not found.\nMake sure your code begins with '$START'.\n");
		free(withza);
		return NULL;
	}
}
