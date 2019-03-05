#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <string.h>

int sushi_exit = 0;

static void refuse_to_die(int sig)
{
	sig++;
  // TODO
}

static void prevent_interruption() {
  // TODO
}

int main() {

	char_lookup_setup(); //initialize char_lookup table

	char *path = super_malloc(strlen(getenv("HOME") + strlen("/sushi.conf") + 1));
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
