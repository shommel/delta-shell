#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi.h"
#include <signal.h>


int sushi_exit = 0;

void welcome(){
	//displaying welcome message
	printf("%s\n%s\n%s\n%s\n", "###############", "Welcome to SUSHI", 
		"Spencer Hommel, 2019 (patent pending)", "###############");
}

static void refuse_to_die(int sig)
{
	if(sig == SIGINT){
		fprintf(stderr, "\n%s\n%s", "Type exit to exit the shell", SUSHI_DEFAULT_PROMPT);

	}
}

//https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
//https://unix.stackexchange.com/questions/351312/sigint-handler-runs-only-once
static void prevent_interruption() {

    struct sigaction action;
    action.sa_handler = refuse_to_die;

    //to solve the fgets: Interrupted system call error from 18 in sushi_read.c
    action.sa_flags = SA_RESTART;

    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);

    sigaction(SIGINT, &action, NULL);
}

int main(int argc, char const *argv[]) {

	welcome();

	int setenv_result;
	if( (setenv_result = setenv("SHELL", argv[0], 1) ) != 0){
		perror("setenv");
	}

	char_lookup_setup(); //initialize char_lookup table

	char *path = super_malloc( strlen(getenv("HOME")) + strlen("/sushi.conf") + 1);
	strcpy(path, getenv("HOME"));
	strcat(path,"/sushi.conf");

	int result;
	if( (result = sushi_read_config(path, 1)) == 1){
		return EXIT_FAILURE;
	}

	free(path);

	if(argc > 1){
		char *arg_tmp;
		for(int i = 1; i < argc; i++){
			arg_tmp = super_strdup(argv[i]);
			sushi_read_config(arg_tmp, 0);
			free(arg_tmp);
		}
	}

	prevent_interruption();

	char *line;
	char *prompt;
	int i = 0;
	while(sushi_exit == 0){ //until user types "exit"

		if((prompt = getenv("PS1")) == NULL){
			printf("%s", SUSHI_DEFAULT_PROMPT);
		}

		else{
			printf("%s", prompt);
		}
		

		if( (line = sushi_read_line(stdin)) != NULL){
			if( (result = sushi_parse_command(line)) == 0 ){
				sushi_store(line);
			}
		}

	}

	return EXIT_SUCCESS;
}
