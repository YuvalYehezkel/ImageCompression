#include "Img.h"
#include"Segment.h"
#include"testFunctions.h"


void main(int argc, char** argv) {

	checkCommandLineArguments( argc, argv);

	unsigned int Qnum;


	printf("Please enter the Q number to test (for the general test enter 0 ) :\n ");

	scanf("%d", &Qnum);

	if (Qnum == 1)
		mainQ1();
	else if (Qnum == 2)
		mainQ2();
	else if (Qnum == 3)
		mainQ3();
	else if (Qnum == 4)
		mainQ4(argc, argv);
	else if (Qnum == 5)
		mainQ5(argc, argv);
	else if (Qnum == 6)
		mainQ6(argc, argv);
	else if (Qnum == 0)
		pgmFileMain(argc, argv);


}