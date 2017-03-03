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

//displays the game board
int display(int ** board, int width, int height){
	for(int index=0; index < height; index++){
		for (int i= 0; i < width; i++){
			printf("%d ", board[i][index]);
		}
		printf("\n");
	}
	printf("\n\n");
}

//checks horizontal victory, returns 1 for victory
int checkHor(int** board, int height, int rowNum, int connect, int player){
	int count = 0;
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
	if(colNum <= rowNum){
		offset = colNum;
		startPosy = rowNum - offset;
	}else{
		offset = rowNum;
		startPosx = colNum - offset;
	}
	if((width-startPosx) < (height -startPosy)){
		max = width;
	}else{
		max = height;
	}
	for(int i=0; i < max - offset; i++){
		if((startPosy + i) > (height - 1) || (startPosx + i) > (width - 1)){
			break;
		}
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
	offset = rowNum + colNum;
	startPosy = offset;
	if(offset > height - 1){
		offset = offset - (height -1);
		startPosx = offset;
		startPosy = startPosy - (colNum - offset);
	}
	if((width-startPosx) < (height -startPosy)){
		max = width;
	}else{
		max = height;
	}
	for(int i=0; i < max - offset; i++){
		if((startPosy + i) < 0 || (startPosx + i) > (width - 1)){
			break;
		}
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


int loadGame(char* buffer, char* load){
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
		display(board, width, height);
}
