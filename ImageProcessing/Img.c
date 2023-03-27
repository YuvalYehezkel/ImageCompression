#include "Img.h"

void ignoreComment(FILE* file, char* letter) {


	while (*letter == '#') {
		fscanf(file, "%c", letter);
		while (*letter != '#' && *letter != '\n')
			fscanf(file, "%c", letter);

		if (*letter == '\n')
			fscanf(file, "%c", letter);
	}
}
grayImage* readPGM(char* fname) {

	grayImage* res;
	res = (grayImage*)malloc(sizeof(grayImage));
	checkAlloc(res); 

		FILE * imgFile;
	imgFile = fopen(fname, "r");
	checkFileOpeningSuccess(imgFile, fname);

	char letter;
	unsigned int maxGrey;
	unsigned short rows, cols;

	checkFileValidity(imgFile);
	fgetc(imgFile);

	fscanf(imgFile, "%c", &letter);
	if (letter == '#')/*if there is no comment, proceeding to a new line*/
		ignoreComment(imgFile, &letter);

	getRowsAndCols(imgFile, &rows, &cols, &letter);/*updates rows and cols from the file*/

	fscanf(imgFile, "%d", &maxGrey);
	fgetc(imgFile);

	res->cols = cols;
	res->rows = rows;
	if (rows == 0 || cols == 0)
		return NULL;

	res->pixels = getPixels(imgFile, res->rows, res->cols);/*gets the array of pixels*/

	fclose(imgFile);
	return res;

}
unsigned char** getPixels(FILE* file, unsigned short rows, unsigned short cols) {

	int x;
	unsigned char** res;
	res = (unsigned char**)malloc(rows * sizeof(unsigned char*));
	checkAlloc(res);

	unsigned int j = 0, i = 0;
	for (i = 0; i < rows; i++) {
		res[i] = (unsigned char*)malloc(cols * sizeof(unsigned char));
		checkAlloc(res[i]);

	}
	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {

			fscanf(file, "%d", &x);/*gets the pixels' grey level*/
			res[i][j] = x;/*updates the pixel in res*/

		}
	}
	return res;
}
void getRowsAndCols(FILE* file, unsigned short* rows, unsigned short* cols, char* letter) {

	*cols = (*letter) - '0';/*converts to integer*/
	fscanf(file, "%c", letter);/*gets the next letter*/
	while (*letter != ' ') {
		*cols = 10 * (*cols) + ((*letter) - '0'); /*updating cols*/
		fscanf(file, "%c", letter);/*gets the next letter*/
	}
	fscanf(file, "%hu", rows);/*gets rows*/
	fgetc(file);/*gets the next char*/

}
void saveCompressed(char* file_name, grayImage* img, unsigned char reduced_grey_levels) {

	checkImg(img);/*checking if the image is valid*/

	FILE* reduced;
	reduced = fopen(file_name, "wb");
	checkFileOpeningSuccess(reduced, file_name);

	fwrite(&img->rows, sizeof(unsigned short), 1, reduced);
	fwrite(&img->cols, sizeof(unsigned short), 1, reduced);
	fwrite(&reduced_grey_levels, sizeof(unsigned char), 1, reduced);

	compress(img, reduced_grey_levels);/*compressing the image*/
	updateBinFile(reduced, img, reduced_grey_levels);/*updates the binary file*/

	fclose(reduced);

}
void compress(grayImage* img, unsigned char reduced_grey_levels) {

	int i, j;
	for (i = 0; i < img->rows; i++) {

		for (j = 0; j < img->cols; j++)
			img->pixels[i][j] = (img->pixels[i][j] * reduced_grey_levels) / 256;/*compressing the pixels according to reduced_grey_levels*/

	}

}
void updateBinFile(FILE* reduced, grayImage* img, unsigned char reduced_grey_levels) {

	unsigned int numOfBytes, totalPixels, i, j, bitIndex = 0;
	double numOfBits;
	unsigned char* bytes;
	numOfBits = log2(reduced_grey_levels);
	totalPixels = (img->cols) * (img->rows) * numOfBits;

	if (totalPixels % 8 == 0)
		numOfBytes = totalPixels / 8;
	else
		numOfBytes = (totalPixels / 8) + 1;

	bytes = (char*)calloc(numOfBytes, sizeof(char));
	checkAlloc(bytes);

	for (i = 0; i < img->rows; i++) {
		for (j = 0; j < img->cols; j++) {

			createBytes(&bytes, bitIndex, img->pixels[i][j], numOfBits);/*creates bytes arr according to the total number of bits needed */
			bitIndex += numOfBits;/*updating index*/
		}

	}

	for (i = 0; i < numOfBytes; i++)
		fwrite(bytes + i, sizeof(char), 1, reduced);/*writting to bytes from reduced*/

	free(bytes);

}
void createBytes(unsigned char** bytes, unsigned int index, unsigned char pixel, double numOfBits) {

	unsigned int howManyBytes = (index % 8) + numOfBits; /*checking on how many bytes the bits are spread on*/
	unsigned int byteIndex = 0, bitIndex;
	unsigned int leftBits = 0, rightBits = 0;
	char maskRight = 0, maskLeft = 0;

	if (howManyBytes <= 8) {/*less than one byte*/
		byteIndex = index / 8;
		bitIndex = index % 8;
		(*bytes)[byteIndex] = (*bytes)[byteIndex] | pixel << (int)(8 - (bitIndex)-numOfBits);
	}
	else {/*more than one byte*/
		leftBits = 8 - (index % 8);/*checking how many bits in each byte*/
		rightBits = numOfBits - leftBits;
		maskLeft = maskLeft | pixel >> rightBits;
		maskRight = maskRight | pixel << 8 - rightBits;
		byteIndex = index / 8;
		(*bytes)[byteIndex] = (*bytes)[byteIndex] | maskLeft;
		(*bytes)[byteIndex + 1] = (*bytes)[byteIndex + 1] | maskRight;
	}


}void convertCompressedImgToPGM(char* compressed_file_name, char* pgm_file_name) {

	FILE* compressed;
	compressed = fopen(compressed_file_name, "rb");
	checkFileOpeningSuccess(compressed, compressed_file_name);

	unsigned int fileSize;
	fseek(compressed, 0, SEEK_END);
	fileSize = ftell(compressed);/*number of bytes*/
	rewind(compressed);/*gets to beginning of file*/

	if (fileSize < 3)
		invalidFile();

	unsigned short rows, cols;
	unsigned char compressedGreyLevels;
	fread(&rows, sizeof(unsigned short), 1, compressed);/*reading rows*/
	fread(&cols, sizeof(unsigned short), 1, compressed);/*reading cols*/
	fread(&compressedGreyLevels, sizeof(unsigned char), 1, compressed);/*reading compressedGreyLevels*/

	FILE* newPGM;
	newPGM = fopen(pgm_file_name, "w");
	checkFileOpeningSuccess(newPGM, pgm_file_name);

	fprintf(newPGM, "%s\n", "P2");
	fprintf(newPGM, "%hu ", cols);
	fprintf(newPGM, "%hu\n", rows);
	fprintf(newPGM, "%d\n", 255);

	decompressPGM(rows, cols, newPGM, compressedGreyLevels, compressed, fileSize);/*decompress the file*/

	fclose(compressed);
	fclose(newPGM);
}
void decompressPGM(unsigned short rows, unsigned short cols, FILE* newPGM, unsigned char compressedGreyLevels, FILE* compressed, unsigned int compressedSize) {
	unsigned int numOfBytes, totalPixels, i = 0, j = 0, bitIndex = 0;
	double numOfBits;
	unsigned char* bytes;
	numOfBits = log2(compressedGreyLevels);/*the num of bits in use*/
	totalPixels = cols * rows * numOfBits;/*the number of all the bits in use*/
	if (totalPixels % 8 == 0)
		numOfBytes = totalPixels / 8;
	else
		numOfBytes = (totalPixels / 8) + 1;

	bytes = (char*)calloc(numOfBytes, sizeof(char));
	checkAlloc(bytes);
	unsigned char pixel = 0;
	unsigned char numOfGreyValues = 256 / compressedGreyLevels;

	while (ftell(compressed) < compressedSize && i < numOfBytes) {
		fread(&bytes[i], sizeof(unsigned char), 1, compressed);/*reading from compressed to bytes arr*/
		i++;
	}

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) {

			pixel = getPixel(&bytes, bitIndex, numOfBits);/*decompressing the pixel*/
			pixel = pixel * numOfGreyValues;
			pixel += (numOfGreyValues - 1);
			fprintf(newPGM, "%d ", pixel);/*prints to the file*/
			bitIndex += numOfBits;/*updating index*/

		}
		fprintf(newPGM, "\n");/*moving to the next line*/
	}

	free(bytes);
}
unsigned char getPixel(unsigned char** bytes, unsigned int index, double numOfBits) {

	unsigned int howManyBytes = (index % 8) + (int)numOfBits; /*checking how many bytes the bits are spread on*/
	unsigned char res = 0;/*the returned pixel*/
	unsigned int byteIndex = 0, bitIndex;
	unsigned int leftBits = 0, rightBits = 0;

	if (howManyBytes <= 8) {/*one byte or less*/
		byteIndex = index / 8;
		bitIndex = index % 8;
		res = res | (*bytes)[byteIndex] >> (int)(8 - (bitIndex)-numOfBits);
	}
	else {/*more than one byte*/
		leftBits = 8 - (index % 8);/*checking how many bits in each byte*/
		rightBits = numOfBits - leftBits;
		byteIndex = index / 8;
		res = res | (*bytes)[byteIndex] << 8 - leftBits;
		res = res >> (int)8 - leftBits - rightBits;
		res = res | (*bytes)[byteIndex + 1] >> 8 - rightBits;


	}

	return res;
}
