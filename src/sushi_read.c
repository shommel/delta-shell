#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include "sushi.h"

#define SEP "\n" 
#define SEP_CODE 10

char *sushi_read_line(FILE *in) {

	char buffer[SUSHI_MAX_INPUT+1]; //+1 for the terminating zero
	char *result;
	char *tok;
	int is_blank = 1;
		

	if( (fgets(buffer, sizeof(buffer), in) == NULL) && (!feof(in)) ) { 
		perror("fgets");
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

	result = super_malloc( strlen(tok) + 1);
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
	if( (fpIN = fopen(fname, "r")) ==  NULL){
		//It's OK if the file does not exist!
		perror(fname);
	}
	
	char *line;
	int result;
	while( !feof(fpIN) ){ 
		if( (line = sushi_read_line(fpIN)) != NULL){
			if( (result = sushi_parse_command(line)) == 0 ){
				sushi_store(line);
			}
		}
	}

	if( (result =  fclose(fpIN)) != 0){
		perror(fname);
		return 1;
	}

  return 0;
}
