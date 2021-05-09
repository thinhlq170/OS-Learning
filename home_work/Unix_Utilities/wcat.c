#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// int main(int argc, char *argv[]) {

// 	char *buffer;
// 	long numByte;
// 	FILE *fp;

// 	if(argc != 2) {
// 		fprintf(stderr, "Usage: wcat prints a file's content\n");
// 		exit(1);
// 	}

// 	fp = fopen(argv[1], "r");
// 	if(fp == NULL) {
		
// 	}

// 	// get file size
// 	if(fseek(fp, 0, SEEK_END) != 0) {
// 		printf("%s\n", strerror(errno));
// 		exit(1);
// 	}
// 	numByte = ftell(fp);

// 	//reset offset to the first byte
// 	fseek(fp, 0, SEEK_SET);

// 	buffer = (char*)calloc(numByte, sizeof(char));

// 	fread(buffer, sizeof(char), numByte, fp);

// 	printf("%s\n", buffer);

// 	fclose(fp);

// 	free(buffer);

//     return 0;
// }

void err_mes() {
	printf("%s\n", strerror(errno));
	exit(1);
}

size_t getNumByteOfFile(FILE *fp) {
	size_t numByte;

	//get number of byte
	if(fseek(fp, 0, SEEK_END) != 0) {
		err_mes();
	}
	if(ftell(fp) == -1) {
		err_mes();
	}
	numByte = ftell(fp);

	//reset to the first offset
	fseek(fp, 0, SEEK_SET);
	return numByte;
}

int main(int argc, char *argv[]) {

	char *buffer;
	long numByte;
	FILE *fp;

	if(argc != 2) {
		fprintf(stderr, "Usage: wcat <file>\n");
		exit(1);
	}

	fp = fopen(argv[1], "r");
	if(fp == NULL) {
		printf("Cannot open file\n");
		exit(1);
	}

	// get file size
	numByte = getNumByteOfFile(fp);

	buffer = (char*)calloc(numByte, sizeof(char));

	while(fgets(buffer, numByte, fp)) {
		printf("%s", buffer);
	}
	printf("\n");

	fclose(fp);

	free(buffer);

    return 0;
}