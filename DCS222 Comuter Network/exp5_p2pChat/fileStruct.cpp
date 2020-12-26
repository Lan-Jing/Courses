#include "fileStruct.h"

char tarFilePath[nameSize]; // set target file path;

int terminateFlag;

int get_fileSize(FILE *fp) {
	unsigned int curPos = ftell(fp);
	int fileSize = 0;
	fseek(fp, 0, SEEK_END);
	fileSize = ftell(fp);
	fseek(fp, curPos, SEEK_SET);

	return fileSize;
}

// e:\\filename -> fileName
void remove_path(char *fileName) {
	int l = strlen(fileName), start = 0;
	for(int i = 0; i < l; i++) {
		if(fileName[i] == '\\' || fileName[i] == '/')
			start = i;
	}
	char newFileName[nameSize];
	memset(newFileName, 0, sizeof(newFileName));
	memcpy(newFileName, fileName + start + 1, l - start - 1);
	memset(fileName, 0, sizeof(fileName));
	memcpy(fileName, newFileName, strlen(newFileName)+1);
}

// fileName.ext -> fileName,newPart.ext
void strcat_noext(char *fileName, char *newPart) {
	char newFileName[nameSize];
	int ptr = 0, flag = 0;
	for(int i = 0; i < strlen(fileName); i++) {
		if(fileName[i] == '.') {
			flag = i;
			break;
		}
		newFileName[ptr++] = fileName[i];
	}
	for(int i = 0; i < strlen(newPart); i++) {
		newFileName[ptr++] = newPart[i];
	}
	for(int i = flag; i < strlen(fileName); i++) {
		newFileName[ptr++] = fileName[i];
	}
	newFileName[ptr] = '\0';
	memcpy(fileName, newFileName, strlen(newFileName) + 1);
}