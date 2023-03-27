#ifndef __Segment_h__
#define __Segment_h__

#include "Validity.h"

/*These functions print the segment*/
void printSegment(Segment* res);
void recPrintSegment(treeNode* root);

/*These functions print the segments arr*/
void printSegmentsArr(imgPosCell** segments, unsigned int size);
void printList(imgPosCell* segment);

/*This function gets an img pointer, an int arr and an unsigned char and return a single segment found in the img*/
Segment* findSingleSegment1(grayImage* img, imgPos kernel, unsigned char threshold);

/*This function gets a pointer to and array of imgPos and a pointer to its' writing index, it gets an img pointer, an 
imgPos that represents a kernel, two unsigned chars- the threshold and the gray value of the kenrl, and an int matrix.
The function returns a pointer to a single segment that was found in the img starting from the kernel given*/
Segment* findSingleSegment2(unsigned int* posIndex, imgPos** positions, grayImage* img, imgPos kernel, unsigned char threshold, BOOL** flagMat, unsigned char grey);

/*This function gets two unsigned integers that represent the number of cols and rows in a wanted matrix and returns
an integer matrix*/
BOOL** buildFlagmat(unsigned int rows, unsigned int cols);

/*This function gets an img pointer , a treeNode pointer, two unsigned chars-the threshold and the kernels' grey level, and
an int matrix and builds a tree starting from the given node*/
void buildKernelsChildren1(grayImage* img, treeNode* res, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey);

/*This function gets two integers and returns a treeNode cointainig those integers*/
treeNode* createNewNode(int row, int col);

/*These are merge sort functions for an imgPos array*/
void mergeSortImgPos(imgPos* positions, unsigned int size);
void mergeImgPos(imgPos* dest, imgPos* left, unsigned int sizeLeft, imgPos* right, unsigned int sizeRight);

/*This function gets an imgPos array and its' size and returns an imgPosCell list created from the arrays' data*/
imgPosCell* createList(imgPos* segPositions, unsigned int segSize);

/*This function gets an imgPosCell list pointer and an imgPos position and adds a new Node containing the positions'
data to the list*/
void addNodeToEndOfList(imgPosCell* lst, imgPos position);

/*This function gets an imgPos position info and returns a new imgPosCell node cintaining th positions' data*/
imgPosCell* createNewListNode(imgPos pos);

/*This function gets an imgPosCell list pointer and an imgPosCell node and adds the node to the end of the list*/
void addNodeToEnd(imgPosCell* lst, imgPosCell* node);

/*These are merge sort functions for for an array of imgPosCell*/
void mergeSortImgPosCell(imgPosCell** segments, unsigned int* arr, unsigned int segSize);
void mergeImgPosCell(imgPosCell** dest, imgPosCell** left, unsigned int sizeLeft, imgPosCell** right, unsigned int sizeRight, unsigned int* leftS, unsigned int* rightS);

/*This function gets an img pointer, an imgPos kernel position, two unsigned chars- the threshold and the
kernels' grey level, and an int matrix and returns a pointer to a single segment that starts from the kernel given in the img*/
Segment* buildSegment1(grayImage* img, imgPos kernel, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey);

/*This function gets a pointer to a treeNode, an img pointer, two unsigned chars- the threshold and the kernels'
grey level, and an int matrix and builds a segment that was found in the img starting from the treeNode
using recursion*/
void recBuildSegment1(treeNode* root, grayImage* img, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey, unsigned int* count);

/*Thsi function gets a pointer to an array of imgPos positions and a pointer to its' writing index, a pointer
to an img, imgPos kernels' position, two unsigned chars - the threshold and the kernels' grey level, and an int matrix
and returns a pointer to a segment that was found in the img starting from the given kernel*/
Segment* buildSegment2(unsigned int* posIndex, imgPos** positions, grayImage* img, imgPos kernel, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey);

/*Thiss function gets a pointer to imgPos positions arr and a pointer to its' writing index, a pointer to a treeNode, 
an img pointer, two unsigned chars- the threshold and the kernels' grey level, an integer matrix and a pointer to
an integer and biulds a segment that was found in the img starting ftom the given node and updates its' size
using recursion*/
void recBuildSegment2(unsigned int* posIndex, imgPos** positions, treeNode* root, grayImage* img, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey, unsigned int* count);

/*This function gets a pointer to a an imgPos array and its' writing index, an img pointer, a treeNode pointer,
2 unsigned chars- the threshold and the kernels' grey level, and an int matrix and builds a tree starting from the node
given in the img*/
void buildKernelsChildren2(unsigned int* posIndex, imgPos** positions, grayImage* img, treeNode* res, unsigned char threshold, BOOL** flagMat, unsigned char kernelGrey);

/*This function gets an img pointer, an unsigned char that represent the threshold an a ponter to
imgPosCell array of pointers. The function returns the number of segments found in the img and updates the
array of segments pointers with each pointer*/
unsigned int findAllSegments(grayImage* img, unsigned char threshold, imgPosCell*** segments);

/*This function gets an img pointer, an int matrix and an imgPos kernels' position and return the min grey level
founs in the img*/
unsigned char findMinGray(grayImage* img, BOOL** flagMat, imgPos kernel);

/*This function geys an img pointer, an array of imgPosCell lists and its' size and returns a pointer to a new 
img with changed grey levels according to the segments*/
grayImage* colorSegments(grayImage* img, imgPosCell** segments, unsigned int size);

/*This function gets an img pointer, an array of imgPosCell lists and its' size and updates the imgs' grey levels
according to the segments*/
void fromSegToImg(unsigned int size, imgPosCell** segments, grayImage* res);

/*This function gets an imgPosCell list an img pointer and an integer that represents the new gray level
and updates the img with the new gray level*/
void insertGreytoPixel(imgPosCell* segments, int grey, grayImage* res);
#endif 
