#include <stdlib.h>
#include <stdio.h>
#include "sushi_yyparser.tab.h"
#include <string.h>






// https://en.wikipedia.org/wiki/Escape_sequences_in_C#Table_of_escape_sequences
char *sushi_unquote(char * s) {

	char char_lookup[128] = { '\0' };
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

	char *result;
	result = malloc(sizeof(s) + 1);

	// if(result == NULL){
	// 	return s;
	// }

	int j = 0;

	for(size_t i = 0; i < strlen(s); i++){

		if( (i + 1) >= strlen(s) ){
			result[j] = s[i];
			break;
		}

		if(s[i] == '\\'){
			if(char_lookup[ (int) s[i+1]] != '\0'){
				result[j] = char_lookup[ (int) s[i+1]];
				i++;
			}
			else{
				result[j] = s[i];
			}
		}

		else{
			result[j] = s[i];
		}

		j++;
	}

	result = realloc(result, strlen(result) + 1);
	printf("%zu\n", strlen(s));
	printf("%zu\n", strlen(result));
	//puts(result);

	//puts(result);
	
  return result;
}

// Do not modify this function
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}
