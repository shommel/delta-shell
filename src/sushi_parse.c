#include <stdlib.h>
#include <stdio.h>
#include "sushi_yyparser.tab.h"
#include <string.h>


static char char_lookup[128] = { '\0' };

void char_lookup_setup(){
	char_lookup['a'] = '\a';
	char_lookup['b'] = '\b';
	char_lookup['f'] = '\f';
	char_lookup['n'] = '\n';
	char_lookup['r'] = '\r';
	char_lookup['t'] = '\t';
	char_lookup['v'] = '\v';
	char_lookup['\\'] = '\\';
	char_lookup['\''] = '\'';
	char_lookup['\"'] = '\"';
	char_lookup['?'] = '\?';
}

// https://en.wikipedia.org/wiki/Escape_sequences_in_C#Table_of_escape_sequences
char *sushi_unquote(char * s) {

	char *result;
	result = malloc(strlen(s) + 1);

	if(result == NULL){
		perror("lexical analyzer");
		return NULL;
	}

	for(size_t i = 0, j = 0; i < strlen(s); i++, j++){

		if( (s[i] == '\\') && (char_lookup[ (int) s[i+1]] != '\0') ){
			result[j] = char_lookup[ (int) s[i+1]];
			i++;
		}

		else{
			result[j] = s[i];
		}
	}

	result = realloc(result, strlen(result) + 1);
	if(result == NULL){
		perror("lexical analyzer");
		return NULL;
	}

  return result;
}

// Do not modify this function
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}
