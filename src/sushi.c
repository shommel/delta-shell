#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

int sushi_exit = 0;

int main()
{
	int result;
	if( (result = sushi_read_config("sushi.conf")) == 1){
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
