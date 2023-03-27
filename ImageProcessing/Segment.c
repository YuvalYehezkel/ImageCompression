#include "Segment.h"


unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments) {

	checkImg(img);

	BOOL** flagMat = buildFlagmat(img->rows, img->cols);
	imgPos kernel;
	unsigned char grey;

	Segment* segment;
	int imgSize = (img->cols) * (img->rows);

	imgPos* positions;
	unsigned int posIndex = 0;
	positions = (imgPos*)malloc(sizeof(imgPos));
	checkAlloc(positions);

	imgPosCell* lst;
	unsigned int segIndex = 0;

	unsigned int* sizeArr;
	sizeArr = (unsigned int*)malloc(sizeof(unsigned int));
	checkAlloc(sizeArr);

	while (imgSize > 0) {

		grey = findMinGray(img, flagMat, kernel);/*finding min gray level in the img*/
		segment = findSingleSegment2(&posIndex, &positions, img, kernel, threshold, flagMat, grey);/*building the segment*/
		mergeSortImgPos(positions + 1, posIndex - 1);/*sorting the positions*/
		lst = createList(positions, posIndex);/*creating a list from the segmen*/
		(*segments)[segIndex] = lst;/*updating the array*/
		segIndex++;
		(*segments) = (imgPosCell**)realloc((*segments), (segIndex + 1) * sizeof(imgPosCell*));
		checkAlloc(*segments);
		sizeArr[segIndex - 1] = segment->size;/*updating the size arr*/
		sizeArr = (unsigned int*)realloc(sizeArr, (segIndex + 1) * sizeof(unsigned int));
		checkAlloc(sizeArr);
		imgSize = imgSize - posIndex;/*updating img size*/
		posIndex = 0;
	}
	(*segments) = (imgPosCell**)realloc((*segments), (segIndex) * sizeof(imgPosCell*));
	checkAlloc(*segments);
	sizeArr = (unsigned int*)realloc(sizeArr, (segIndex) * sizeof(unsigned int));
	checkAlloc(sizeArr);
	mergeSortImgPosCell(*segments, sizeArr, segIndex);/*sorting the lists according to size*/
	freeFlagmat(flagMat, (img->rows));
	free(positions);
	free(sizeArr);

	return segIndex;
}

unsigned char findMinGray(grayImage* img, BOOL** flagMat, imgPos kernel) {

	unsigned char min = 255;
	unsigned int i, j;

	for (i = 0; i < img->rows; i++) {

		for (j = 0; j < img->cols; j++) {

			if ((img->pixels[i][j] < min) && (flagMat[i][j] == 0)) {
				min = img->pixels[i][j];
				kernel[0] = i;
				kernel[1] = j;
			}

		}
	}
	return min;
}
Segment* findSingleSegment2(unsigned int* posIndex, imgPos** positions, grayImage* img, imgPos kernel, unsigned char threshold, BOOL** flagMat, unsigned char grey) {


	flagMat[kernel[0]][kernel[1]] = 1;

	Segment* res = buildSegment2(posIndex, positions, img, kernel, threshold, flagMat, grey);
	
	return res;

}

Segment* buildSegment2(unsigned int* posIndex, imgPos** positions, grayImage* img, imgPos kernel, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey) {

	unsigned int count = 0;
	Segment* newSeg;
	newSeg = (Segment*)malloc(sizeof(Segment));
	checkAlloc(newSeg);

	newSeg->root = (treeNode*)malloc(sizeof(treeNode));
	checkAlloc(newSeg->root);

	newSeg->root->similar_neighbors = (treeNode**)malloc(9 * sizeof(treeNode*));
	checkAlloc(newSeg->root->similar_neighbors);

	newSeg->root->position[0] = kernel[0];
	newSeg->root->position[1] = kernel[1];

	(*positions)[0][0] = kernel[0];/*updating the positions array to sort later*/
	(*positions)[0][1] = kernel[1];
	(*posIndex)++;

	recBuildSegment2(posIndex, positions, newSeg->root, img, threshold, flagMat, kernelGrey, &count);/*biuilding the segmnt recursivly*/

	newSeg->size = count;
	return newSeg;
}
void recBuildSegment2(unsigned int* posIndex, imgPos** positions, treeNode* root, grayImage* img, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey, unsigned int* count) {

	unsigned int i = 0;

	(*count)++;
	buildKernelsChildren2(posIndex, positions, img, root, threshold, flagMat, kernelGrey);

	while (root->similar_neighbors[i]) {/*going over each node in the arr*/

		recBuildSegment2(posIndex, positions, root->similar_neighbors[i], img, threshold, flagMat, kernelGrey, count);
		i++;
	}
}
void buildKernelsChildren2(unsigned int* posIndex, imgPos** positions, grayImage* img, treeNode* res, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey) {

	unsigned int i = 0;
	int  writingIndex = 0, rootCol, rootRow;




	for (rootRow = res->position[0] - 1; rootRow <= res->position[0] + 1; rootRow++) {/*checking all 8 surrounding pixels*/
		for (rootCol = res->position[1] - 1; rootCol <= res->position[1] + 1; rootCol++) {

			if (checkCellValidity(img, rootRow, rootCol, threshold, flagMat, kernelGrey)) {/*if the pixel is valid to the seg*/

				*positions = (imgPos*)realloc(*positions, (((*posIndex) + 1) * sizeof(imgPos)));/*updating positions*/
				checkAlloc(*positions);
				res->similar_neighbors[writingIndex] = createNewNode(rootRow, rootCol);/*updating the neighboors*/
				writingIndex++;
				(*positions)[*posIndex][0] = rootRow;
				(*positions)[*posIndex][1] = rootCol;
				(*posIndex)++;
				flagMat[rootRow][rootCol] = 1;/*updating flags*/
			}

		}
	}
	if (writingIndex < 8) {
		res->similar_neighbors = (treeNode**)realloc(res->similar_neighbors, (writingIndex + 1) * sizeof(treeNode*));
		checkAlloc(res->similar_neighbors);
	}

	res->similar_neighbors[writingIndex] = NULL;

}

void mergeSortImgPos(imgPos* positions, unsigned int size) {

	imgPos* ptr;
	unsigned int i;

	if (size <= 1)
		return;

	mergeSortImgPos(positions, size / 2);
	mergeSortImgPos(positions + size / 2, size - (size / 2));

	ptr = (imgPos*)malloc(size * sizeof(imgPos));
	checkAlloc(ptr);

	mergeImgPos(ptr, positions, size / 2, positions + size / 2, size - size / 2);

	for (i = 0; i < size; i++) {
		positions[i][0] = ptr[i][0];
		positions[i][1] = ptr[i][1];
	}
	free(ptr);
}

void mergeImgPos(imgPos* dest, imgPos* left, unsigned int sizeLeft, imgPos* right, unsigned int sizeRight) {

	unsigned int i = 0, j = 0;


	while ((i < sizeLeft) && (j < sizeRight)) {

		if (left[i][1] < right[j][1]) {
			dest[i + j][1] = left[i][1];
			dest[i + j][0] = left[i][0];
			i++;
		}
		else if (left[i][1] == right[j][1]) {

			if (left[i][0] > right[j][0]) {
				dest[i + j][1] = right[j][1];
				dest[i + j][0] = right[j][0];
				j++;
			}
			else {
				dest[i + j][1] = left[j][1];
				dest[i + j][0] = left[j][0];
				i++;
			}

		}
		else {
			dest[i + j][1] = right[j][1];
			dest[i + j][0] = right[j][0];
			j++;
		}
	}
	while (i < sizeLeft) {
		dest[i + j][0] = left[i][0];
		dest[i + j][1] = left[i][1];
		i++;
	}
	while (j < sizeRight) {
		dest[i + j][0] = right[j][0];
		dest[i + j][1] = right[j][1];
		j++;
	}

}
imgPosCell* createList(imgPos* segPositions, unsigned int segSize) {

	imgPosCell* res;
	res = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(res);
	res->next = NULL;
	res->prev = NULL;

	unsigned int i = 0;

	res->position[0] = segPositions[0][0];
	res->position[1] = segPositions[0][1];

	for (i = 1; i < segSize; i++)/*creating the list using the segment*/
		addNodeToEndOfList(res, segPositions[i]);



	return res;
}
void addNodeToEndOfList(imgPosCell* lst, imgPos position) {

	imgPosCell* node;

	node = createNewListNode(position);

	addNodeToEnd(lst, node);

}
imgPosCell* createNewListNode(imgPos pos) {

	imgPosCell* res;
	res = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(res);

	res->position[0] = pos[0];
	res->position[1] = pos[1];

	res->prev = NULL;
	res->next = NULL;

	return res;
}
void addNodeToEnd(imgPosCell* lst, imgPosCell* node) {

	imgPosCell* curr, * temp;

	curr = lst;
	temp = curr;

	while (curr) {
		temp = curr;
		curr = curr->next;

	}

	node->prev = temp;
	temp->next = node;

}
void mergeSortImgPosCell(imgPosCell** segments, unsigned int* arr, unsigned int segSize) {

	imgPosCell** ptr;
	unsigned int i;

	if (segSize <= 1)
		return;

	mergeSortImgPosCell(segments, arr, segSize / 2);
	mergeSortImgPosCell(segments + segSize / 2, arr + segSize / 2, segSize - (segSize / 2));

	ptr = (imgPosCell**)malloc(segSize * sizeof(imgPosCell*));
	checkAlloc(ptr);

	mergeImgPosCell(ptr, segments, segSize / 2, segments + segSize / 2, segSize - segSize / 2, arr, arr + segSize / 2);

	for (i = 0; i < segSize; i++)
		segments[i] = ptr[i];


	free(ptr);
}

void mergeImgPosCell(imgPosCell** dest, imgPosCell** left, unsigned int sizeLeft, imgPosCell** right, unsigned int sizeRight, unsigned int* leftS, unsigned int* rightS) {

	unsigned int i = 0, j = 0;


	while ((i < sizeLeft) && (j < sizeRight)) {

		if (leftS[i] < rightS[j]) {
			dest[i + j] = left[i];
			i++;
		}

		else {
			dest[i + j] = right[j];
			j++;
		}
	}

	while (i < sizeLeft) {
		dest[i + j] = left[i];
		i++;
	}
	while (j < sizeRight) {
		dest[i + j] = right[j];
		j++;
	}

}

grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size) {

	checkImg(img);
	grayImage* res = (grayImage*)malloc(sizeof(grayImage));
	checkAlloc(res);

	unsigned int resSize;
	
	res->cols = img->cols;
	res->rows = img->rows;
	res->pixels = (unsigned char**)malloc((res->rows) * sizeof(unsigned char*));
	checkAlloc(res->pixels);

	unsigned int j = 0;

	for (j = 0; j < res->rows; j++) {
		res->pixels[j] = (unsigned char*)malloc((res->cols)*sizeof(unsigned char));
		checkAlloc(res->pixels[j]);
	}

	fromSegToImg(size, segments, res);

	return res;
}
void fromSegToImg(unsigned int size, imgPosCell** segments, grayImage* res) {

	

	unsigned int i, grey;

	if (size == 1)
		insertGreytoPixel(*segments, 0, res);
	else {
		for (i = 0; i < size; i++) {
			grey = (i * ((255) / (size - 1)));/*getting the wanted grey level to update*/
			insertGreytoPixel(segments[i], grey, res);/*updating the img*/
		}

	}

}

void insertGreytoPixel(imgPosCell* segments, int grey, grayImage* res) {

	imgPosCell* curr;
	unsigned int k = 0, i = 0;

	curr = segments;
	while (curr&& curr->position[0]<res->rows&& curr->position[1]<res->cols) {
		
		res->pixels[curr->position[0]][curr->position[1]] = grey;
		curr = curr->next;
	}


}
Segment* findSingleSegment1(grayImage* img, imgPos kernel, unsigned char threshold) {

	checkImg(img);
	unsigned int rows, cols, rootCol = 0, rootRow = 0;
	unsigned char grey;

	rows = img->rows;
	cols = img->cols;
	BOOL** flagMat = buildFlagmat(img->rows, img->cols);

	flagMat[kernel[0]][kernel[1]] = 1;
	grey = img->pixels[kernel[0]][kernel[1]];

	Segment* res = buildSegment1(img, kernel, threshold, flagMat, grey);

	freeFlagmat(flagMat, rows);
	return res;

}
BOOL** buildFlagmat(unsigned int rows, unsigned int cols) {

	unsigned int i;
	BOOL** flagMat;
	flagMat = (BOOL**)malloc(rows * sizeof(BOOL*));
	checkAlloc(flagMat);

	for (i = 0; i < rows; i++) {
		flagMat[i] = (BOOL*)calloc(cols, sizeof(BOOL));
		checkAlloc(flagMat[i]);
	}
	return flagMat;
}


Segment* buildSegment1(grayImage* img, imgPos kernel, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey) {

	unsigned int count = 0;
	Segment* newSeg;
	newSeg = (Segment*)malloc(sizeof(Segment));
	checkAlloc(newSeg);

	newSeg->root = (treeNode*)malloc(sizeof(treeNode));
	checkAlloc(newSeg->root);
	newSeg->root->similar_neighbors= (treeNode**)malloc(9 * sizeof(treeNode*));
	checkAlloc(newSeg->root->similar_neighbors);

	newSeg->root->position[0] = kernel[0];
	newSeg->root->position[1] = kernel[1];

	recBuildSegment1(newSeg->root, img, threshold, flagMat, kernelGrey, &count);

	newSeg->size = count;
	return newSeg;
}
void recBuildSegment1(treeNode* root, grayImage* img, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey, unsigned int* count) {

	unsigned int i = 0;

	(*count)++;
	buildKernelsChildren1(img, root, threshold, flagMat, kernelGrey);

	while (root->similar_neighbors[i]) {/*going over each node*/

		recBuildSegment1(root->similar_neighbors[i], img, threshold, flagMat, kernelGrey, count);
		i++;
	}
}
void buildKernelsChildren1(grayImage* img, treeNode* res, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey) {


	int  writingIndex = 0, rootCol, rootRow;
	
	for (rootRow = res->position[0] - 1; rootRow <= res->position[0] + 1; rootRow++) {/*checking all 8 surrounding pixels*/
		for (rootCol = res->position[1] - 1; rootCol <= res->position[1] + 1; rootCol++) {

			if (checkCellValidity(img, rootRow, rootCol, threshold, flagMat, kernelGrey)) {/*if the pixel is valid we add it to the segment*/

				res->similar_neighbors[writingIndex] = createNewNode(rootRow, rootCol);
				writingIndex++;
				flagMat[rootRow][rootCol] = 1;
			}

		}
	}
	if (writingIndex < 8) {
		res->similar_neighbors = (treeNode**)realloc(res->similar_neighbors, (writingIndex + 1) * sizeof(treeNode*));
		checkAlloc(res->similar_neighbors);
	}

	res->similar_neighbors[writingIndex] = NULL;

}
treeNode* createNewNode(int row, int col) {

	treeNode* res;
	res = (treeNode*)malloc(sizeof(treeNode));
	checkAlloc(res);

	res->similar_neighbors = (treeNode**)malloc(9 * sizeof(treeNode*));
	checkAlloc(res->similar_neighbors);

	res->position[0] = row;
	res->position[1] = col;


	return res;
}

void printSegment(Segment* res) {

	if (!res->root)
		printf("Empty segment");
	else
		recPrintSegment(res->root);

}
void recPrintSegment(treeNode* root) {

	unsigned int i = 0;
	if (!root)
		return;

	printf("root position: %d %d\n", root->position[0], root->position[1]);
	while (root && root->similar_neighbors[i]) {
		printf("segment pixel position : %d %d\n", root->similar_neighbors[i]->position[0], root->similar_neighbors[i]->position[1]);
		recPrintSegment(root->similar_neighbors[i]);
		i++;
	}


}
void printSegmentsArr(imgPosCell** segments, unsigned int size) {
	unsigned int i = 0;
	while (i < size) {
		printList(segments[i]);
		i++;
	}
}
void printList(imgPosCell* segment) {
	imgPosCell* curr;
	curr = segment;
	while (curr) {
		printf("%d %d\n", curr->position[0], curr->position[1]);
		curr = curr->next;
	}
}
