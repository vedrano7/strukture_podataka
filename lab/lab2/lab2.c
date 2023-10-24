/*2. Definirati strukturu osoba(ime, prezime, godina rođenja) i napisati program koji :
A.dinamički dodaje novi element na početak liste,
B.ispisuje listu,
C.dinamički dodaje novi element na kraj liste,
D.pronalazi element u listi(po prezimenu),
E.briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

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
int addPersonToBegg(position P);
int printList(position P);
int addPersonToEnd(position P);
position findPerson(position P);
position del(position P);

int main() {
	Person head = { {0},{0},0,NULL};

	menu(&head);

	return 0;
}

int menu(position head) {
	char pick='\0';
	position person = NULL;

		while (1) {
			printf("\nZa dodati novi element na pocetak liste upisite 1, za ispisati listu upisite 2, za dodati novi element na kraj liste upisite 3, za pronaci odredeni element u listi po prezimenu upisite 4, za izbrisati odredeni element iz liste upisite 5 te upisite 0 za izaci iz programa\n");
			scanf("\n%c", &pick);
			switch (pick){
			
				case '1':

					addPersonToBegg(head);

					break;
				case '2':

					printList(head->next);
					break;

				case '3':

					addPersonToEnd(head);

					break;
				case '4':
					person=findPerson(head->next);
					if (person == NULL)
						printf("\nOsoba nije nadena u listi\n");
					else
						printf("\nOsoba po prezimenu %s je nadena na lokaciji %d\n", person,person->surname);;

					break;

				case '5':
					person=del(head);

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

int addPersonToBegg(position P){
	position newPerson=NULL;

	newPerson =(Person*)malloc(sizeof(Person));
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

	printf("------------");

	return 0;
}

int addPersonToEnd(position P) {

	position newPerson=NULL;

	newPerson =(Person*)malloc(sizeof(Person));

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


position del(position P) {
	Person wantedPersonToDel = { {0},{0},0,NULL };
	position temp=NULL;

	temp = (position)malloc(sizeof(Person));

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

	return P->next;
}
