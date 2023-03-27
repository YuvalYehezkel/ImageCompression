#include "testFunctions.h"
#include "Img.h"
#include"Segment.h"

void mainQ1() {

	Segment* res;

	grayImage img;
	img.cols = 4;
	img.rows = 2;

	img.pixels = (char**)malloc(img.rows * sizeof(char*));
	checkAlloc(img.pixels);

	unsigned int i;

	for (i = 0; i < img.rows; i++) {
		img.pixels[i] = (char*)malloc(img.cols * sizeof(char));
		checkAlloc(img.pixels[i]);

	}
	img.pixels[0][0] = 255;
	img.pixels[0][1] = 192;
	img.pixels[0][2] = 0;
	img.pixels[0][3] = 56;
	img.pixels[1][0] = 120;
	img.pixels[1][1] = 40;
	img.pixels[1][2] = 78;
	img.pixels[1][3] = 89;

	imgPos kernel;
	kernel[0] = 1;
	kernel[1] = 1;


	unsigned char threshold = 40;


	res = findSingleSegment1(&img, kernel, threshold);


	printSegment(res);
	freeSegment(res);
	freeImage(&img);

}
void mainQ2() {


	unsigned char threshold = 10;
	unsigned int size;

	grayImage img;
	img.cols = 4;
	img.rows = 2;

	img.pixels = (char**)malloc(img.rows * sizeof(char*));
	checkAlloc(img.pixels);

	unsigned int i;

	for (i = 0; i < img.rows; i++) {
		img.pixels[i] = (char*)malloc(img.cols * sizeof(char));
		checkAlloc(img.pixels[i]);

	}
	img.pixels[0][0] = 255;
	img.pixels[0][1] = 192;
	img.pixels[0][2] = 0;
	img.pixels[0][3] = 56;
	img.pixels[1][0] = 120;
	img.pixels[1][1] = 40;
	img.pixels[1][2] = 78;
	img.pixels[1][3] = 89;

	imgPos kernel;
	kernel[0] = 1;
	kernel[1] = 1;

	imgPosCell** segments;
	segments = (imgPosCell**)malloc(sizeof(imgPosCell*));
	checkAlloc(segments);
	segments[0] = NULL;

	size = findAllSegments(&img, threshold, &segments);
	printSegmentsArr(segments, size);
	freeSegmentArr(segments, size);
	freeImage(&img);





}
void mainQ3() {

	
	grayImage* img;
	img = (grayImage*)malloc(sizeof(grayImage));
	checkAlloc(img);

	grayImage* img1;
	img->cols = 3;
	img->rows = 2;

	int size = 2;

	imgPosCell* segment1;
	imgPosCell* segment2;
	imgPosCell* segment3;

	imgPosCell* segment4;
	imgPosCell* segment5;
	imgPosCell* segment6;

	segment1 = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(segment1);
	segment1->prev = NULL;
	segment1->next = NULL;
	segment1->position[0] = 1;
	segment1->position[1] = 1;


	segment2 = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(segment2);
	segment2->next = segment2->prev = NULL;
	segment2->position[0] = 1;
	segment2->position[1] = 5;


	segment3 = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(segment3);
	segment3->next = segment3->prev = NULL;
	segment3->position[0] = 2;
	segment3->position[1] = 1;


	segment4 = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(segment4);
	segment4->next = segment4->prev = NULL;
	segment4->position[0] = 2;
	segment4->position[1] = 2;


	segment5 = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(segment5);
	segment5->next = segment5->prev = NULL;
	segment5->position[0] = 2;
	segment5->position[1] = 0;


	segment6 = (imgPosCell*)malloc(sizeof(imgPosCell));
	checkAlloc(segment6);
	segment6->next = segment6->prev = NULL;
	segment6->position[0] = 1;
	segment6->position[1] = 3;

	segment1->next = segment2;
	segment2->prev = segment1;
	segment2->next = segment3;

	segment4->next = segment5;
	segment5->next = segment6;

	segment3->prev = segment2;
	segment5->prev = segment4;
	segment6->prev = segment5;

	imgPosCell** segments;
	segments = (imgPosCell**)malloc(2 * sizeof(imgPosCell*));
	segments[0] = segment1;
	segments[1] = segment4;

	img1 = colorSegments(img, segments, size);

	unsigned int i = 0, j = 0;

	for (i = 0; i < img1->rows; i++) {
		for (j = 0; j < img1->cols; j++)
			printf("%d, %c \n", img1->pixels[i][j], img1->pixels[i][j]);
		
	}
	

	free(segment1);
	free(segment2);
	free(segment3);
	free(segment4);
	free(segment5);
	free(segment6);
	free(segments);

	freeImage(img1);
	free(img);

}
void mainQ4(int argc, char** argv) {


	grayImage* image;

	image = readPGM(argv[3]);

	unsigned int i = 0, j = 0;

	for (i = 0; i < image->rows; i++) {
		for (j = 0; j < image->cols; j++)
			printf("%d ", image->pixels[i][j]);

		printf("\n");
	}
	



	freeImage(image);


}
void mainQ5(int argc, char** argv) {

	

	grayImage img;
	img.cols = 4;
	img.rows = 2;

	img.pixels = (char**)malloc(img.rows * sizeof(char*));
	checkAlloc(img.pixels);

	unsigned int i;

	for (i = 0; i < img.rows; i++) {
		img.pixels[i] = (char*)malloc(img.cols * sizeof(char));
		checkAlloc(img.pixels[i]);

	}
	img.pixels[0][0] = 255;
	img.pixels[0][1] = 192;
	img.pixels[0][2] = 0;
	img.pixels[0][3] = 56;
	img.pixels[1][0] = 120;
	img.pixels[1][1] = 40;
	img.pixels[1][2] = 78;
	img.pixels[1][3] = 89;


	unsigned char reducedGreyLevels = 32;
	saveCompressed(argv[4], &img, reducedGreyLevels);
	freeImage(&img);

}
void mainQ6(int argc, char** argv) {


	convertCompressedImgToPGM(argv[4], argv[5]);


}

void pgmFileMain(int argc, char** argv) {



	grayImage* img;
	img = readPGM(argv[3]);

	grayImage* newImg;

	imgPosCell** segments;
	segments = (imgPosCell**)malloc(sizeof(imgPosCell*));
	checkAlloc(segments);
	segments[0] = NULL;

	unsigned char threshold = getArgument(argv[1]);

	unsigned int count = findAllSegments(img, threshold, &segments);

	newImg = colorSegments(img, segments, count);

	unsigned char reducedGrayLevels = getArgument(argv[2]);
	saveCompressed(argv[4], img, reducedGrayLevels);

	convertCompressedImgToPGM(argv[4], argv[5]);

	freeImage(img);
	freeSegmentArr(segments, count);


}



