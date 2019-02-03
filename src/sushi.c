#include <stdlib.h>
#include <stdio.h>
#include "sushi.h"

// int main(int argc, char *argv[]) {
//     return EXIT_SUCCESS;
// }

int main()
{
	
	sushi_read_config("sushi.conf");
	//sushi_show_history();

	char *line;

	printf("%s", SUSHI_DEFAULT_PROMPT);
	line = sushi_read_line(stdin);
	sushi_store(line);

	sushi_show_history();
	return EXIT_SUCCESS;
}
