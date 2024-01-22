/*
"BookHaven" je knjižnica koja želi unaprijediti svoj sustav praćenja knjiga, autora i korisnika.
Svaka knjiga ima informacije o naslovu, autoru, godini izdanja i dostupnim primjercima.
Korisnici mogu posuđivati knjige i vratiti ih nakon nekog vremena.

Program mora korisniku omogućiti :
a) ispis svih knjiga abecedno(tako da se mogu vidjeti svi podatci o knjizi i korisnici koji trenutno posuđuju knjigu)
b) ispis svih korisnika abecedno(tako da se mogu vidjeti sve knjige koje je neki korisnik posudio)
c) pretraživanje knjiga po godini izdanja(tako da se mogu vidjeti sve knjige iz te godine i njihova količina)
d) pretraživanje knjiga po nazivu autora(tako da se mogu vidjeti sve knjige tog autora i njihova količina)
e) unos novog korisnika
f) posudba knjige korisniku
- može više primjeraka iste knjige posuditi od puta
- korisnik u sebe nikad ne smije imati više od 5 knjiga sve skupa
- ako pokušava napravit posudbu koja će ga dovesti preko tog broja program treba upozoriti korisnika na to i ispisati mu broj koliko još knjiga posudit
- ako korisnik pita za više primjeraka knjige, a knjižnica na stanju nema nema dovoljno treba korisniku omogućiti izbor hoće li posudit to što ima ili ne.
g) povratak knjiga na stanje
h) spremi(u datoteku / datoteke spremiti trenutno stanje u knjižnici, tako da kad se idući put pokrene program moguće nastaviti od spremljenog stanja)

Sami odaberite strukturu podataka i format datoteka koju će te koristiti za ovo izvesti.
*/


#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_NAME_SIZE (50)
#define MAX_NO_BORROWED_BOOKS (5)
#define MAX_LINE_SIZE (1024)
#define MAX_NO_BOOK_COPIES (20)



typedef struct BookNode* bookPosition;
typedef struct BookNode {

	char bookTitle[MAX_NAME_SIZE];
	char authorName[MAX_NAME_SIZE];
	int publicationYear;
	int numOfAvailableCopies;
	char * usersBorrowingBook[MAX_NO_BOOK_COPIES];

	bookPosition next;
}bookNode;

typedef struct BorrowedBook* positionBorrowedBook;
typedef struct BorrowedBook{

	char bookTitle[MAX_NAME_SIZE];
	int numOfBorrowedCopies;

}borrowedBook;

typedef struct UserNode* userPosition;
typedef struct UserNode {
	char userName[MAX_NAME_SIZE];
	positionBorrowedBook borrowedBooks[MAX_NO_BORROWED_BOOKS];
	int totalNumOfBorrowedBooks;

	userPosition next;
}userNode;


bookPosition createBookNode(char* bookTitle, char* authorName, int publicationYear, int numOfAvailableCopies);
userPosition createUserNode(char* userName);
int readBooksFromFileAndMakeList(char* filename, bookPosition headBookList);
int bookListSortInsert(bookPosition headBookList, char* bookTitle, char* authorName, int publicationYear, int numOfAvailableCopies);
int readUsersFromFileAndMakeList(char* filename, userPosition headUserList, bookPosition headBookList);
userPosition userListSortInsert(userPosition headUserList, char* userName, int totalNumOfBorrowedBooks);
bookPosition findBookNode(bookPosition firstBook, char* bookTitle);
int addNewUserThatBorrowedBookToBookNode(bookPosition bookNode, char* userName);
int addNewBorrowedBookToUserNode(userPosition userNode, char* bookTitle, int numOfBorrowedCopies);
int menu(bookPosition headBookList, userPosition headUserList);
int printBooksAlphabetically(bookPosition firstBook);
int printUsersAlphabetically(userPosition firstUser);
int printBooksOfParticularYear(bookPosition firstBook, int wantedYear);
int printBooksOfParticularAuthor(bookPosition firstBook, char* wantedAuthor);
int insertNewUser(userPosition headUserList);
int borrowBooks(bookPosition headBookList, userPosition headUserList);
userPosition findUserNode(userPosition firstUser, char* userName);
int returnBook(bookPosition headBookList, userPosition headUserList);
int delUserThatBorrowedBookFromBookNode(bookPosition bookNode, char* userName);

int main() {
	bookPosition headBookList = createBookNode(" "," ",0,0);
	userPosition headUserList = createUserNode(" ");

	readBooksFromFileAndMakeList("book_database.txt",headBookList);
	readUsersFromFileAndMakeList("user_database.txt", headUserList,headBookList);

	menu(headBookList, headUserList);

	free(headBookList);
	free(headUserList);

	return EXIT_SUCCESS;
}

bookPosition createBookNode(char* bookTitle, char* authorName, int publicationYear, int numOfAvailableCopies) {
	bookPosition newBookNode = NULL;
	int i = 0;

	newBookNode = malloc(sizeof(bookNode));

	if (!newBookNode) {
		printf("\nUnable to allocate memory for newBookNode!\n");
		return NULL;
	}

	strcpy(newBookNode->authorName, authorName);
	strcpy(newBookNode->bookTitle, bookTitle);
	newBookNode->publicationYear = publicationYear;
	newBookNode->numOfAvailableCopies = numOfAvailableCopies;

	for (; i < MAX_NO_BOOK_COPIES; i++)
		newBookNode->usersBorrowingBook[i] = NULL;

	newBookNode->next = NULL;

	return newBookNode;
}

userPosition createUserNode(char* userName) {
	userPosition newUserNode = NULL;
	int i = 0;

	newUserNode = malloc(sizeof(userNode));

	if (!newUserNode) {
		printf("\nUnable to allocate memory for newUserNode!\n");
		return NULL;
	}

	strcpy(newUserNode->userName, userName);
	newUserNode->totalNumOfBorrowedBooks = 0;

	for (; i < MAX_NO_BORROWED_BOOKS; i++) {
		newUserNode->borrowedBooks[i] = malloc(sizeof(borrowedBook));
		strcpy(newUserNode->borrowedBooks[i]->bookTitle, " ");
		newUserNode->borrowedBooks[i]->numOfBorrowedCopies = 0;
	}

	newUserNode->next = NULL;

	return newUserNode;
}

int readBooksFromFileAndMakeList(char* filename, bookPosition headBookList) {
	FILE* bookFile = NULL;
	char buffer[MAX_LINE_SIZE] = { 0 }, bookTitle[MAX_NAME_SIZE] = { 0 }, authorName[MAX_NAME_SIZE] = { 0 };
	int publicationYear=0, numOfAvailableCopies=0;
	bookFile = fopen(filename, "r");

	if (!bookFile) {
		printf("\nUnable to open file bookFile!\n");
		return EXIT_FAILURE;
	}

	while (!feof(bookFile)) {
		fgets(buffer, MAX_LINE_SIZE, bookFile);
		sscanf(buffer, "%[^,]%*c%[^,]%*c%d %d", bookTitle, authorName, &publicationYear, &numOfAvailableCopies);

		bookListSortInsert(headBookList, bookTitle, authorName, publicationYear, numOfAvailableCopies);
	}

	fclose(bookFile);

	return EXIT_SUCCESS;
}

int bookListSortInsert(bookPosition headBookList, char* bookTitle, char* authorName, int publicationYear, int numOfAvailableCopies) {
	bookPosition newNode = NULL, current = headBookList;

	newNode = createBookNode(bookTitle, authorName, publicationYear, numOfAvailableCopies);

	while (current->next != NULL && strcmp(newNode->bookTitle, current->next->bookTitle) > 0)
		current = current->next;

	newNode->next = current->next;
	current->next = newNode;

	return EXIT_SUCCESS;
}

int readUsersFromFileAndMakeList(char* filename, userPosition headUserList, bookPosition headBookList) {
	FILE* userFile = NULL;
	char userName[MAX_NAME_SIZE] = { 0 }, bookTitle[MAX_NAME_SIZE] = { 0 };
	char* buffer = NULL;
	int totalNumOfBorrowedBooks = 0, numOfBorrowedCopies = 0, itemsRead = 0, numOfBytes = 0,i=0;
	userPosition newUserNode = NULL;
	bookPosition bookNode = NULL;

	userFile = fopen(filename, "r");

	if (!userFile) {
		printf("\nUnable to open file bookFile!\n");
		return EXIT_FAILURE;
	}

	while (!feof(userFile)) {
		buffer = malloc(sizeof(char) * MAX_LINE_SIZE);

		fgets(buffer, MAX_LINE_SIZE, userFile);
		sscanf(buffer, "%[^,]%*c%d %n", userName, &totalNumOfBorrowedBooks,&numOfBytes);
		buffer += numOfBytes;

		newUserNode = userListSortInsert(headUserList,userName, totalNumOfBorrowedBooks);

		if (totalNumOfBorrowedBooks > 0) {
	
			do {

				itemsRead = sscanf(buffer, "%[^,]%*c%d %n", bookTitle, &numOfBorrowedCopies, &numOfBytes);

				if (itemsRead == 2) {

					bookNode = findBookNode(headBookList->next, bookTitle);
					addNewUserThatBorrowedBookToBookNode(bookNode, userName);

					addNewBorrowedBookToUserNode(newUserNode, bookTitle, numOfBorrowedCopies);
					buffer += numOfBytes;
				}
			
			} while (itemsRead == 2);
		}
	}

	fclose(userFile);

	return EXIT_SUCCESS;
}

userPosition userListSortInsert(userPosition headUserList, char* userName, int totalNumOfBorrowedBooks) {
	userPosition newNode = NULL, current = headUserList;

	newNode = createUserNode(userName);
	newNode->totalNumOfBorrowedBooks = totalNumOfBorrowedBooks;

	while (current->next != NULL && strcmp(newNode->userName, current->next->userName) > 0)
		current = current->next;

	newNode->next = current->next;
	current->next = newNode;

	return newNode;
}

bookPosition findBookNode(bookPosition firstBook,char * bookTitle) {
	bookPosition current = firstBook;

	while (current != NULL && strcmp(bookTitle,current->bookTitle)!=0)
		current = current->next;

	return current;
}

int addNewUserThatBorrowedBookToBookNode(bookPosition bookNode, char* userName) {
	int i = 0;

	for (; i < MAX_NO_BORROWED_BOOKS; i++) {

		if (bookNode->usersBorrowingBook[i] == NULL) {
			bookNode->usersBorrowingBook[i] = malloc(sizeof(char) * MAX_NAME_SIZE);
			strcpy(bookNode->usersBorrowingBook[i], userName);
			break;
		}
	}

	return EXIT_SUCCESS;
}

int addNewBorrowedBookToUserNode(userPosition userNode, char* bookTitle, int numOfBorrowedCopies) {
	int i = 0;

	for (; i < MAX_NO_BORROWED_BOOKS; i++) {

		if (userNode->borrowedBooks[i]->numOfBorrowedCopies == 0) {
			strcpy(userNode->borrowedBooks[i]->bookTitle, bookTitle);
			userNode->borrowedBooks[i]->numOfBorrowedCopies = numOfBorrowedCopies;
			break;
		}
	}

	return EXIT_SUCCESS;
}

int menu(bookPosition headBookList,userPosition headUserList) {
	char pick = '\0';
	int tempInt = 0;
	char tempString[MAX_NAME_SIZE] = { 0 };

	while (1) {
		printf("\nZa ispisati sve knjige abecedno zajedno s njihovim pojedinostima upisite '1', za ispisati sve korisnike abecedno zajedno s njihovim pojedinostima upisite '2', za ispisati sve knjige iz odredene godine i njihove dostupne kolicine upisite '3', za ispisati sve knjige odredenog autora i njihove dostupne kolicine upisite '4', za unos novog korisnika upisite '5', ukoliko korisnik zeli posuditi knjigu/e upisite '6', ukoliko korisnik zeli posuditi vratiti knjigu/e upisite '7', a za spremiti stanje u knjiznici i izaci iz programa upisite '0': \n ");
		scanf(" %c", &pick);

		switch (pick) {

		case '1':

			printBooksAlphabetically(headBookList->next);

			continue;

		case '2':
			printUsersAlphabetically(headUserList->next);

			continue;

		case '3':

			printf("\nUpisite zeljenu godinu: ");
			scanf("%d", &tempInt);
			printBooksOfParticularYear(headBookList->next, tempInt);

			continue;

		case '4':

			printf("\nUpisite zeljenog autora: ");
			scanf("%*c");
			fgets(tempString, MAX_NAME_SIZE, stdin);
			tempString[strlen(tempString) - 1] = '\0';

			printBooksOfParticularAuthor(headBookList->next, tempString);
		
			continue;

		case '5':

			insertNewUser(headUserList);
		
			continue;

		case '6':
			
			borrowBooks(headBookList, headUserList);

			continue;

		case '7':

			returnBook(headBookList, headUserList);

			continue;

		case '0':

			saveCurrentLibraryStatus(headBookList->next, headUserList->next, "book_database.txt", "user_database.txt");

			printf("\nStanje knjiznice spremljeno, kraj programa!\n");

			break;

		default:
			printf("\nKrivi unos!!\n");

			continue;

		}

		break;
	}

	return EXIT_SUCCESS;
}

int printBooksAlphabetically(bookPosition firstBook) {
	bookPosition current = firstBook;
	int i = 0, counter = 0;

	printf("\n--------------------------------------------KNJIGE-------------------------------------------\n");

	while (current != NULL) {
		counter = 0;

		printf("\n%s, %s, %d.", current->bookTitle, current->authorName, current->publicationYear);

		if(current->numOfAvailableCopies==1)
			printf("\n		Na stanju trenutno: %d primjerak", current->numOfAvailableCopies);
		else
			printf("\n		Na stanju trenutno: %d primjerka/primjeraka", current->numOfAvailableCopies);
		
		for (i = 0; i < MAX_NO_BOOK_COPIES; i++) {
			if (current->usersBorrowingBook[i] != NULL)
				counter++;
		}

		if(counter){
			i = 0;

			printf("\n		Korisnici koji trenutno posuduju primjerak ove knjige:");

			while (i < MAX_NO_BORROWED_BOOKS) {

				if (current->usersBorrowingBook[i] != NULL) {

					if (current->usersBorrowingBook[i + 1] == NULL)
						printf(" %s", current->usersBorrowingBook[i]);
					else
						printf(" %s |", current->usersBorrowingBook[i]);
				}

				i++;
			}
			printf("\n");
		}

		else
			printf("\n		Nitko trenutno ne posuduje primjerak ove knjige.\n");

		current = current->next;
	}

	printf("\n---------------------------------------------------------------------------------------------\n");

	return EXIT_SUCCESS;
}

int printUsersAlphabetically(userPosition firstUser) {
	userPosition current = firstUser;
	int i = 0;

	printf("\n--------------------------------------------KORISNICI-------------------------------------------\n");

	if (firstUser == NULL)
		printf("Nema upisanih korisnika knjiznice!\n");

	else {

		while (current != NULL) {

			printf("\n%s\n", current->userName);

			if (current->totalNumOfBorrowedBooks != 0) {

				printf("	%d knjiga/e posudena/e trenutno ->", current->totalNumOfBorrowedBooks);

				for (i = 0; i < MAX_NO_BORROWED_BOOKS; i++) {

					if (current->borrowedBooks[i]->numOfBorrowedCopies != 0){

						if (current->borrowedBooks[i]->numOfBorrowedCopies > 1) {
							if (current->borrowedBooks[i + 1]->numOfBorrowedCopies != 0)
								printf(" %s - %d primjerka/primjeraka,", current->borrowedBooks[i]->bookTitle, current->borrowedBooks[i]->numOfBorrowedCopies);
							else
								printf(" %s - %d primjerka/primjeraka", current->borrowedBooks[i]->bookTitle, current->borrowedBooks[i]->numOfBorrowedCopies);
						}

						else {
							if (current->borrowedBooks[i + 1]->numOfBorrowedCopies != 0)
								printf(" %s - %d primjerak,", current->borrowedBooks[i]->bookTitle, current->borrowedBooks[i]->numOfBorrowedCopies);
							else
								printf(" %s - %d primjerak", current->borrowedBooks[i]->bookTitle, current->borrowedBooks[i]->numOfBorrowedCopies);
						}
					}
				}

				printf("\n");
			}

			else
				printf("	Korisnik nema posudenu nijednu knjigu trenutno.\n");

			current = current->next;
		}


	}

	printf("\n---------------------------------------------------------------------------------------------\n");

	return 0;

}

int printBooksOfParticularYear(bookPosition firstBook, int wantedYear) {
	bookPosition current = firstBook;
	int counter = 0;

	while (current != NULL) {

		if (current->publicationYear == wantedYear) {

			if(current->numOfAvailableCopies==1)
				printf("\n%s - dostupan %d primjerak\n", current->bookTitle, current->numOfAvailableCopies);

			else
				printf("\n%s - dostupno %d primjerka/primjeraka\n", current->bookTitle, current->numOfAvailableCopies);

			counter++;
		}

		current = current->next;
	}

	if (counter == 0)
		printf("\nNazalost nemamo nijednu knjigu izdanu te godine.\n");

	return EXIT_SUCCESS;
}

int printBooksOfParticularAuthor(bookPosition firstBook, char* wantedAuthor) {
	bookPosition current = firstBook;
	int counter = 0;

	while (current != NULL) {

		if (strcmp(current->authorName,wantedAuthor)==0) {

			if (current->numOfAvailableCopies == 1)
				printf("\n%s - dostupan %d primjerak\n", current->bookTitle, current->numOfAvailableCopies);

			else
				printf("\n%s - dostupno %d primjerka/primjeraka\n", current->bookTitle, current->numOfAvailableCopies);

			counter++;
		}

		current = current->next;
	}

	if (counter == 0)
		printf("\nNazalost nemamo nijednu knjigu od tog autora.\n");

	return EXIT_SUCCESS;
}

int insertNewUser(userPosition headUserList) {
	userPosition newUserNode = NULL;
	char userName[MAX_LINE_SIZE] = { 0 };

	printf("\nUnesite ime novog korisnika: ");
	scanf("%*c");
	fgets(userName, MAX_NAME_SIZE, stdin);
	userName[strlen(userName) - 1] = '\0';

	newUserNode = userListSortInsert(headUserList, userName, 0);

	if (newUserNode != NULL)
		printf("\nUspjesno dodan korisnik %s!\n", userName);
	else
		printf("\Dodavanje korisnika %s nije uspjelo!\n", userName);

	return EXIT_SUCCESS;
}

int borrowBooks(bookPosition headBookList, userPosition headUserList) {
	char userName[MAX_LINE_SIZE] = { 0 }, buffer[MAX_LINE_SIZE] = { 0 }, bookTitle[MAX_NAME_SIZE] = { 0 },pick='\0';
	userPosition wantedUserNode = NULL;
	int bookSum = 0, permittedNumOfBooksLeft = 0, wantedNumOfCopies = 0, i = 0,counter=0;
	bookPosition wantedBookNode = NULL;
	positionBorrowedBook alreadyBorrowedBook = NULL;

	printf("\nUnesite ime korisnika koji zeli posuditi knjigu/e: ");
	scanf("%*c");
	fgets(userName, MAX_NAME_SIZE, stdin);
	userName[strlen(userName) - 1] = '\0';

	wantedUserNode = findUserNode(headUserList->next, userName);

	if (wantedUserNode == NULL) {
		printf("\nUneseni korisnik se ne nalazi u nasoj bazi podataka.\n");
		return EXIT_FAILURE;
	}

	while (1) {
		printf("\nUnesite naslov i broj primjeraka knjige koje korisnik zeli posuditi u formatu 'naslov,brPrimjeraka' :\n");
		fgets(buffer, MAX_LINE_SIZE, stdin);
		sscanf(buffer," %[^,]%*c%d", bookTitle, &wantedNumOfCopies);

		wantedBookNode = findBookNode(headBookList->next, bookTitle);

		if (wantedBookNode == NULL)
			printf("\nNazalost primjerke te knjige ne posjedujemo.\n");
	
		else {

			if (wantedNumOfCopies > wantedBookNode->numOfAvailableCopies) {
				printf("\nNazalost nemamo na stanju toliki broj primjeraka te knjige, ako zelite svejedno posuditi sve dostupne primjerke upisite 'y' ili ako zelite posuditi neku drugu knjigu upisite 'n'.\n");
				scanf(" %c", &pick);

				if (pick == 'y')
					wantedNumOfCopies = wantedBookNode->numOfAvailableCopies;

				else {
					scanf("%*c");
					continue;
				}
			}

			permittedNumOfBooksLeft = MAX_NO_BORROWED_BOOKS - wantedUserNode->totalNumOfBorrowedBooks;
			bookSum = permittedNumOfBooksLeft - wantedNumOfCopies;

			if (bookSum < 0) {

				if (permittedNumOfBooksLeft == 0)
					printf("\nNazalost ne mozete posuditi trazene knjige zato sto ste dosegli dozvoljeni broj posudenih knjiga (%d).\n",MAX_NO_BORROWED_BOOKS);
				else if (permittedNumOfBooksLeft == 1)
					printf("\nNazalost ne mozete posuditi trazene knjige zato sto Vam je dopusteno posuditi jos samo %d knjigu.\n", permittedNumOfBooksLeft);
				else
					printf("\nNazalost ne mozete posuditi trazene knjige zato sto Vam je dopusteno posuditi jos samo %d knjige/a.\n", permittedNumOfBooksLeft);
			}

			else {

				alreadyBorrowedBook = NULL;
				counter = 0;

				wantedUserNode->totalNumOfBorrowedBooks += wantedNumOfCopies;

				for (i = 0; i < MAX_NO_BORROWED_BOOKS; i++) {

					if (strcmp(wantedUserNode->borrowedBooks[i]->bookTitle, bookTitle) == 0) {
						alreadyBorrowedBook = wantedUserNode->borrowedBooks[i];
						break;
					}
				}

				if (alreadyBorrowedBook == NULL)
					addNewBorrowedBookToUserNode(wantedUserNode, bookTitle, wantedNumOfCopies);
				else
					alreadyBorrowedBook->numOfBorrowedCopies += wantedNumOfCopies;


				wantedBookNode->numOfAvailableCopies -= wantedNumOfCopies;

				for (i = 0; i < MAX_NO_BORROWED_BOOKS; i++) {

					if (wantedBookNode->usersBorrowingBook[i] != NULL && strcmp(wantedBookNode->usersBorrowingBook[i], userName) == 0) {
						counter++;
						break;
					}
				}

				if (!counter)
					addNewUserThatBorrowedBookToBookNode(wantedBookNode, userName);

				if (wantedNumOfCopies == 1)
					printf("\nUspjesno posuden %d primjerak knjige %s!\n", wantedNumOfCopies, bookTitle);
				else
					printf("\nUspjesno posudeno %d primjerka/primjeraka knjige %s!\n", wantedNumOfCopies, bookTitle);
			}
		}

		printf("\nUkoliko zelite posuditi neku drugu knjigu upisite 'y', a ako ne upisite 'n': ");
		scanf(" %c", &pick);

		if (pick == 'n')
			break;

		else
			scanf("%*c");
	}

	return EXIT_SUCCESS;
}

userPosition findUserNode(userPosition firstUser, char* userName) {
	userPosition current = firstUser;

	while (current != NULL && strcmp(userName, current->userName) != 0)
		current = current->next;

	return current;
}

int returnBook(bookPosition headBookList, userPosition headUserList) {
	char userName[MAX_NAME_SIZE] = { 0 }, buffer[MAX_LINE_SIZE] = { 0 }, bookTitle[MAX_NAME_SIZE] = { 0 },pick='\0';
	int numOfCopiesToReturn = 0, i = 0, counter = 0;
	userPosition wantedUserNode = NULL;
	bookPosition wantedBookNode = NULL;
	positionBorrowedBook borrowedBook = NULL;

	printf("\nUnesite ime korisnika koji zeli vratiti knjigu/e: ");
	scanf("%*c");
	fgets(userName, MAX_NAME_SIZE, stdin);
	userName[strlen(userName) - 1] = '\0';

	wantedUserNode = findUserNode(headUserList->next, userName);

	if (wantedUserNode == NULL) {
		printf("\nUneseni korisnik se ne nalazi u nasoj bazi podataka.\n");
		return EXIT_FAILURE;
	}

	while (1) {

		printf("\nUnesite naslov i broj primjeraka knjige koje korisnik zeli vratiti u formatu 'naslov,brPrimjeraka' :\n");
		fgets(buffer, MAX_LINE_SIZE, stdin);
		sscanf(buffer, " %[^,]%*c%d", bookTitle, &numOfCopiesToReturn);

		wantedBookNode = findBookNode(headBookList->next, bookTitle);

		wantedUserNode->totalNumOfBorrowedBooks -= numOfCopiesToReturn;

		counter = 0;

		for (i = 0; i < MAX_NO_BORROWED_BOOKS; i++) {

			if (strcmp(wantedUserNode->borrowedBooks[i]->bookTitle, bookTitle) == 0) {
				borrowedBook = wantedUserNode->borrowedBooks[i];
				borrowedBook->numOfBorrowedCopies -= numOfCopiesToReturn;
				wantedBookNode->numOfAvailableCopies += numOfCopiesToReturn;

				if (borrowedBook->numOfBorrowedCopies == 0) {
					strcpy(borrowedBook->bookTitle, " ");
					borrowedBook->numOfBorrowedCopies = 0;

					delUserThatBorrowedBookFromBookNode(wantedBookNode, userName);
				}

				counter++;
				break;
			}
		}


		if (counter) {

			if (numOfCopiesToReturn == 1)
				printf("\nUspjesno vracen %d primjerak knjige %s!\n", numOfCopiesToReturn, bookTitle);
			else
				printf("\nUspjesno vraceno %d primjerka/primjeraka knjige %s!\n", numOfCopiesToReturn, bookTitle);
		}

		else
			printf("\nKorisnik trenutno nema posuden primjerak trazene knjige!\n");

		printf("\nUkoliko zelite vratiti neku drugu knjigu upisite 'y', a ako ne upisite 'n': ");
		scanf(" %c", &pick);

		if (pick == 'n')
			break;

		else
			scanf("%*c");

	}

	return EXIT_SUCCESS;
}

int delUserThatBorrowedBookFromBookNode(bookPosition bookNode,char * userName) {
	int i = 0;

	while(i<MAX_NO_BOOK_COPIES){

		if (bookNode->usersBorrowingBook[i] != NULL && strcmp(bookNode->usersBorrowingBook[i], userName) == 0) {

			bookNode->usersBorrowingBook[i] = NULL;
			break;
		}
	
		i++;
	}

	return EXIT_SUCCESS;
}

int saveCurrentLibraryStatus(bookPosition firstBook, userPosition firstUser, char* booksFilename, char* usersFilename) {
	FILE* filenameBooks = NULL,*filenameUsers = NULL;
	bookPosition currentBook = firstBook;
	userPosition currentUser = firstUser;
	int i = 0;

	filenameBooks = fopen(booksFilename, "w");

	while (currentBook != NULL) {
		
		if(currentBook->next!=NULL)
			fprintf(filenameBooks, "%s,%s,%d %d\n", currentBook->bookTitle, currentBook->authorName, currentBook->publicationYear, currentBook->numOfAvailableCopies);

		else
			fprintf(filenameBooks, "%s,%s,%d %d", currentBook->bookTitle, currentBook->authorName, currentBook->publicationYear, currentBook->numOfAvailableCopies);

		currentBook = currentBook->next;
	}


	filenameUsers = fopen(usersFilename, "w");

	while (currentUser != NULL) {

		fprintf(filenameUsers, "%s,%d", currentUser->userName, currentUser->totalNumOfBorrowedBooks);

		if (currentUser->totalNumOfBorrowedBooks != 0) {

			for (i = 0; i < MAX_NO_BORROWED_BOOKS; i++) {

				if (currentUser->borrowedBooks[i]->numOfBorrowedCopies != 0)
					fprintf(filenameUsers," %s,%d", currentUser->borrowedBooks[i]->bookTitle, currentUser->borrowedBooks[i]->numOfBorrowedCopies);
			}
		}

		if (currentUser->next != NULL)
			fprintf(filenameUsers, "\n"); 
		
		currentUser = currentUser->next;
	}


	fclose(filenameBooks);
	fclose(filenameUsers);

	return EXIT_SUCCESS;
}