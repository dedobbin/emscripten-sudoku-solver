#include <emscripten.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef void drawState();
uint32_t* sudoku = NULL;

int isLegal(const uint32_t* const ptr, int index, int val, int print)
{
	int w = 9;
	int h = 9;
	int x = index % w;
	int y = index / w;	

	print && printf("try %d at %d,%d: ",val, x, y);

	//sanity check
	if (val < 0){
		print && printf("illegal: < 0\n");
		return 0;
	} else if (ptr[index] != 0){
		print && printf("illegal: not empty\n");
		return 0;
	}
	
	//check row, todo: combine with col?
	for (int i = w * x; i < w * x + w; i++){
		if (ptr[i] == val){
			print && printf("illegal: in row\n");
			return 0;
		}
	}

	//check col
	for (int i = y; i < h * w; i+= w){
		if (ptr[i] == val){
			print && printf("illegal: in col\n");
			return 0;
		}
	}
	
	//check area
	int ax = x / 3; 
	int ay = y / 3; 

	for (int row = ax * 3; row < ax * 3 + 3; row++){
		for (int col = ay * 3; col < ay * 3 + 3; col++){
			int pos = col + row * w;
			if (pos == index){
				continue;
			}
			if (ptr[pos] == val){
				print && printf("illegal: in area\n");
				return 0;
			}
		}
	}
	print && printf("placed\n");
	return 1;
}

int go(uint32_t* ptr, uint32_t pos, int drawCallback)
{	
	while (ptr[pos]){
		pos ++;
	}

	if (pos >= 81){
		return 1;
	}

	for (int i = 1; i < 10; i++){
		if (isLegal(ptr, pos, i, 0)){
			ptr[pos] = i;
			if (drawCallback){
				((drawState*)drawCallback)();
			}
			if (go(ptr, pos, drawCallback)){
				return 1;
			} 
			ptr[pos] = 0;
			
		}
	}
	return 0;
}


EMSCRIPTEN_KEEPALIVE
uint32_t solveSudoku(int drawCallback)
{
	if (go(sudoku, 0, drawCallback)){
		//printf("solved\n");
		return 1;
	} else {
		//printf("impossible\n");
		return 0;
	}
}

EMSCRIPTEN_KEEPALIVE 
uint32_t* initSolver(const uint32_t* const ptr)
{
	sudoku = malloc(sizeof(*sudoku) * 81);
	memcpy(sudoku, ptr, 81 * sizeof(*sudoku));
	
	return sudoku;
}

// int posToIndex(int x, int y, int w, int h)
// {
// 	return x + y * w;
// }

// void indexToPos(int index, int w, int h, int * const output)
// {
// 	output[0] = index / w;
// 	output[1] = index % w;
// }
