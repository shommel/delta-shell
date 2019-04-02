#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sushi.h"
#include "sushi_yyparser.tab.h"
#include <sys/wait.h>
#include <sys/types.h>

static char char_lookup[128] = { '\0' };

int count_digits(int n){

	if(n == 0){
		return 1;
	}

	return (int)log10(n) + 1;

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

	size_t j = 0;
	//i is reader pointer, j is writer pointer
	for(size_t i = 0; i < strlen(s); i++, j++){

		if( (s[i] == '\\') && (char_lookup[ (int) s[i+1]] != '\0') ){
			s[j] = char_lookup[ (int) s[i+1]];
			i++; //eat next character
		}

		else{
			s[j] = s[i];
		}

	}

	s[j] = '\0'; //putting a null-term zero at the end of the writer pointer (end of new string)

  return s;
}


// Do not modify these functions
void yyerror(const char* s) {
  fprintf(stderr, "Parse error: %s\n", s);
}

void __not_implemented__() {
  fprintf(stderr, "%s\n", "This operation is not implemented yet");

}

void free_memory(prog_t *exe) {
	prog_t *tmp;

	while( exe != NULL){ //will loop backwards until it reaches first arugment on 
						 //command line's prev (which is NULL)
		for(size_t i = 0; i < sizeof(exe->args.args)/sizeof(exe->args.args[0]); i++){
			if(exe->args.args[i] != NULL){
				free(exe->args.args[i]);
			}
		}

		free(exe->args.args);

	  	//free each non-NULL exe->redirection
		if(exe->redirection.in != NULL){
			free(exe->redirection.in);
		}

		if(exe->redirection.out1 != NULL){
			free(exe->redirection.out1);
		}

		if(exe->redirection.out2 != NULL){
			free(exe->redirection.out2);
		}

		tmp = exe;
		exe = exe->prev;
		free(tmp);
	}
}

/// Skeleton
void sushi_assign(char *name, char *value) {
	int result;
	if( (result = setenv(name, value, 1)) != 0){
		perror("setenv");
	}
	
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

/*------------------------------------------------------------------
 * You can use these "convenience" functions as building blocks for
 * HW5 instead of your code, if you want. You may change them, too.
 *------------------------------------------------------------------*/

// Find the number of programs on the command line
static size_t cmd_length(prog_t *exe) {
  int count = 0;
  while(exe->prev) {
    exe = exe->prev;
    count++;
  }
  return count;
}

// Wait for the process pid to terminate; once it does, set the
// environmental variable "_" to the exit code of the process.
static int wait_and_setenv(pid_t pid) {
  int status;
  if (-1 == waitpid(pid, &status, 0)) {
    perror("waitpid");
    status = 1; // Something bad happened
  }
  char retval[16]; // Just to be safe
  sprintf(retval, "%d", status);
  if(-1 == setenv("_", retval, 1)) {
    perror("_");
    return 1;
  } else
    return 0;
}

// Execute the program defined in "exe"
static void start(prog_t *exe) {
  arglist_t args = exe->args;
  args.args = realloc(args.args, sizeof(char*) * (args.size + 1));
  args.args[args.size] = (char*)NULL;
  execvp(args.args[0], args.args);
  perror(args.args[0]);
}

// "Rename" fule descriptor "old" to "new," if necessary. After the
// execution of this function a program that "believes" that it uses
// the "old" descriptor (e.g., stdout #1 for output) will be actually
// using the "new" descriprot (e.g., an outgoinf pipe).  This
// functions terminates the process of error and should not be used in
// the parent, only in a child.
static void dup_me (int new, int old) {
  if (new != old && -1 == dup2(new, old)) {
    perror("dup2");
    exit(1);
  }
}

/*--------------------------------------------------------------------
 * End of "convenience" functions
 *--------------------------------------------------------------------*/

int sushi_spawn(prog_t *exe, int bgmode){

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

		if(bgmode == 1){
			return 0;
		}

		else if(bgmode == 0){
			int child_status;

			free_memory(exe); //Free Memory

			pid_t w = waitpid(result, &child_status, 0);

			//convert child_status to string
			char status_string[ count_digits(child_status) + 1];
			sprintf(status_string, "%d", child_status);

			setenv("_", status_string, 1);

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

char *super_strdup(const char *ptr) {
	char *ptr2 = strdup(ptr);
	if(ptr2 == NULL) { abort(); }

	return ptr2;
}
