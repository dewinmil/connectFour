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

	//create empty board (0 is our non-value)
	int** board = malloc(sizeof(int*) * width);
	for(int index=0; index < width; index++){
		board[index] = calloc(sizeof(int), height);
	}

	//load if tagged with -l filename
	if(load != NULL){
		board = loadGame(buffer, load);
	}

	//sets disp so that we will immediately display our board when loop begins
	int disp =1;

	//loop for user input, broken by user calls, victory, etc.
	while(1){

		//check if we should display board
		if(disp ==1){
				display(board, width, height);
		}
		//set board to be displayed next loop
		disp = 1;
		fprintf(stderr, "Type help to learn more about how to play.\n");
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

		//print off help statement
		if(strcmp(token, "help")==0){
			display(board, width, height);
			fprintf(stderr, "-Enter column number to play that column.\n");
			fprintf(stderr, "-Columns are numbered starting from 0.\n");
			fprintf(stderr, "-Save or load by typing save or load followed by a filename.\n");
			fprintf(stderr, "-You can display the board at any time by typing display.\n");
			fprintf(stderr, "-You can exit the game at any time by typing exit.\n\n");

			//prevent new board from printing right after print statement
			disp =0;
			continue;
		}

		//dispaly board - only useful for when error messages push the board
		//out of view.
		if(strcmp(token, "display")==0){
			display(board, width, height);
			//prevents the board from re-printing immediately.
			disp =0;
			continue;
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

		//check for load
		if(strcmp(token, "load")==0){
			//increment token
			token = strtok(NULL, "\n");
			if(token != NULL){
				board = loadGame(buffer, token);
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

		//user input is not a number / also isn't a valid call as it would have
		//been caught earlier
		if(!tokenIsNumber){
			fprintf(stderr, "Error: Invalid Input\n\n");
			disp = 0;

		//user is a valid token, attempt to play a column
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
				checkwin = checkHor(board, height, results[1], connect, player);
				if(checkwin ==1){
					display(board, width, height);
					fprintf(stderr, "Player %d Wins!\n", player);
					break;
				}

				//check vertical victory
				checkwin = checkVert(board, width, results[0], connect, player);
				if(checkwin ==1){
					display(board, width, height);
					fprintf(stderr, "Player %d Wins!\n", player);
					break;
				}

				//check diagnal up victory
				checkwin = checkDiagnalUp(board, width, height, results[0],
					results[1], connect, player);
					if(checkwin ==1){
						display(board, width, height);
						fprintf(stderr, "Player %d Wins!\n", player);
						break;
					}

				//check diagnal down victory
				checkwin = checkDiagnalDown(board, width, height, results[0],
					results[1], connect, player);
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
