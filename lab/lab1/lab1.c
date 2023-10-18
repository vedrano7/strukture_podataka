#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define MAX 100
#define MAXBRBOD 80



typedef struct {
	char name[MAX];
	char surname[MAX];
	int bodovi;
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

void unos(student* baza) {
	FILE* datoteka = NULL;
	int i = 0;

	datoteka = fopen("studenti.txt", "r");

	while (!feof(datoteka)) {
		fscanf(datoteka,"%s %s %d", baza[i].name,baza[i].surname,&baza[i].bodovi);
		i++;
	}

	fclose(datoteka);
}

void ispis(student* baza,int brred) {
	double relbod;
	int i;

	for(i=0;i<brred;i++) {
		relbod = ((double)baza[i].bodovi / MAXBRBOD) * 100;

		printf("\n Ucenik %s %s je imao apsolutan broj bodova od %d i relativan od %lf\n", baza[i].name, baza[i].surname, baza[i].bodovi,relbod);
	}

}


int main() {

	int brred;
	student* baza;


	brred = readNoRows();

	baza = malloc(sizeof(student) * brred);

	unos(baza);

	ispis(baza,brred);


	return 0;
}

