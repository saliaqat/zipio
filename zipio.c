#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 262144
#define FILENAME_SIZE 512
#define FILE_SUFFIX ".zipio"

#define DEBUG 0
#define DEBUG_PREFIX "tests/output/"

int EndsWith(const char *str, const char *suffix);
void uncompress(const char *str);
void compress(const char *str);
char checkForEscapedChars(const char v, char* vArr);
void getRidOfPrefix(const char* original, char *out);
void getRidOfSuffix(const char* original, char *out);
unsigned char getCharFromDif(const int dif1, const int dif2);
int getFirstValueFromCharEncoding (const char value);
int getSecondValueFromCharEncoding (const char value);

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

	// get rid of /n at end of file name
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
				fprintf(writeFile, "%c", buf[index+1]);
				index+=2;

			}
			// else if(buf[index] == 'r') {
			// 	int start = index;
			// 	int end = index+1;
			// 	while(buf[end] != 'r') {
			// 		end++;
			// 		if(end < BUFFER_SIZE-2 && buf[end] == '\\' && buf[end+1] == 'r') {
			// 			end+=2;
			// 		}
			// 	}
			// 	char value = buf[end-1];

			// 	int numStart = start+1;
			// 	int numEnd = end-1;
			// 	int numLen = numEnd-numStart;
			// 	char num[numLen];
			// 	int numIndex = 0;
			// 	for(int i = numStart; i < numEnd; i++) {
			// 		num[numIndex] = buf[i];
			// 		numIndex++;
			// 	}
			// 	int repeat = atoi(num);


			// 	for(int i = 0; i < repeat; i++) {
			// 		fprintf(writeFile, "%c", value);
			// 	}
			// 	index = end;
			// 	index++;
			// }
			// else if(buf[index] == 'c') {
			// 	index++;
			// 	char startChar = buf[index];
			// 	fprintf(writeFile, "%c", startChar);
			// 	index++;
			// 	while (buf[index] != 'c') {
			// 		char value = buf[index];
			// 		char v1 = getFirstValueFromCharEncoding(value);
			// 		char v2 = getSecondValueFromCharEncoding(value);
			// 		if(startChar +v1 != startChar) {
			// 			fprintf(writeFile, "%c", startChar+v1);						
			// 		}
			// 		if(startChar + v2 != startChar) {
			// 			fprintf(writeFile, "%c", startChar+v2);						
			// 		}
			// 		index++;
			// 	}
			// 	if(buf[index] == 'c') {
			// 		index++;
			// 	}
			// }
			else {
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
			// if(index < BUFFER_SIZE-1 && buf[index] == buf[index+1]) {
			// 	int repeat = 2;
			// 	while(index + repeat < BUFFER_SIZE && buf[index] == buf[index+repeat]) {
			// 		repeat++;
			// 	}
			// 	char val[3];
			// 	checkForEscapedChars(buf[index], val);
			// 	fprintf(writeFile, "r%d%sr", repeat, val);
			
			// 	index+=repeat;
			// }
			// else if(index < BUFFER_SIZE-2 
			// 	&& (buf[index+1]-buf[index]) < 15 && (buf[index+1]-buf[index]) > 0
			// 	&& (buf[index+2]-buf[index]) < 15 && (buf[index+2]-buf[index]) > 0
			// 	&& (buf[index+1] != buf[index+2]) && buf[index+2] !='\0'){
			// 	char startChar = buf[index];
			// 	char val[3];
			// 	checkForEscapedChars(buf[index], val);
			// 	printf("At character: %d\n", buf[index]);
			// 	fprintf(writeFile, "c%s", val);
			// 	while(index < BUFFER_SIZE-1 
			// 		&& buf[index+1] - startChar < 15 
			// 		&& buf[index+1] - startChar > 0 
			// 		&& buf[index+2] != '\0') {
			// 		int dif1 = buf[index+1] - startChar;
			// 		int dif2 = buf[index+2] - startChar;
			// 		printf("Define 1: %d\n", buf[index+1]);
			// 		printf("Define 2: %d\n", buf[index+2]);
			// 		// this corresponds to a compression value (c) which is not allowed
			// 		if(dif1 == 6 && dif2 == 3) {
			// 			break;
			// 		}
			// 		if(index <  BUFFER_SIZE-2 && buf[index+2]-startChar < 15 && buf[index+2]-startChar > 0) {
			// 			char toWrite = getCharFromDif(dif1, dif2);
			// 			fprintf(writeFile, "%c", toWrite);
			// 			index+=2;
			// 		}else {
			// 			int dif2 = 0;
			// 			char toWrite = getCharFromDif(dif1, dif2);
			// 			fprintf(writeFile, "%c", toWrite);
			// 			index++;
			// 		}
			// 	} 
			// 	fprintf(writeFile, "c");
			// 	// if(buf[index] == 'c') {
			// 		index++;					
			// 	// }
			// }
			// else{
				char val[3];
				checkForEscapedChars(buf[index], val);
				fprintf(writeFile, "%s", val);
				index++;	
			// }
		}
		bytes = fgets(buf, BUFFER_SIZE, readFile);
	}
}

int getFirstValueFromCharEncoding (const char value) {
	return (value >> 4) & 0x0F;
}

int getSecondValueFromCharEncoding (const char value) {
	return (value >> 0) & 0x0F;
}

unsigned char getCharFromDif(const int dif1, const int dif2){
	if(dif1 > 15 || dif2 > 15) {
		exit(-1);
	}

	unsigned char returnVal = ((dif1 << 4) & 0xF0) | ((dif2 << 0) & 0x0F);
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
char checkForEscapedChars(const char v, char* vArr) {
	if(v == 'r') {
		vArr[0] = '\\';
		vArr[1] = 'r';
		vArr[2] = '\0';
	}else if(v == '\\') {
		vArr[0] = '\\';
		vArr[1] = '\\';
		vArr[2] = '\0';
	}else if(v == 'c'){
		vArr[0] = '\\';
		vArr[1] = 'c';
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