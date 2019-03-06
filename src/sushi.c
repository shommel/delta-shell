#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi.h"
#include <signal.h>


int sushi_exit = 0;

static void refuse_to_die(int sig)
{
	if(sig == SIGINT){ 	
		fprintf(stderr, "\n%s\n", "Type exit to exit the shell");
	}
}

//https://www.gnu.org/software/libc/manual/html_node/Sigaction-Function-Example.html
//https://unix.stackexchange.com/questions/351312/sigint-handler-runs-only-once
static void prevent_interruption() {

    struct sigaction action;

    action.sa_handler = refuse_to_die;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask, SIGINT);
    sigaction(SIGINT, &action, NULL);
    
}

int main() {

	char_lookup_setup(); //initialize char_lookup table

	char *path = super_malloc( strlen(getenv("HOME")) + strlen("/sushi.conf") + 1);
	strcpy(path, getenv("HOME"));
	strcat(path,"/sushi.conf");

	int result;
	if( (result = sushi_read_config(path)) == 1){
		return EXIT_FAILURE;
	}
	free(path);

	prevent_interruption();

	char *line;
	int i = 0;
	while(sushi_exit == 0){ //until user types "exit"
		printf("%s", SUSHI_DEFAULT_PROMPT);
		if(	(line = sushi_read_line(stdin)) == NULL) {
			return EXIT_FAILURE;
		}
		if( (result = sushi_parse_command(line)) == 0 ){
			sushi_store(line);
		}

	}

	return EXIT_SUCCESS;
}
