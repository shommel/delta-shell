#include <string.h>
#include <errno.h>
#include "sushi.h"
#include "sushi_yyparser.tab.h"

static char char_lookup[128] = { '\0' };

void char_lookup_setup(){
	//changes the char_lookup table with alloc necessary escape sequences

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
	puts("freeing memory (free_memory not yet implemented)");
  // TODO - but not this time
}

// Skeleton
void sushi_assign(char *name, char *value) {
}

// Skeleton
char *sushi_safe_getenv(char *name) {
  return NULL; // DZ: change it!
}

int sushi_spawn(prog_t *exe, prog_t *pipe, int bgmode){

	exe->args.args = super_realloc(exe->args.args, (exe->args.size + 1) * sizeof(char *) );
	exe->args.args[exe->args.size] = NULL; //the form that execvp expects

	pid_t result = fork();

	if(result < 0) { //fork failed, parent process
		perror("fork");
		return 1;
	}

	else if(result == 0) { //child process
		int status = execvp(exe->args.args[0], exe->args.args);

		if(status < 0){
			perror(exe->args.args[0]);
			exit(0);
		}		
	}

	else { //parent process
		free_memory(exe, pipe);
	}

	return 0;
}

void *super_malloc(size_t size) {

	void *ptr = malloc(size);
	if(ptr == NULL) { abort(); }	

  return ptr;
}

void *super_realloc(void *ptr, size_t size) {
	
	ptr = realloc(ptr, size);
	if(ptr == NULL) { abort(); }

  return ptr;
}

char *super_strdup(char *ptr) {
	char *ptr2 = strdup(ptr);
	//puts(ptr2);
	//printf("%zu\n", strlen(ptr));
	if(ptr2 == NULL) { abort(); }

	return ptr2;
}
