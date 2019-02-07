#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sushi.h"

#define SEP "\n\r"
// DZ: 10 is a magic number; use '\n'
#define SEP_CODE 10 //ASCII code for newline

char *sushi_read_line(FILE *in) {

	char buffer[SUSHI_MAX_INPUT+1]; //+1 for the terminating zero
	char *result;
	char *tok;
	int is_blank = 1;

	if(fgets(buffer, sizeof(buffer), in) == NULL){
	  // DZ: Must add "if(!feof(in))"
	  // DZ: Otherwise one gets "Error with memory allocation: Success"
		perror("Error with memory allocation");
		return NULL;
	}

	tok = strtok(buffer, SEP); //seperate by newline

	if(tok == NULL){
		return NULL;
	}

	for(size_t i = 0; i < strlen(tok); i++){
		if(!isspace(tok[i])){
			is_blank = 0;
			break;
		}
	}

	if(is_blank){ //return NULL if line is blank
		return NULL;
	}

	// DZ: Must always check the returned value of malloc
	result = malloc( strlen(tok) + 1);
	strcpy(result, tok);

	if(strlen(buffer) == SUSHI_MAX_INPUT){ //enter only when line is longer than max
		fprintf(stderr, "%s\n", "Line too long, truncated");
		char *remainder; //variable for cleaning up rest of line

		//cleaning up rest of line
		while( (remainder = strchr(buffer, SEP_CODE)) == NULL){
			fgets(buffer, sizeof(buffer), in);
		}
	}

  return result;
}

int sushi_read_config(char *fname) {

	FILE *fpIN;

	if( (fpIN = fopen(fname, "r")) == NULL){ //error opening
		perror(fname);
		// DZ: It's OK if the file does not exist!
		return 1;		
	}

	char *line;

	while( !feof(fpIN) ){ 
		if( (line = sushi_read_line(fpIN)) != NULL){
			sushi_store(line);
		}
	}

	fclose(fpIN);

  return 0;
}
