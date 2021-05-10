#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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

void wcat_func(char *file_path) {

	char *buffer;
	long numByte;
	FILE *fp;

	fp = fopen(file_path, "r");
	if(fp == NULL) {
		printf("wcat: cannot open file\n");
		exit(1);
	}

	// get file size
	numByte = getNumByteOfFile(fp);

	buffer = (char*)calloc(numByte, sizeof(char));

	while(fgets(buffer, numByte, fp)) {
		printf("%s", buffer);
	}

	fclose(fp);
	free(buffer);
}

int main(int argc, char *argv[]) {

	for(int i = 1; i < argc; i++) {
		wcat_func(argv[i]);
	}

    return 0;
}
