#include "Validity.h"

void checkImg(grayImage* img) {

	if (!img) {
		printf("Error 404: image not found!");
		exit(-1);
	}

}

void checkAlloc(void* ptr) {

	if (!ptr) {
		fprintf(stderr, "Memory allocation error \n");
		exit(MEMORY_ALLOC_ERROR);
	}

}

void checkFileOpeningSuccess(FILE* ptr, char* fname) {

	if (!ptr) {

		fprintf(stderr, "Error opening the file %s \n", fname);
		exit(FILE_OPENING_ERROR);
	}

}

void checkFileValidity(FILE* file) {

	char format[3];

	fscanf(file, "%s", format);/*reading the current format from the file*/
	format[2] = '\0';

	if (format[0] != 'P' || format[1] != '2') {
		printf("invalid file format");
		exit(-1);
	}
		
}

BOOL checkCellValidity(grayImage* img, int row, int col, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey) {


	BOOL valid = TRUE;

	if (row >= img->rows || col >= img->cols || row < 0 || col < 0)/*if the cell is in  whithin the img range*/
		valid = FALSE;
	else if (flagMat[row][col] != 0)/*if the cell wasn't added to another segment*/
		valid = FALSE;
	else if ((unsigned int)abs((int)kernelGrey - ((unsigned int)img->pixels[row][col])) > (unsigned int)threshold)/*if the cell is whithin the threshold*/
		valid = FALSE;

	return valid;
}
void invalidFile() {

	printf("The compressed file is invalid");
	exit(INVALID_FILE);
}
void freeFlagmat(BOOL** flagmat, unsigned int size) {

	unsigned int i = 0;
	while (i < size) {
		free(flagmat[i]);
		i++;
	}
	free(flagmat);
}

void freeImage(grayImage* img) {

	unsigned int j = 0;

	for (j = 0; j < img->rows; j++)
		free(img->pixels[j]);

	free(img->pixels);
}
void freeSegment(Segment* seg) {

	if (!seg->root)
		free(seg);
	else {
		recFreeSegment(seg->root);
		free(seg);
	}
}
void recFreeSegment(treeNode* neighbor) {

	unsigned int i = 0;


	if (!neighbor)
		return;

	while (neighbor && neighbor->similar_neighbors[i]) {
		recFreeSegment(neighbor->similar_neighbors[i]);
		i++;
	}
	free(neighbor->similar_neighbors);
	free(neighbor);
}
void freeSegmentArr(imgPosCell** segment, unsigned int size) {
	unsigned int i = 0;
	while (i < size) {
		freeList(segment[i]);
		i++;
	}

	free(segment);
}
void freeList(imgPosCell* segment) {

	imgPosCell* curr;
	imgPosCell* tmp;
	curr = segment;

	while (curr) {
		tmp = curr->next;
		free(curr);
		curr = tmp;
	}
}

void checkCommandLineArguments(int argc, char** argv) {

	BOOL valid = TRUE;
	if (argc < 2|| argc> 6)
		valid = FALSE;
	
	else {

		if (!checkThreshold(argv[1]))
			valid = FALSE;

		else if (!checkReducedGreyLevels(argv[2]))
			valid = FALSE;

		else if (!checkPgmFileName(argv[3]))
			valid = FALSE;

		else if (!checkReducedfileName(argv[4]))
			valid = FALSE;

	}
	
	if (!valid) {

		printf("invalid command line arguments");
		exit(-1);
	}

}
BOOL checkThreshold(char* threshold) {

	unsigned int i=0, len = strlen(threshold);
	char ch;
	BOOL flag = TRUE;

	if (len > 3)
		flag = FALSE;

	else if(len == 1) {

		ch = threshold[0];
		if (ch < 0 || ch>255)
			flag = FALSE;
	}
	else {

		while (threshold[i] != '\0' && flag) {

			if (threshold[i] < '0' || threshold[i]>'9')
				flag = FALSE;

			i++;
		}

	}
	return flag;
}
BOOL checkReducedGreyLevels(char* RducedGreyLevels) {

	unsigned int i = 0;
	BOOL flag = TRUE;
	unsigned int len = strlen(RducedGreyLevels);
	if (len == 0)
		return FALSE;
	if (len == 1) {
		if (RducedGreyLevels[0] >= '0' && RducedGreyLevels[0] <= '9') {
			if (atoi(RducedGreyLevels) != 2 && atoi(RducedGreyLevels) != 4 && atoi(RducedGreyLevels) != 8)
				flag = FALSE;
			
		}
		else {
			if (RducedGreyLevels[0] != 2 && RducedGreyLevels[0] != 4 && RducedGreyLevels[0] != 8 && RducedGreyLevels[0]
				&&RducedGreyLevels[0] != 16 && RducedGreyLevels[0] != 32 && RducedGreyLevels[0] != 64 && RducedGreyLevels[0] != 128)
				flag = FALSE;
	
		}
	}
	else {
		while (RducedGreyLevels[i] != '\0' && flag) {

			if (RducedGreyLevels[i] < '0' || RducedGreyLevels[i]>'9')
				flag = FALSE;

			i++;
		}
		if (atoi(RducedGreyLevels) != 16 && atoi(RducedGreyLevels) != 32 && atoi(RducedGreyLevels) != 64 && atoi(RducedGreyLevels)!=128)
			flag = FALSE;
		

	}

	return flag;
	
	 

}
BOOL checkPgmFileName(char* pgmFname) {

	FILE* pgmfile;
	pgmfile = fopen(pgmFname, "r");

	BOOL flag=TRUE;

	if (!pgmfile) {
		
			printf("Invalid command line parameters");
			exit(FILE_OPENING_ERROR);
	}

	else {
		char format[3];

		fscanf(pgmfile, "%s", format);
		format[2] = '\0';

		if (format[0] != 'P' || format[1] != '2') 
			flag = FALSE;
		
	}
		
	fclose(pgmfile);
	return flag;

}
BOOL checkReducedfileName(char* fname) {

	BOOL flag = TRUE;
	unsigned int i = 0, len = strlen(fname);

	if (len < 5)
		flag = FALSE;
	else {
		while (fname[i] != '.') 
			i++;
		
		if (strcmp((fname + i), ".bin") != 0)
			flag = FALSE;
		
	}

	return flag;

}
unsigned char getArgument(char* argument) {

	unsigned char res = 0;
	unsigned int len = strlen(argument);

	if (len == 1) {

		if (argument[0] >= '0' && argument[0] <= '9')
			res = atoi(argument);
		else
			res = argument[0];
	}
	else
		res = atoi(argument);

	return res;


}



