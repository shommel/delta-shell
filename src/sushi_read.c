#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "sushi.h"

#define SEP "\n\r"

char *sushi_read_line(FILE *in) {

	char buffer[SUSHI_MAX_INPUT+1];
	char *result;
	char *tok;
	//char *tok;

	if(fgets(buffer, sizeof(buffer), in) == NULL){
		perror(fgets(buffer, sizeof(buffer), in));
		return NULL;
	}

	int is_blank = 1;

	puts("checking length");
	
	// if( buffer != SEP){
	// 	puts("nonzero");

	// 	tok = malloc( sizeof(buffer) + 1);
	// 	tok = strtok(buffer, SEP);

	// 	puts(tok);

	// 	for(size_t i = 0; i < strlen(tok); i++){
	// 		puts("checking spaces");

	// 		if(!isspace(tok[i])){
	// 			is_blank = 0;
	// 			break;
	// 		}
	// 	}
	// }

	// else{
	// 	is_blank = 1;
	// }

	// if(is_blank){
	// 	puts("fully blank");
	// 	return NULL;
	// }

	result = malloc( strlen(tok) + 1);
	strcpy(result, tok);
	//free(tok);
	//puts(result);

	if(strlen(buffer) == SUSHI_MAX_INPUT+1){
		while( fgets(buffer, sizeof(buffer), in ) != NULL){
			
		}
	}
	


  return result;
}

int sushi_read_config(char *fname) {

	FILE *fpIN;

	if( (fpIN = fopen(fname, "r")) == NULL){ //error opening
		perror(fname);
		return 1;		
	}

	char *line;

	while( (line = sushi_read_line(fpIN)) != NULL){
		sushi_store(line);
	}

	fclose(fpIN);

  return 0;
}
