#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <string.h>

int sushi_exit = 0;

int main()
{

	
	char_lookup_setup();
	char *path = getenv("HOME");
	puts(path);

	int result;
	if( (result = sushi_read_config(strcat(path,"/sushi.conf"))) == 1){
		return EXIT_FAILURE;
	}

	char *line;
	int i = 0;

	while(sushi_exit == 0){
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
