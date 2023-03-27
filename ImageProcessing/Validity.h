#ifndef __Validity_h__
#define __Validity_h__

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef int BOOL;

#define  TRUE 1
#define  FALSE 0
#define MEMORY_ALLOC_ERROR -1
#define FILE_OPENING_ERROR -2
#define INVALID_FILE -3


typedef unsigned short imgPos[2];

typedef struct _grayImage {
	unsigned short rows, cols;
	unsigned char** pixels;

} grayImage;

typedef struct _treeNode {
	imgPos position;
	struct _treeNode** similar_neighbors;
} treeNode;

typedef struct _segment {
	treeNode* root;
	unsigned int size;
} Segment;

typedef struct _imgPosCell {
	imgPos position;
	struct _imgPosCell* next, * prev;
}imgPosCell;


/*This function prints out an error message and exits the programm if the compressed file is invalid*/
void invalidFile();

/*This functionn gets a void pointer and prints out an error massege if the memory alloction was unssuccessfull*/
void checkAlloc(void* ptr);

/*This function gets a file pointer and a string and checks wheather the file was opened successfuly*/
void checkFileOpeningSuccess(FILE* ptr, char* fname);

/*This function gets an img pointer and checks wheather it's empty or not*/
void checkImg(grayImage* img);

/*This function gets an img pointer, two integers,two unsigned characters and an integers matrix and checks wheather
a pixel cell is valid*/
BOOL checkCellValidity(grayImage* img, int row, int col, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey);

/*This function gets a file pointer and check wheather the format of the file is valid*/
void checkFileValidity(FILE* file);

/*This function gets an integer matrix and its' size and frees it*/
void freeFlagmat(BOOL** flagmat, unsigned int size);

/*This function gets an img pointer and frees the img*/
void freeImage(grayImage* img);

/*This function gets a segment pointer and frees it*/
void freeSegment(Segment* seg);

/*This function gets a treeNode pointer and frees is using recursion*/
void recFreeSegment(treeNode* neighbor);

/*This function gets an array of imgPosCell its' size and frees it*/
void freeSegmentArr(imgPosCell** segment, unsigned int size);

/*This function gets an imgPosCell list and frees in*/
void freeList(imgPosCell* segment);

/*This function gets the command line arguments entered and checks wheather they are valid*/
void checkCommandLineArguments(int argc, char** argv);

/*This function gets a string and returns 1 if it's valid and 0 otherwise*/
BOOL checkThreshold(char* threshold);

/*This function gets a string and returns 1 if it's valid and 0 otherwise*/
BOOL checkReducedGreyLevels(char* RducedGreyLevels);
/*This function gets a string and returns 1 if it's valid and 0 otherwise*/
BOOL checkPgmFileName(char* pgmFname);
/*This function gets a string and returns 1 if it's valid and 0 otherwise*/
BOOL checkReducedfileName(char* fname);

/*This function gets a string and returns its' numeric value using an unsigned char*/
unsigned char getArgument(char* argument);
#endif 
