#ifndef __testFunctions_h__
#define __testFunctions_h__

#include "Validity.h"

/*This is a test to func 1 it builds a single segment with threshold=40 and prints it out*/
void mainQ1();

/*This is a test to func 2 it builds an array of 2 segments with threshold=10 and prints it out*/
void mainQ2();

/*This is a test to func 3 it gets the 2 segments from Q2 main and returns a new img with the changed pixels
and prints them out*/
void mainQ3();

/*This is a test to func 4 it gets a pgmfile name from the command line and returns an img and prints it out*/
void mainQ4(int argc, char** argv);

/*This is a test to func 5 it gets a pgmfile name and a bin file name , reduces the pixels in the img according
to 32 reduced grey levels and copies them to a binary file*/
void mainQ5(int argc, char** argv);

/*This is a test to func 6 it gets a pgm file name, a bin file name with reduced pixels values and decompresses
the pixels and creates a new pgm file*/
void mainQ6(int argc, char** argv);

/*This is a general test that gets a threshold, reduced grey levels, a pgm file name wanted to compress, a bin file
name to create and put the compressed pixels in and a new decompressed pgm file name to create from the commmand
line. it reads an img, finds all segments according to the threshold in the img, creates a new img with colored pixels according to the segments,
it compresses the imgs' pixels into a binary file according to the reduced grey levels given and creates a decompressed
new pgm file with the compressed values in the binary file*/
void pgmFileMain(int argc, char** argv);

#endif 