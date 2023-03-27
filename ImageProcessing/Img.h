#ifndef __Img_h__
#define __Img_h__

#include "Validity.h"

/*This function gets a file name, reads the pgm file and returns a gray image*/
grayImage* readPGM(char* fname);
/*This function gets a file and a letter and if there is no comment, proceeding to a new line*/
void ignoreComment(FILE* file, char* letter);
/*This function gets a file, and two unsigned short - rows and cols, and returns an array of pixels by the file, in size rowsXcols*/
unsigned char** getPixels(FILE* file, unsigned short rows, unsigned short cols);
/*This function gets a file and reads the number of the rows and the number of the cols from it*/
void getRowsAndCols(FILE* file, unsigned short* rows, unsigned short* cols, char* letter);
/*This function get a file name, an image and an unsigned char, and compress it by using the function compress*/
void saveCompressed(char* file_name, grayImage* img, unsigned char reduced_grey_levels);
/*This function gets an image and reduced_grey_levels and compress each pixel in the image according to reduced_grey_levels*/
void compress(grayImage* img, unsigned char reduced_grey_levels);
/*This function gets a binary file and updates it*/
void updateBinFile(FILE* reduced, grayImage* img, unsigned char reduced_grey_levels);
/*This function gets num of Bits and an array of bytes and creates new bytes by the num of the bits*/
void createBytes(unsigned char** bytes, unsigned int index, unsigned char pixel, double numOfBits);
/*This function gets name of file with compressed image and convert it to decompressed pgm image by using decompressPGM function*/
void convertCompressedImgToPGM(char* compressed_file_name, char* pgm_file_name);
/*This function gets a bytes array, an index and num of bits and returns pixel*/
unsigned char getPixel(unsigned char** bytes, unsigned int index, double numOfBits);
/*This function gets name of file with compressed image and convert it to pgm file*/
void decompressPGM(unsigned short rows, unsigned short cols, FILE* newPGM, unsigned char compressedGreyLevels, FILE* compressed, unsigned int compressedSize);

#endif 
