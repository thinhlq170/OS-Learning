#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>

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

void checkTermInLine(char *line, char *term) {
	char firstLetterOfTerm = *(term);
	size_t line_len = strlen(line);
	size_t term_len = strlen(term);
	

	for(size_t i = 0; i < line_len; i++) {
		char *word = (char*)calloc(term_len, sizeof(char));
		char letter = *(line+i);
		if(firstLetterOfTerm == letter) {
			strncat(word, line+i, term_len);
			if(strcmp(term, word) == 0) {
				printf("%s", line); //found then break
				break;
			} else {
				free(word);
			}
		}
	}
	
}



int main(int argc, char *argv[]) {

	FILE *fp;
	char *term;
	//size_t term_size;
	size_t buf_size;

	if(argc < 2) {
		printf("wgrep: searchterm [file ...]\n");
		exit(1);
	} else if (argc == 2) {
		fp = stdin;
		//buf_size = ftell(fp);
		char buffer[1000];
		term = argv[1];
		while(fgets(buffer, 1000, fp) != NULL) {
			checkTermInLine(buffer, term);
		}	
	} else {
		fp = fopen(argv[2], "r");
		if(fp == NULL) {
			printf("wgrep: cannot open file\n");
			exit(1);
		}

		buf_size = getNumByteOfFile(fp);
		char *buffer = (char*)calloc(buf_size, sizeof(char));
		term = argv[1];
		while(fgets(buffer, buf_size, fp) != NULL) {
			checkTermInLine(buffer, term);
		}
		fclose(fp);
	}

	return 0;
}