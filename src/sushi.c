#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <string.h>

int sushi_exit = 0;

int main() {

	char *path = getenv("HOME");
	char_lookup_setup(); //initialize char_lookup table


	int result;
	if( (result = sushi_read_config(strcat(path,"/sushi.conf"))) == 1){
		return EXIT_FAILURE;
	}

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
