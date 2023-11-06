/*2. Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

/*3. Prethodnom zadatku dodati funkcije:
A. dinamički dodaje novi element iza određenog elementa,
B. dinamički dodaje novi element ispred određenog elementa,
C. sortira listu po prezimenima osoba (!!NAPRAVIT SORTIRANI UNOS!!),
D. upisuje listu u datoteku,
E. čita listu iz datoteke.*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#define MAX 100

struct person;
typedef struct person* position;
typedef struct person {
	char name[MAX];
	char surname[MAX];
	int birthYear;

	position next;
}
Person;

int menu(position head);
int addPersonToBeg(position P);
int printList(position P);
int addPersonToEnd(position P);
position findPerson(position P);
int del(position P);
int insertAfterPerson(position P);
int insertBeforePerson(position P);
int sortedInput(position P);
int inputListToFIle(position P);
int inputListFromFile(position P);

int main() {
	Person head = { {0},{0},0,NULL};

	menu(&head);

	return 0;
}

int menu(position head) {
	char pick='\0';
	position resPerson = NULL;

		while (1) {
			printf("\nZa dodati novi element na pocetak liste upisite 1, za ispisati listu upisite 2, za dodati novi element na kraj liste upisite 3, za pronaci odredeni element u listi po prezimenu upisite 4, za izbrisati odredeni element iz liste upisite 5, za umetnuti novi element iza odredenog elementa upisite 6, za umetnuti novi element ispred odredenog elementa upisite 7, za sortirani unos (od manjeg prema vecem) upisite 8, za upisati listu u datoteku upisite 9, za ispisati listu iz datoteke upisite 'a' te upisite 0 za izaci iz programa\n");
			scanf("\n%c", &pick);
			switch (pick){
			
				case '1':

					addPersonToBeg(head);

					break;
				case '2':

					printList(head->next);
					break;

				case '3':

					addPersonToEnd(head);

					break;
				case '4':
					resPerson=findPerson(head->next);
					if (resPerson == NULL)
						printf("\nOsoba nije nadena u listi\n");
					else
						printf("\nOsoba po prezimenu %s je nadena na lokaciji %d\n", resPerson,resPerson->surname);;

					break;

				case '5':
					del(head);

					break;

				case '6':
					insertAfterPerson(head);

					break;

				case '7':
					insertBeforePerson(head);

					break;

				case '8':
					sortedInput(head);

					break;

				case '9':
					inputListToFIle(head->next);

					break;

				case 'a':
					inputListFromFile(head);

					break;

				case '0':

					printf("\nKraj programa!\n");

					break;
			default:
				printf("\nKrivi unos!!\n");
				menu(head);

			}
			
			getchar();   //doda san ovaj getchar jer kad je  bia Krivi unos uvik bi osta jedan '\n' koji bi zblesia kod (mora bi dva puta upisat 0 da se program zatvori)

			if (pick == '0')
				break;
			
		};


	return 0;
}

int addPersonToBeg(position P){
	position newPerson=NULL;

	newPerson =(Person*)malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	newPerson->next = NULL;

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe:\n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);
	
	newPerson->next = P->next;
	P->next = newPerson;

	return 0;
}

int printList(position P) {

	printf("\n------------\n"); 
	
	if (P == NULL)
		printf("Lista je prazna!\n");

	else{

		while (P != NULL) {
			printf("%s %s %d\n", P->name, P->surname, P->birthYear);
			P = P->next;
		}
	}

	printf("------------\n");

	return 0;
}

int addPersonToEnd(position P) {

	position newPerson=NULL;

	newPerson =(Person*)malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (P->next != NULL) {
		P = P->next;
	}

	newPerson->next = P->next;
	P->next = newPerson;

	return 0;
}

position findPerson(position P){
	
	char wantedPerson[MAX];

	printf("\nUnesite prezime trazene osobe: \n");
	scanf("\n%s", wantedPerson);

	while (P != NULL && strcmp(P->surname, wantedPerson)!=0) {
		P = P->next;

	}

	return P;
	}


int del(position P) {
	Person wantedPersonToDel = { {0},{0},0,NULL };
	position temp=NULL;

	printf("\nUnesite ime, prezime i god rodenja osobe koju zelite izbrisati iz liste:\n");
	scanf("\n%s %s %d%", wantedPersonToDel.name, wantedPersonToDel.surname, &wantedPersonToDel.birthYear);


	while (P->next != NULL && strcmp(P->next->surname, wantedPersonToDel.surname)!=0 && strcmp(P->next->name, wantedPersonToDel.name)!=0 && P->next->birthYear!=wantedPersonToDel.birthYear){
		
		P = P->next;
	}


	if (P->next == NULL)
		printf("\nTe osoba nema na listi\n");

	else {
		temp = P->next;
		P->next = temp->next;
		printf("\nOsoba %s %s %d je obrisana!\n", wantedPersonToDel.name, wantedPersonToDel.surname, wantedPersonToDel.birthYear);
	
	}

	free(temp);

	return 0;
}

int insertAfterPerson(position P) {
	position newPerson = NULL;
	Person personBehind = { {0},{0},0 };

	newPerson = malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja osobe iza koje zelite umetnuti novu osobu: \n");
	scanf("\n%s %s %d", personBehind.name, personBehind.surname, &personBehind.birthYear);

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (P != NULL && strcmp(P->surname, personBehind.surname) != 0 && strcmp(P->name, personBehind.name) != 0 && P->birthYear != personBehind.birthYear) {
		P = P->next;
	}

	if (P == NULL)
		printf("\nmOsoba iza koje ste htjeli umetnuti novu osobu ne postoji u listi\n");


	else {
		newPerson->next = P->next;
		P->next = newPerson;
	}

	return 0;
}

int insertBeforePerson(position P) {
	position newPerson = NULL;
	Person persoInFront = { {0},{0},0 };

	newPerson = malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja osobe ispred koje zelite umetnuti novu osobu: \n");
	scanf("\n%s %s %d", persoInFront.name, persoInFront.surname, &persoInFront.birthYear);

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (P != NULL && strcmp(P->next->surname, persoInFront.surname) != 0 && strcmp(P->next->name, persoInFront.name) != 0 && P->next->birthYear != persoInFront.birthYear) {
		P = P->next;

	}

	if (P == NULL)
		printf("\nmOsoba ispred koje ste htjeli umetnuti novu osobu ne postoji u listi\n");
	
	else {
		newPerson->next = P->next;
		P->next = newPerson;

	}

	return 0;
}

int sortedInput(position P) {
	position newPerson = NULL;

	newPerson = malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (P->next!= NULL && strcmp(newPerson->surname, P->next->surname) > 0) {
		P = P->next;
	}

	newPerson->next = P->next;
	P->next = newPerson;

	return 0;
}

int inputListToFIle(position P) {
	FILE* students = NULL;

	students = fopen("studenti.txt", "w");

	if (!students) {
		printf("\nFile students unsuccesfully opened!\n");
		return -1;
	}

	while (P != NULL) {
		fprintf(students, "%s %s %d\n", P->name, P->surname, P->birthYear);
		P = P->next;
	}

	fclose(students);

	return 0;
}

int inputListFromFile(position P) {
	FILE* students = NULL;
	position newPerson=NULL;

	students = fopen("studenti.txt", "r");

	if (!students) {
		printf("\nFile students unsuccesfully opened!\n");
		return -1;
	}

	while (!feof(students)) {

		newPerson = malloc(sizeof(Person));

		fscanf(students, "%s %s %d\n", newPerson->name, newPerson->surname, &newPerson->birthYear);

		newPerson->next = P->next;
		P->next = newPerson;

		P = P->next;
		
	}

	fclose(students);

	return 0;
}