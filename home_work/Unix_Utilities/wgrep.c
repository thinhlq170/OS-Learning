#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

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

	FILE *fp;
	char *term;
	size_t term_size;
	size_t buf_size;

	if(argc < 2) {
		fprintf(stderr, "Usage: wgrep <term> <file>...\n");
		exit(1);
	} else if (argc == 2) {
		fp = stdin;
		buf_size = ftell(fp);
	} else {
		fp = fopen(argv[2], "r");
		buf_size = getNumByteOfFile(fp);
	}
	if(fp == NULL) {
		printf("%s\n", strerror(errno));
		exit(1);
	}

	//init buffer
	char b[buf_size];
	char *buffer = b;

	//get term and the first letter in term
	term = argv[1];
	term_size = strlen(term);
	char firstLetterOfTerm = *(argv[1]);
	

	while(getline(&buffer, &buf_size, fp) > 0) {
		size_t i;
		char *word = (char*)calloc(term_size, sizeof(char));
		size_t len = strlen(buffer);
		
		for(i = 0; i < len; i++) {
			char letter = *(buffer+i);
			if(firstLetterOfTerm == letter) {
				strncat(word, buffer+i, term_size);
				if(strcmp(term, word) == 0) {
					printf("%s", buffer);
				} else {
					strcpy(word, "");
				}
			}
		}

		free(word);
	}
	fclose(fp);
	printf("\n");

	return 0;
}