/* created by Miles De Wind on 02/03/2017
 * Got help from stack overflow and code to deretmine file size
 * was given by Ira Woodring
 * edited -02/27/2017
 * added checks, save and load functions for a game of connect four
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
/*
//displays the game board
int display(int ** board, int width, int height){
	for(int index=0; index < height; index++){
		for (int i= 0; i < width; i++){
			//player 1 is X
			if(board[i][index] == 1){
				printf("X");
			//player 2 is O
			}else if(board[i][index] == 2){
				printf("O");
			}else{
				printf("*");
			}
			//space out board
			printf(" ");
		}
		printf("\n");
	}
	printf("\n\n");
}
*/

//displays the game board - note that his function will display in batches of up
//to 50 columns. Each 50 columns will be labeled with their line numbers below
//for instance columns 0-49;
int display(int ** board, int width, int height){
	int over50 = 0;
	int count = 0;
	if(width > 50){
		over50 = 1;
	}

	//if the width is less than 50 - will display entire board
	if(over50 ==0){
		for(int index=0; index < height; index++){
			for (int i= 0; i < width; i++){
				//player 1 is X
				if(board[i][index] == 1){
					printf("X");
				//player 2 is O
			}else if(board[i][index] == 2){
					printf("O");
				}else{
					printf("*");
				}
				//space out board
				printf(" ");
			}
			printf("\n");
		}



	//if the width is greater than 50 - will display board in batches of up to 50
	//columns	with a label below each batch (columns 0-49, etc)
	}else{

		//continue until width is no longer greater than 50
		while(over50 == 1){
			for(int index=0; index < height; index++){
				for (int i= 0; i < 50; i++){
					//player 1 is X
					if(board[count+i][index] == 1){
						printf("X");
					//player 2 is O
				}else if(board[count+i][index] == 2){
						printf("O");
					}else{
						printf("*");
					}
					//space out board
					printf(" ");
				}
				//fprintf(stderr, "Its Here\n");
				printf("\n");

				//wrote out first 50 columns, create label & spacing
				if(index == height-1){
					printf("\n\n columns %d to %d", count, count + 49);
					printf("\n\n");
				}
			}

			//increment width count and over50
			width -= 50;
			count += 50;
			if(width < 50){
				over50 = 0;
			}
		}
	}

	printf("\n\n");
}

//checks horizontal victory, returns 1 for victory
int checkHor(int** board, int height, int rowNum, int connect, int player){
	int count = 0;
	//go through row and count consecutive plays by that player
	for(int i=0; i < height; i++){
		if(board[i][rowNum] == player){
			count++;
			if(count >= connect){
				return 1;
			}
		}else{
			count = 0;
		}
	}
	return 0;
}


//checks Vertical victory, returns 1 for victory
int checkVert(int** board, int width, int colNum, int connect, int player){
	int count = 0;
	//go through column and count consecutive plays by player
	for(int i=0; i < width; i++){
		if(board[colNum][i] == player){
			count++;
			if(count >= connect){
				return 1;
			}
		}else{
			count = 0;
		}
	}
	return 0;
}

int checkDiagnalDown(int** board, int width, int height,
  int colNum, int rowNum, int connect, int player){
	int count = 0;
	int offset;
	int distToEdge = 0;
	int startPosx = 0;
	int startPosy = 0;
	int startIndex = 0;
	int max = 0;

	//set offset to whichever is closer to the top or left edges
	//set our starting x or y position. (one of these will remain 0)
	if(colNum <= rowNum){
		offset = colNum;
		startPosy = rowNum - offset;
	}else{
		offset = rowNum;
		startPosx = colNum - offset;
	}
	//find out how far we can loop
	if((width-startPosx) < (height -startPosy)){
		max = width;
	}else{
		max = height;
	}

	//loop along remaining width or height
	for(int i=0; i < max - offset; i++){
		//catch if we try to loop too far
		if((startPosy + i) > (height - 1) || (startPosx + i) > (width - 1)){
			break;
		}

		//count consecutive cells possessed by the player, return 1 for victory
		if(board[startPosx + i][startPosy + i] == player){
			count++;
			if(count >= connect){
				return 1;
			}
		}else{
			count = 0;
		}
	}
	return 0;
}

int checkDiagnalUp(int** board, int width, int height,
  int colNum, int rowNum, int connect, int player){
	int count = 0;
	int offset;
	int distToEdge = 0;
	int startPosx = 0;
	int startPosy = 0;
	int startIndex = 0;
	int max = 0;

	//set offset to the sum of the rowNum and colNum, this is our starting y value
	offset = rowNum + colNum;
	startPosy = offset;

	//if our starting y value is greater than the height of the board, then
	//set the starting x value to the extra
	if(offset > height - 1){
		offset = offset - (height -1);
		startPosx = offset;
		startPosy = height-1;
	}

	//find out how far we can loop
	if((width-startPosx) < (height -startPosy)){
		max = width;
	}else{
		max = height;
	}
	for(int i=0; i < max - offset; i++){

		//catch if we go out of bounds
		if((startPosy + i) < 0 || (startPosx + i) > (width - 1)){
			break;
		}

		//count consecutive cells possesed by player, return 1 for victory
		if(board[startPosx + i][startPosy - i] == player){
			count++;
			if(count >= connect){
				return 1;
			}
		}else{
			count = 0;
		}
	}
	return 0;
}

int get_fileSize(char*);

int read_file(char* filename, char**buffer){
  //find size of file & create buffer
  struct stat st;
  stat(filename, &st);
  int size = st.st_size;
  *buffer = malloc(size*sizeof(char));

  //create run through file and save to buffer
  FILE* fp;
  fp = fopen(filename, "r");
  fread(*buffer, size, 1, fp);

  fclose(fp);
  return(size);

}

int write_file(char* filename, char* buffer, int size){

  //create / open new file
  FILE* fp = fopen(filename, "w");

  //loop down from end of buffer & write to file
  for(int i = 0; i < size; i++){
    fwrite(buffer + i, 1, sizeof(char), fp);
  }

  fclose(fp);
  return 0;
}


int saveGame(int** board, int width, int height, int connect, int player,
	 char* save){

	//create a buffer to hold all elements of save file
	char* buffer = malloc((sizeof(char) * width * height) +
		(sizeof(char) * width) + (sizeof(char) * height * 2) +
		(sizeof(char) * connect) + (sizeof(char) * 5));

	//size is the size of the buffer, the +5 accounts for the player number and
	//some of the \n's. \n's are also account for one value of height.
	int size = width * height + width +  2*height + connect + 5;

	//add w's that sum to width
	for (int i= 0; i < width; i++){
		buffer[i] = 'w';
	}
	buffer[width] = '\n';

	//add h's that sum to height
	for (int i= 0; i < height; i++){
		buffer[width + 1 + i] = 'h';
	}
	buffer[width + height + 1] = '\n';

	//add c's that sum to connect
	for (int i= 0; i < connect; i++){
		buffer[width + height + 2 + i] = 'c';
	}
	buffer[width + height + connect + 3] = '\n';

	//add player value
	buffer[width + height + connect + 4] = player + '0';
	buffer[width + height + connect + 5] = '\n';

	//start indexes the buffer so as not to overwrite the w's h's, etc that
	//have already been added.
	int start = width + height + connect + 6;

	//count is used to make space for \n's while in a constant loop.
	int count = 0;
	for(int index=0; index < height; index++){
		for (int i= 0; i < width; i++){

			//the +'0' casts to a char
			buffer[start + count + index * width + i] = board[i][index] + '0';
			if(i + 1 == width){
				buffer[start + count + index * width + i + 1] = '\n';
				count ++;
			}
		}
	}

	write_file(save, buffer, size);
	free(buffer);
}


int** loadGame(char* buffer, char* load){
		int width;
		int height;
		int connect;
		int player;
		int count = 0;
		int count2 = 0;
		int size;

		//read file into buffer
		size = read_file(load, &buffer);

		//count w's to find width
		while(buffer[count] == 'w'){
			count++;
		}
		width = count;
		count++;

		//count h's to find height
		while(buffer[count] == 'h'){
			count++;
			count2++;
		}
		height = count2;
		count2 = 0;
		count++;

		//count c's to find connect
		while(buffer[count] == 'c'){
			count++;
			count2++;
		}
		connect = count2;

		//Usually augment by 1 to get past \n however there is an extra character
		//placed into the save file here / doesn't effect program.
		count = count+2;

		//find player- note: used -'0' to cast to int
		player = buffer[count] - '0';
		count = count+2;

		//create the board / filled with 0's
		int** board = malloc(sizeof(int*) * width);
	 	for(int index=0; index < width; index++){
	 		board[index] = calloc(sizeof(int), height);
	 	}

		//count2 is used to make space for \n's while in  upcoming loop
		count2 = 0;

		//fill the board with load data from buffer
		for(int index =0; index < height; index++){
			for(int i=0; i < width; i++){
				board[i][index] = buffer[count2 + count + (index * width) + i] - '0';
				//board[index][i] = buffer[count2 + count + index + i];
			}
			count2++;
		}
		return board;
}


int * playCol(int** board, int width, int height, int colNum, int player){
	int* results = malloc(sizeof(int) * 2);

	//check if colNum is within the bounds of our board, set results to error -1
	if(colNum > height -1 || colNum < 0)
	{
		results[0] = -1;
		results[1] = -1;
		return results;
	}

	for(int i=0; i < height; i++){
		if(board[colNum][i+1] != 0){

			//if the entire column is full set results to error -2
			if(board[colNum][i] != 0){
				results[0] = -2;
				results[1] = -2;
				break;
			}

			//set lowest open slot in column to player
			board[colNum][i] = player;
			results[0] = colNum;
			results[1] = i;
			break;
		}

		//if the entire column was empty / 0's set last slot to player
		if((i+1) == height && board[colNum][i] == 0){
			board[colNum][i] = player;
			results[0] = colNum;
			results[1] = i;
			break;
		}
	}

	return results;
}
