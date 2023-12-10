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


typedef struct person* position;

typedef struct person {
	char name[MAX];
	char surname[MAX];
	int birthYear;

	position next;
}
Person;

int menu(position head);
int addPersonToBeg(position head);
int printList(position firstEl);
int addPersonToEnd(position head);
position findPerson(position firstEl);
int del(position head);
int insertAfterPerson(position firstEl);
int insertBeforePerson(position head);
int sortedInput(position head);
int inputListToFIle(position firstEl);
int inputListFromFile(position head);

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

					continue;

				case '2':

					printList(head->next);

					continue;

				case '3':

					addPersonToEnd(head);

					continue;

				case '4':
					resPerson=findPerson(head->next);
					if (resPerson == NULL)
						printf("\nOsoba nije nadena u listi\n");
					else
						printf("\nOsoba po prezimenu %s je nadena na adresi %p\n", resPerson,resPerson->surname);;

					continue;

				case '5':
					del(head);

					continue;

				case '6':
					insertAfterPerson(head->next);

					continue;

				case '7':
					insertBeforePerson(head);

					continue;

				case '8':
					sortedInput(head);

					continue;

				case '9':
					inputListToFIle(head->next);

					continue;

				case 'a':
					inputListFromFile(head);

					continue;

				case '0':

					printf("\nKraj programa!\n");

					break;

				default:
					printf("\nKrivi unos!!\n");

					continue;
			}
			
			break;
		};


	return 0;
}

int addPersonToBeg(position head){
	position newPerson=NULL;

	newPerson =(Person*)malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	newPerson->next = NULL;

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe:\n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);
	
	newPerson->next = head->next;
	head->next = newPerson;

	return 0;
}

int printList(position firstEl) {
	position current = firstEl;

	printf("\n------------\n"); 
	
	if (current == NULL)
		printf("Lista je prazna!\n");

	else{

		while (current != NULL) {
			printf("%s %s %d\n", current->name, current->surname, current->birthYear);
			current = current->next;
		}
	}

	printf("------------\n");

	return 0;
}

int addPersonToEnd(position head) {

	position newPerson = NULL, current = head;

	newPerson =(Person*)malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (current->next != NULL) {
		current = current->next;
	}

	newPerson->next = current->next;
	current->next = newPerson;

	return 0;
}

position findPerson(position firstEl){
	position current = firstEl;
	char wantedPerson[MAX];

	printf("\nUnesite prezime trazene osobe: \n");
	scanf("\n%s", wantedPerson);

	while (current != NULL && strcmp(current->surname, wantedPerson)!=0) {
		current = current->next;

	}

	return current;
}

int del(position head) {
	Person wantedPersonToDel = { {0},{0},0,NULL };
	position temp=NULL,current = head;

	printf("\nUnesite ime, prezime i god rodenja osobe koju zelite izbrisati iz liste:\n");
	scanf("\n%s %s %d%", wantedPersonToDel.name, wantedPersonToDel.surname, &wantedPersonToDel.birthYear);


	while (current->next != NULL && strcmp(current->next->surname, wantedPersonToDel.surname)!=0 && strcmp(current->next->name, wantedPersonToDel.name)!=0 && current->next->birthYear!=wantedPersonToDel.birthYear){
		
		current = current->next;
	}


	if (current->next == NULL)
		printf("\nTe osoba nema na listi\n");

	else {
		temp = current->next;
		current->next = temp->next;
		printf("\nOsoba %s %s %d je obrisana!\n", wantedPersonToDel.name, wantedPersonToDel.surname, wantedPersonToDel.birthYear);
	
	}

	free(temp);

	return 0;
}

int insertAfterPerson(position firstEl) {
	position newPerson = NULL,current = firstEl;
	Person afterWhichPersonToInsert = { {0},{0},0 };

	newPerson = malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja osobe iza koje zelite umetnuti novu osobu: \n");
	scanf("\n%s %s %d", afterWhichPersonToInsert.name, afterWhichPersonToInsert.surname, &afterWhichPersonToInsert.birthYear);

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (current != NULL && strcmp(current->surname, afterWhichPersonToInsert.surname) != 0 && strcmp(current->name, afterWhichPersonToInsert.name) != 0 && current->birthYear != afterWhichPersonToInsert.birthYear) {
		current = current->next;
	}

	if (current == NULL) {
		printf("\nmOsoba iza koje ste htjeli umetnuti novu osobu ne postoji u listi\n");
		free(newPerson);
	}

	else {
		newPerson->next = current->next;
		current->next = newPerson;
	}

	return 0;
}

int insertBeforePerson(position head) {
	position newPerson = NULL, current = head;
	Person personInFront = { {0},{0},0 };

	newPerson = malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja osobe ispred koje zelite umetnuti novu osobu: \n");
	scanf("\n%s %s %d", personInFront.name, personInFront.surname, &personInFront.birthYear);

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (current->next != NULL && strcmp(current->next->surname, personInFront.surname) != 0 && strcmp(current->next->name, personInFront.name) != 0 && current->next->birthYear != personInFront.birthYear) {
		current = current->next;

	}

	if (current->next == NULL) {
		printf("\nOsoba ispred koje ste htjeli umetnuti novu osobu ne postoji u listi\n");
		free(newPerson);
	}
	
	else {
		newPerson->next = current->next;
		current->next = newPerson;

	}

	return 0;
}

int sortedInput(position head) {
	position newPerson = NULL, current = head;

	newPerson = malloc(sizeof(Person));

	if (!newPerson) {
		printf("\nNeuspjela alokacija memorije za newPerson\n");
		exit(-1);
	}

	printf("\nUnesite ime, prezime i godinu rodenja nove osobe: \n");
	scanf("\n%s %s %d", newPerson->name, newPerson->surname, &newPerson->birthYear);

	while (current->next!= NULL && strcmp(newPerson->surname, current->next->surname) > 0)
		current = current->next;


	newPerson->next = current->next;
	current->next = newPerson;

	return 0;
}

int inputListToFIle(position firstEl) {
	FILE* students = NULL;
	position current = firstEl;

	students = fopen("studenti.txt", "w");

	if (!students) {
		printf("\nFile students unsuccesfully opened!\n");
		return -1;
	}

	while (current != NULL) {
		fprintf(students, "%s %s %d\n", current->name, current->surname, current->birthYear);
		current = current->next;
	}

	fclose(students);

	return 0;
}

int inputListFromFile(position head) {
	FILE* students = NULL;
	position newPerson=NULL, current = head;

	students = fopen("studenti.txt", "r");

	if (!students) {
		printf("\nFile students unsuccesfully opened!\n");
		return -1;
	}

	while (!feof(students)) {

		newPerson = malloc(sizeof(Person));

		fscanf(students, "%s %s %d\n", newPerson->name, newPerson->surname, &newPerson->birthYear);

		newPerson->next = current->next;
		current->next = newPerson;

		current = current->next;
		
	}

	fclose(students);

	return 0;
}