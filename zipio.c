#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define FILENAME_SIZE 512
#define FILE_SUFFIX ".zipio"

#define DEBUG 0
#define DEBUG_PREFIX "tests/output/"

int EndsWith(const char *str, const char *suffix);
void uncompress(const char *str);
void compress(const char *str);
char getVal(const char v, char* vArr);
void getRidOfPrefix(const char* original, char *out);
void getRidOfSuffix(const char* original, char *out);

int main(int argc, char** argv) {

	// See input params
	printf("%d\n", argc);
	for(int i = 0; i < argc; i++) {
		printf("%s\n", argv[i]);
	}

	// Input params check
	if(argc < 2) {
		printf("Invalid params\n");
		printf("./zipio <file>\n");
		return -1;
	}

	// Check if compress or not	
	if(EndsWith(argv[1], ".zipio")) {
		uncompress(argv[1]);
	}else {
		compress(argv[1]);
	}
}

void uncompress(const char *str) {
	FILE* readFile = fopen(str, "r");
	if(!readFile) {
		printf("File failed to load\n");
		exit(-1);
	}
	unsigned char* buf = malloc(sizeof(unsigned char)*BUFFER_SIZE);
	unsigned char* bytes = fgets(buf, BUFFER_SIZE, readFile);

	int bufIndex = 0;
	while(buf[bufIndex] != '\0') {
		bufIndex++;
	}
	buf[bufIndex-1] = '\0';

	FILE* writeFile = fopen(buf, "w");

	bytes = fgets(buf, BUFFER_SIZE, readFile);
	while(bytes != '\0') {
		int index = 0;
		while(buf[index] != '\0') {
			if(buf[index] == '\\') {
				printf("Found escaped char: %c. Val after: %c\n", buf[index+1], buf[index+2]);
				fprintf(writeFile, "%c", buf[index+1]);
				index+=2;

			}else if(buf[index] == 'r') {
				int start = index;
				int end = index+1;
				while(buf[end] != 'r') {
					end++;
					if(end < BUFFER_SIZE-2 && buf[end] == '\\' && buf[end+1] == 'r') {
						end+=2;
					}
				}
				char value = buf[end-1];

				int numStart = start+1;
				int numEnd = end-1;
				int numLen = numEnd-numStart;
				char num[numLen];
				int numIndex = 0;
				for(int i = numStart; i < numEnd; i++) {
					num[numIndex] = buf[i];
					numIndex++;
				}
				int repeat = atoi(num);

				printf("Repeating Val: %d, val: %c \n", repeat, value);

				for(int i = 0; i < repeat; i++) {
					fprintf(writeFile, "%c", value);
				}
				index = end;
				index++;
			}else {
				fprintf(writeFile, "%c", buf[index]);
				index++;
			}
		}
		bytes = fgets(buf, BUFFER_SIZE, readFile);

	}

}
void compress(const char *str) {
	FILE* readFile = fopen(str, "r");
	if(!readFile) {
		printf("File failed to load\n");
		exit(-1);
	}
	// define write file
	FILE* writeFile;
	// outfilename to hold temporarily so we can define a non messed up file anem
	char outFilename1[FILENAME_SIZE];
	// get rid of prefix (directories) and save result in outFilename1
	getRidOfPrefix(str, outFilename1);
	char newFileName[FILENAME_SIZE];
	// copy filname to newFilname
	strncpy(newFileName, outFilename1, FILENAME_SIZE);
	// Get rid of suffix (file extension) and use that for work
	char outFilename[FILENAME_SIZE];
	getRidOfSuffix(outFilename1, outFilename);
	if(DEBUG) {
		char filename[FILENAME_SIZE];
		strncpy(filename, DEBUG_PREFIX, FILENAME_SIZE);
		strcat(filename, outFilename);
		strcat(filename, FILE_SUFFIX);
		writeFile = fopen(filename, "w");
	}else{
		strcat(outFilename, FILE_SUFFIX);
		writeFile = fopen(outFilename, "w");		
	}
	fprintf(writeFile, "%s\n", newFileName);
	unsigned char* buf = malloc(sizeof(unsigned char)*BUFFER_SIZE);

	unsigned char* bytes = fgets(buf, BUFFER_SIZE, readFile);

	while (bytes != '\0') {
		int index = 0;
		while(buf[index] != '\0') {
			if(index < BUFFER_SIZE-1 && buf[index] == buf[index+1]) {
				int repeat = 2;
				while(index + repeat < BUFFER_SIZE && buf[index] == buf[index+repeat]) {
					repeat++;
				}
				char val[3];
				getVal(buf[index], val);
				fprintf(writeFile, "r%d%sr", repeat, val);
				printf("Repeat\n");
				index+=repeat;
			}else {
				char val[3];
				getVal(buf[index], val);
				fprintf(writeFile, "%s", val);
				index++;	
			}
		}
		bytes = fgets(buf, BUFFER_SIZE, readFile);
	}
}

void getRidOfSuffix(const char* original, char *out) {
	int index = 0;
	int lastDot = 0;
	while(original[index] != '\0') {
		out[index] = original[index];
		if(original[index] == '.') {
			lastDot = index;
		}
		index++;
	}
	out[index] = '\0';
	if(lastDot != 0) {
		out[lastDot] = '\0';		
	}

}
void getRidOfPrefix(const char* original, char *out) {
	int index = 0;
	int start = 0;
	while(original[index] != '\0') {
		if(original[index] == '/') {
			start = index+1;
		}
		index++;
	}
	int j = 0;
	for(int i= start; i < index; i++) {
		out[j] = original[i];
		j++;
	}
	out[j] = '\0';
}
char getVal(const char v, char* vArr) {
	if(v == 'r') {
		vArr[0] = '\\';
		vArr[1] = 'r';
		vArr[2] = '\0';
	}else if(v == '\\') {
		vArr[0] = '\\';
		vArr[1] = '\\';
		vArr[2] = '\0';
	}else {
		vArr[0] = v;
		vArr[1] = '\0';
	}
}

int EndsWith(const char *str, const char *suffix)
{
    if (!str || !suffix)
        return 0;
    size_t lenstr = strlen(str);
    size_t lensuffix = strlen(suffix);
    if (lensuffix >  lenstr)
        return 0;
    return strncmp(str + lenstr - lensuffix, suffix, lensuffix) == 0;
}