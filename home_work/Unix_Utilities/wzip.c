#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define MAXFILESIZE 15728640 // for ~15Mb file

#pragma pack(1)  //package exact byte a structure owned, get rid of padding

//5 bytes structure
struct zipFormat { 
	char character;	// 1 byte
	int numCharacter; // 4 bytes
};




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

struct zipFormat *talloc(void) {
	return (struct zipFormat *)malloc(sizeof(struct zipFormat));
}

void printZipOutput(int numCharacter, char character) {
	fwrite(&numCharacter, sizeof(int), 1, stdout);
	printf("%c", character);
}


void encryptZipOneLine(char *line) {

	//struct zipFormat *ret;
	size_t len = strlen(line);

	for(size_t i = 0; i < len+1; i++) {
		char letter = *(line + i);
		int count = 1;
		for(size_t j = i+1; j < len+1; j++) {
			char nextLetter = *(line + j);
			if(letter != nextLetter) {
				// ret = talloc();
				// ret->numCharacter = count;
				// ret->character = letter;
				printZipOutput(count, letter);		
				i = j-1;
				break;
			} else {
				++count;
				if(j == len){
					// ret = talloc();
					// ret->numCharacter = count;
					// ret->character = letter;
					printZipOutput(count, letter);
				}
			}
		}
	}
	//free(ret);
}


int main(int argc, char *argv[]) {

	if(argc < 2) {
		printf("wzip: file1 [file2 ...]\n");
		exit(1);
	}

	char *line = (char*)calloc(MAXFILESIZE, sizeof(char));

	for(int i = 1; i < argc; i++) {
		FILE *fp;
		char *buffer;
		size_t file_size;

		fp = fopen(argv[i], "r");
		if(fp == NULL) {
			printf("wzip: cannot open file\n");
			exit(1);
		}
		file_size = getNumByteOfFile(fp);
		buffer = (char *)calloc(file_size, sizeof(char)); // even though buffer size is always less than file_size
		while(fgets(buffer, file_size+1, fp)) {
			strcat(line, buffer);
		}
		free(buffer);

	}
	encryptZipOneLine(line);
	free(line);

	return 0;
}