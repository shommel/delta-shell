#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "sushi.h"

static char *history[SUSHI_HISTORY_LENGTH] = {NULL};

void sushi_store(char *line) {

	if(line == NULL){
		return;
	}
	
	if(history[SUSHI_HISTORY_LENGTH-1] == NULL){ 
		free(history[SUSHI_HISTORY_LENGTH-1]);
	}

	//copy block of memory
	bcopy(history, &(history[1]), sizeof(char *) * (SUSHI_HISTORY_LENGTH-1));

	history[0] = line; //add most recent line to history


	return;
}

void sushi_show_history() {

	for(int i = 0; i < SUSHI_HISTORY_LENGTH; i++){
		if(history[i] != NULL){
			printf("% 5d  %s\n", i+1, history[i]); //right justified to 5 dec. places
		}

		else{
			break;
		}

	}

	return;

}

