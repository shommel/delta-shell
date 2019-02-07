#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

// int main(int argc, char *argv[]) {
//     return EXIT_SUCCESS;
// }

int main()
{
	int result;
	// DZ: Must be $HOME/sushi.conf
	if( (result = sushi_read_config("sushi.conf")) == 1){
		return EXIT_FAILURE;
	}

	char *line;

	printf("%s", SUSHI_DEFAULT_PROMPT);

	if(	(line = sushi_read_line(stdin)) == NULL) {
		return EXIT_FAILURE;
	}

	sushi_store(line);
	sushi_show_history();
	return EXIT_SUCCESS;

}
