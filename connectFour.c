#include <stdio.h>
#include <stdlib.h>
#include "arguments.h"
#include "file_utils.h"
/**
 * Created by Miles De Wind - -2/26/2017
 * Creates a game of connect four.
 */

int main(int argc, char** argv){
	int width;
	int height;
	int connect;
	int player = 1;
	struct arguments values;
	int checkwin = 0;
	char* save;
	char* load;
	char* buffer;
	values = setup(argc, argv);
	width = values.first;
	height = values.second;
	connect = values.third;
	save = values.save_file;
	load = values.load_file;

	if(width == -1){
	fprintf(stderr, "ERROR: Connect value is greater than board width or height\n");
	}
	int** board = malloc(sizeof(int*) * width);
	for(int index=0; index < width; index++){
		board[index] = calloc(sizeof(int), height);
	}


	display(board, width, height);
	//checkwin = checkVert(board, height, 0, 4, 1);
	//checkwin = checkDiagnalDown(board, width, height, 0, 1, 4, 1);
	char* str;
	//must fix this
	fgets(str, 1, stdin);
	if(str[0] == '1'){
		fprintf(stderr, "worksbby\n");
	}
	if(save != NULL){
		saveGame(board, width, height, connect, player, save);
	}

	if(load != NULL){
		loadGame(buffer, load);
	}



  return 0;

}
