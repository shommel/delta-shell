#include "sushi.h"
#include "sushi_yyparser.tab.h"
#include <string.h>


static char char_lookup[128] = { '\0' };

void char_lookup_setup(){
	//changes the char_lookup table with all necessary escape sequences

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

	//i is reader pointer, j is writer pointer
	for(size_t i = 0, j = 0; i < strlen(s); i++, j++){

		if( (s[i] == '\\') && (char_lookup[ (int) s[i+1]] != '\0') ){
			s[j] = char_lookup[ (int) s[i+1]];
			i++; //eat next character
		}

		else{
			s[j] = s[i];
		}
	}

  return s;
}

// Do not modify these functions
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}

void __not_implemented__() {  
  fputs("This operation is not implemented yet\n", stderr);
}

// Function skeletons for HW3
void free_memory(prog_t *exe, prog_t *pipe) {
  // TODO - but not this time
}

int spawn(prog_t *exe, prog_t *pipe, int bgmode) {
  return 0; // TODO
}

void *super_malloc(size_t size) {
  return NULL; // TODO
}

void *super_realloc(void *ptr, size_t size) {
  return NULL; // TODO
}
