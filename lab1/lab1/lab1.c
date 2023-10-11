#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define MAX 100



typedef struct {
	char name[MAX];
	char surname[MAX];
	double bodovi;
} student;

int readNoRows() {
	int counter = 0;
	FILE* datoteka = NULL;
	char buffer[MAX] = { 0 };

	datoteka = fopen("studenti.txt", "r");

	if (!datoteka) {
		printf("File not open");
		return -1;
	}

	while (!feof(datoteka)) {
		fgets(buffer, MAX, datoteka);
		counter++;
	}


	fclose(datoteka);

	return counter;
};




int main() {

	int brred;

	brred = readNoRows;

	printf("%d", readNoRows);

	return 0;
}