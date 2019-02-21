#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"
#include <string.h>

int sushi_exit = 0;

int main()
{
<<<<<<< HEAD

	
	char_lookup_setup();
	char *path = getenv("HOME");
	puts(path);
=======
	char_lookup_setup(); //initialize char_lookup table
>>>>>>> efbfbbc18afd8c3008206788326824b6b82009e8

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
