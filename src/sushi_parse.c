#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#include "sushi.h"
#include "sushi_yyparser.tab.h"
#include <sys/wait.h>
#include <sys/types.h>

static char char_lookup[128] = { '\0' };

int count_digits(int n){
	if(n == 0){
		return 1;
	}

	else{
		int count = 0;

    	while(n != 0){
    		count++;

    		n /= 10;

    	}
    	return count;
    }
    
}


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

	size_t new_size;
	//i is reader pointer, j is writer pointer
	for(size_t i = 0, j = 0; i < strlen(s); i++, j++){

		if( (s[i] == '\\') && (char_lookup[ (int) s[i+1]] != '\0') ){
			s[j] = char_lookup[ (int) s[i+1]];
			i++; //eat next character
		}

		else{
			s[j] = s[i];
		}

		new_size = j;

	}

	//s = super_realloc(s, new_size+1);
	//s[new_size] = '\0';

  return s;
}


// Do not modify these functions
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}

void __not_implemented__() {  
  fprintf(stderr, "%s\n", "This operation is not implemented yet");

}

void free_memory(prog_t *exe, prog_t *pipe) {

	for(size_t i = 0; i < sizeof(exe->args.args)/sizeof(exe->args.args[0]); i++){
		if(exe->args.args[i] != NULL){
			free(exe->args.args[i]);
		}
	}

	free(exe->args.args);

  	//free each non-NULL exe->redirection
	if(exe->redirection.in != NULL){
		puts("free in");
		free(exe->redirection.in);
	}

	if(exe->redirection.out1 != NULL){
		puts("free out1");

		free(exe->redirection.out1);
	}

	if(exe->redirection.out2 != NULL){
		puts("free out2");

		free(exe->redirection.out2);
	}

	free(exe);

	// if(pipe != NULL){
	// 	free(pipe);
	// }
}

/// Skeleton
void sushi_assign(char *name, char *value) {
	printf("Name = %s\n", name);
	printf("Value = %s\n", value);

	setenv(name, value, 1);

	free(value);
	free(name);
}

// Skeleton
char *sushi_safe_getenv(char *name) {
	char *result;
	if( (result = getenv(name)) != NULL) {
		return result;
	}
  return ""; // DZ: change it!
}

int sushi_spawn(prog_t *exe, prog_t *pipe, int bgmode){
	
	exe->args.args = super_realloc(exe->args.args, (exe->args.size + 1) * sizeof(char *) );
	exe->args.args[exe->args.size] = NULL; //the form that execvp expects

	pid_t result = fork();
	//pid_t pid;

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
		if(bgmode == 1){
			return 0;
		}
		else if(bgmode == 0){
			int child_status;

			free_memory(exe, pipe); //Free Memory

			pid_t w = waitpid(result, &child_status, 0);
			// puts("child status");
			// printf("%d\n", child_status);

			//convert child_status to string
			char status_string[ count_digits(child_status) + 1];

			// printf("%d\n", child_status);
			// printf("%zu\n", sizeof(child_status));
			// printf("%zu\n", sizeof(int));


			sprintf(status_string, "%d", child_status);
			//itoa(child_status, status_string, 10);

			// puts("status string");
			// puts(status_string);


			setenv("_", status_string, 1);
			//free(status_string);
		}
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
	if(ptr2 == NULL) { abort(); }

	return ptr2;
}
