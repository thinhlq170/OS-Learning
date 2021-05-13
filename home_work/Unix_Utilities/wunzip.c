#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define MAXFILESIZE 15728640

#pragma pack(1)  //package exact byte a structure owned, get rid of padding

struct zipFormat {
	int numCharacter;
	char character;
};

struct zipFormat *talloc(size_t size) {
	return (struct zipFormat*)malloc(size);
}


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
	return numByte+1;
}

void decompressFile(FILE *fp) {

	struct zipFormat *buffer = talloc(MAXFILESIZE);
	size_t file_size = getNumByteOfFile(fp);
	size_t items;

	while((items = fread(buffer, sizeof(struct zipFormat), file_size, fp)) > 0) {
		for(size_t j = 0; j < items+1; j++) {

			struct zipFormat *curBuf = buffer + j;
			int numCharacter = curBuf->numCharacter;
			char character = curBuf->character;

			for(size_t i = 0; i < (size_t)numCharacter; i++) {
				printf("%c", character);
			}
		}
	}
	free(buffer);
}


int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("wunzip: file1 [file2 ...]\n");
		exit(1);
	}

	FILE *fp;
	

	for(int i = 1; i < argc; i++) {
		fp = fopen(argv[i], "r");
		if(fp == NULL) {
			printf("wgrep: cannot open file\n");
			exit(1);
		}

		decompressFile(fp);
		fclose(fp);
	}

	return 0;
}