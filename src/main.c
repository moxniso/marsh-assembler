// main.c
// file created: May 20 2020
// written by moxniso

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "marshdefs.h"
#include "vector.h"

extern char* main_assembler(char* sourceBuffer, long sourceSize, char ROM[MAX_ROMSIZE]);
extern void outputROM(char* ROMdata, char* outname);
vector* ldatabase;

int main(int argc, char** argv)
{
        char* outname;
        if (argc != 2) {
		printf("Usage: mas [filename].m -o [output].mbin\n");
		return 0;
	}
	else {
		if (argc >= 3) {
		  if (strncmp(argv[1], "-o", 3) == 0) {
		        outname = malloc(sizeof(char) * (strlen(argv[2])+1));  
		        strcpy(outname, argv[2]); 
		   	argv[1] = argv[3];
		   }
		  else if (strncmp(argv[2], "-o", 3) == 0) {
		        outname = malloc(sizeof(char) * (strlen(argv[3])+1));
		        strcpy(outname, argv[3]);
		  }
		}
                
		else {
			outname = malloc(sizeof(char) * 14);
			strcpy(outname, "marshout.mbin");
		}

		fprintf(stdout, "Marsh 8-bit assembler\ncompiled on %s at %s\n2020 moxniso\n\nStarting...\n", __DATE__, __TIME__);
		FILE* source = fopen(argv[1], "r");
		if (!source) {
			printf("No source file found called %s.\n", argv[1]);
			return 1;
		}
		
		printf("File to be assembled, %s\n", argv[1]);
		fseek(source, 0, SEEK_END);
		long sourceSize = (ftell(source));
		rewind(source);
		char* sourceBuffer = malloc(sizeof(char) * sourceSize);
		fread(sourceBuffer, sizeof(char), sourceSize, source);
		fclose(source);
		printf("File loaded in successfully!\n");
	
		char ROM[MAX_ROMSIZE];	
		char* output = main_assembler(sourceBuffer, sourceSize, ROM);
		free(sourceBuffer);
		vector_free(ldatabase);

		if (!output) {
			printf("Failed to assemble\n");
			return 1;
		}

		outputROM(output, outname);
		free(outname);
		return 0;
	}
}
