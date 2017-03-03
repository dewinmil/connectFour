#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
	int* results = malloc(sizeof(int) * 2);
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

	if(load != NULL){
		loadGame(buffer, load);
	}

	while(1){
		display(board, width, height);
		fprintf(stderr, "Player %d:", player);

		char* str = malloc(sizeof(char) * 80);

		//receive user input & parse by line and space
		fgets(str, 80, stdin);
		char *token;
		if(strcmp(str, "\n") == 0){
			continue;
		}
		token = strtok(str," \n");

		//exit call
		if(strcmp(token, "exit") ==0){
			break;
		}
		//check for save
		if(strcmp(token, "save")==0){
			//increment token
			token = strtok(NULL, "\n");
			if(token != NULL){
				saveGame(board, width, height, connect, player, token);
				continue;
			}else{
				fprintf(stderr, "Error: No Filename\n");
			}
		}

		if(strcmp(token, "load")==0){
			//increment token
			token = strtok(NULL, "\n");
			if(token != NULL){
				loadGame(buffer, token);
				continue;
			}else{
				fprintf(stderr, "Error: No Filename\n");
			}
		}

		//check if user input is a number
		int tokenIsNumber = 1;
		for(int i = 0; i < strlen(token); i++){
			if(!isdigit((int)token[i])){
					tokenIsNumber = 0;
			}
		}

		if(!tokenIsNumber){
			fprintf(stderr, "Error: Invalid Input\n");
		}else{
			int tokenNumber = atoi(token);
			results = playCol(board, width, height, tokenNumber, player);

			//check if out of bounds
			if(results[0] == -1){
				fprintf(stderr, "Error: Exceeds Board Proportions\n");
			}
			//check if column is full
			else if(results[0] == -2){
				fprintf(stderr, "Error: Column Full\n");
			}
			else {
				//check horizontal victory
				checkwin = checkHor(board, height, results[1], connect, 1);
				if(checkwin ==1){
					display(board, width, height);
					fprintf(stderr, "Player %d Wins!\n", player);
					break;
				}

				//check vertical victory
				checkwin = checkVert(board, width, results[0], connect, 1);
				if(checkwin ==1){
					display(board, width, height);
					fprintf(stderr, "Player %d Wins!\n", player);
					break;
				}

				//check diagnal up victory
				checkwin = checkDiagnalUp(board, width, height, results[0],
					results[1], connect, 1);
					if(checkwin ==1){
						display(board, width, height);
						fprintf(stderr, "Player %d Wins!\n", player);
						break;
					}

				//check diagnal down victory
				checkwin = checkDiagnalDown(board, width, height, results[0],
					results[1], connect, 1);
					if(checkwin ==1){
						display(board, width, height);
						fprintf(stderr, "Player %d Wins!\n", player);
						break;
					}

				//no win - swap current player
				if(board[results[0]][results[1]] == 1){
					player = 2;
				}else{
					player = 1;
				}
			}
		}

	}





  return 0;

}
