// outputROM.c
// file created: May 21 2020
// written by moxniso

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libmarsh.h" // just for debug_printf()

void outputROM(char* ROMdata, char* outname)
{
        FILE* test = fopen(outname, "r");
	if (test) {
		printf("A file named %s exists, are you sure you want to overwrite it?\n(y or n): ", outname);
		switch(fgetc(stdin)) {
			case 'y':
			case 'Y':
				printf("Overwriting...\n");
				break;
			default:
				printf("Output cancelled.\n");
				fclose(test);
				return;
		}
		fclose(test);
	}
	FILE* output = fopen(outname, "w");
       	if (!output) {
		printf("Unable to create a ROM file. Maybe try with sudo?\n");
		return;
	}

	int romSize = strnlen(ROMdata, 7000);
	debug_printf("sizeof_rom: %d\n", romSize);
	for (int i = 0;i<=romSize;i++) 
	  debug_printf("0x%x\n", ROMdata[i]);
	
	for (int i = 0;i<=romSize;i++) {
	  debug_printf("0x%x going into %d\n", ROMdata[i], i);
	  fprintf(output, "%c", ROMdata[i]);
	}
	//fwrite(ROMdata, 1, sizeof(ROMdata), output);
	
	fclose(output);
	printf("ROM successfully outputted (%s).\n", outname);
	return;
}	
