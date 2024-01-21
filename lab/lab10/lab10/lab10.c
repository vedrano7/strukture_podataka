/*10. Napisati program koji čita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih država.Uz
ime države u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
države.Svaka datoteka koja predstavlja državu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.

a) Potrebno je formirati sortiranu vezanu listu država po nazivu.Svaki čvor vezane liste
sadrži stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.

b) Potrebno je formirati stablo država sortirano po nazivu.Svaki čvor stabla sadrži vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.

Nakon formiranja podataka potrebno je ispisati države i gradove te omogućiti korisniku putem
tastature pretragu gradova određene države koji imaju broj stanovnika veći od unosa na
tastaturi.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include <string.h>
#define MAXLINESIZE (100)
#define MAXNAMESIZE (1024)

//a)


typedef struct TreeNodeA* positionTreeNodeA;
typedef struct TreeNodeA {
	char cityName[MAXNAMESIZE];
	int cityPopulation;
	positionTreeNodeA leftChild;
	positionTreeNodeA rightChild;

}treeNodeA;

typedef struct ListNodeA* positionListNodeA;
typedef struct ListNodeA {
	char countryName[MAXNAMESIZE];

	positionTreeNodeA rootCities;

	positionListNodeA next;
}listNodeA;


//b)


typedef struct ListNodeB* positionListNodeB;
typedef struct ListNodeB {
	char cityName[MAXNAMESIZE];
	int cityPopulation;
	positionListNodeB next;
}listNodeB;

typedef struct TreeNodeB* positionTreeNodeB;
typedef struct TreeNodeB {
	char countryName[MAXNAMESIZE];

	positionListNodeB headCities;

	positionTreeNodeB leftChild;
	positionTreeNodeB rightChild;
}treeNodeB;


//a)

int problemA();
int makeCountryListAndCityTree(char* filename,positionListNodeA headCountryList);
positionListNodeA countryListSortInsert(positionListNodeA head, char* countryName);
int printCountryListAndCityTree(positionListNodeA firstEl);
int readFromCityFileAndMakeTree(positionListNodeA countryNodePosition,char* cityFileName);
positionTreeNodeA cityTreeSortInsert(positionTreeNodeA root, int cityPop, char* cityName);
positionTreeNodeA createTreeNodeA(char* cityName, int cityPop);
int inorderPrintTree(positionTreeNodeA root);
int printCitiesWithGreaterPopulationA(positionListNodeA headCountryList,char* wantedCountryName,int inputPopNum);
positionTreeNodeA findGreaterPopCityA(positionTreeNodeA rootCitiesSortedPopulation, int inputPopNum);
int printCitiesA(positionTreeNodeA root, int inputPopNum);


//b)

int problemB();
positionTreeNodeB makeCountryTreeAndCityList(char* filename, positionTreeNodeB rootCountryTree);
positionTreeNodeB countryTreeSortInsert(positionTreeNodeB root, char* countryName);
positionTreeNodeB createTreeNodeB(char* countryName);
positionListNodeB createCityListNode(int cityPop, char* cityName);
int readFromCityFileAndMakeLists(positionTreeNodeB rootCountryTree, char* countryName, char* cityFileName);
positionTreeNodeB findCountryNode(positionTreeNodeB root, char* countryName);
int cityListSortInsert(positionListNodeB head, int cityPop, char* cityName);
positionListNodeB createCityListNode(int cityPop, char* cityName);
int inorderPrintCountryTreeAndCityLists(positionTreeNodeB root);
int printCityList(positionListNodeB firstEl);
int printCitiesWithGreaterPopulationB(positionTreeNodeB rootCountry,char* wantedCountryName, int inputPopNum);
positionListNodeB findGreaterPopCityB(positionListNodeB firstEl, int inputPopNum);
int printCitiesB(char* countryName, positionListNodeB firstGreaterPopCity, positionListNodeB firstEl);

//main
int main() {
	char pick = '\0';

	printf("\nUkoliko zelite zadatak pod a) upisite 'a', a ukoliko zelite zadatak pod b) upisite 'b'\n");
	scanf("%c", &pick);

	if (pick == 'a')
		problemA();

	else
		problemB();

	return EXIT_SUCCESS;
}

//a)
int problemA() {
	listNodeA headCountryList = { {0}, NULL,NULL };
	int inputPopNum = 0;
	char wantedCountryName[MAXNAMESIZE] = { '\0' };

	makeCountryListAndCityTree("drzave.txt", &headCountryList);
	
	printCountryListAndCityTree(headCountryList.next);

	printf("\nUpisite odredenu drzavu i broj stanovnika i mi cemo Vam ispisati imena svih gradova te drzave ovdje upisano s vecim brojem stanovnika!\n");
	scanf("%s %d", wantedCountryName, &inputPopNum);

	printCitiesWithGreaterPopulationA(headCountryList.next, wantedCountryName, inputPopNum);

	return EXIT_SUCCESS;
}

int makeCountryListAndCityTree(char* filename, positionListNodeA headCountryList){
	FILE* countriesFile = NULL;
	char buffer[MAXLINESIZE] = { 0 }, countryName[MAXNAMESIZE] = { 0 }, cityFileName[MAXNAMESIZE] = { 0 };
	positionListNodeA countryNodePosition = NULL; 
	
	countriesFile = fopen(filename, "r");
	
	if (!countriesFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}

	while (!feof(countriesFile)) {
		fgets(buffer, MAXLINESIZE, countriesFile);
		sscanf(buffer, "%s %s", countryName, cityFileName);

		countryNodePosition=countryListSortInsert(headCountryList, countryName);
		readFromCityFileAndMakeTree(countryNodePosition,cityFileName);
	}

	fclose(countriesFile);

	return EXIT_SUCCESS;
}

positionListNodeA countryListSortInsert(positionListNodeA head, char* countryName) {
	positionListNodeA newNode = NULL, current = head;

	newNode = malloc(sizeof(listNodeA));

	if (!newNode) {
		printf("\nNeuspjela alokacija memorije za newNode\n");
		exit(-1);
	}

	strcpy(newNode->countryName, countryName);
	newNode->rootCities = NULL;

	while (current->next != NULL && strcmp(newNode->countryName, current->next->countryName)>0 )
		current = current->next;

	newNode->next = current->next;
	current->next = newNode;

	return newNode;
}

int readFromCityFileAndMakeTree(positionListNodeA countryNodePosition,char* cityFileName) {
	FILE* cityFile = NULL;
	char buffer[MAXNAMESIZE] = { '\0' }, cityName[MAXNAMESIZE] = { '\0' };
	int cityPopulation = 0;

	cityFile = fopen(cityFileName, "r");

	if (!cityFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}

	
	while (!feof(cityFile)) {
		fgets(buffer, MAXLINESIZE, cityFile);
		sscanf(buffer, "%[^,]%*c%d", cityName, &cityPopulation);	//za ime grada uzima sve do zareza onda uzima i odma odbacuje zarez i zadnje uzima broj za populaciju grada

		countryNodePosition->rootCities = cityTreeSortInsert(countryNodePosition->rootCities, cityPopulation, cityName);
	}
	
	fclose(cityFile);

	return EXIT_SUCCESS;
}

positionTreeNodeA cityTreeSortInsert(positionTreeNodeA root,int cityPop,char* cityName) {

	if (root == NULL)
		return createTreeNodeA(cityName, cityPop);

	else if (cityPop < root->cityPopulation)
		root->leftChild = cityTreeSortInsert(root->leftChild, cityPop, cityName);

	else if (cityPop > root->cityPopulation)
		root->rightChild = cityTreeSortInsert(root->rightChild, cityPop, cityName);

	else {		//u slucaju da u fajlu imamo dva grada istih populacija onda ih sortiramo po imenu (to je zadano u tekstu zad: "sortirano po broju stanovnika, zatim po nazivu grada")

		if (strcmp(cityName, root->cityName) < 0)
		root->leftChild = cityTreeSortInsert(root->leftChild, cityPop, cityName);

		else if (strcmp(cityName, root->cityName) > 0)
		root->rightChild = cityTreeSortInsert(root->rightChild, cityPop, cityName);

	}

	return root;
}

positionTreeNodeA createTreeNodeA(char* cityName, int cityPop) {
	positionTreeNodeA newNode = NULL;

	newNode = malloc(sizeof(treeNodeA));

	if (!newNode) {
		printf("\nUnable to allocate memory for newNode!\n");
		return NULL;
	}

	strcpy(newNode->cityName,cityName);
	newNode->cityPopulation = cityPop;

	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}

int printCountryListAndCityTree(positionListNodeA firstEl) {
	positionListNodeA current = firstEl;

	printf("\n---------\n");

	if (current == NULL)
		printf("Lista je prazna!\n");

	else {
		while (current != NULL) {

			printf("%s ->", current->countryName);
			inorderPrintTree(current->rootCities);
			printf("\n");
			current = current->next;
		}
	}

	printf("---------\n");

	return 0;
}

int inorderPrintTree(positionTreeNodeA root) {

	if (root != NULL) {
		inorderPrintTree(root->leftChild);
		printf("   %s	", root->cityName);
		inorderPrintTree(root->rightChild);
	}

	return EXIT_SUCCESS;
}

int printCitiesWithGreaterPopulationA(positionListNodeA firstCountryInList,char* wantedCountryName,int inputPopNum) {
	positionTreeNodeA greaterPopCity = NULL;
	positionListNodeA current = firstCountryInList;

	while (current != NULL) {

		if (strcmp(current->countryName, wantedCountryName)==0) {
			greaterPopCity = NULL;

			greaterPopCity = findGreaterPopCityA(current->rootCities, inputPopNum);

			if (greaterPopCity == NULL)
				printf("\nNema gradova s vecim brojem stanovnika upisano u fajl '%s'!\n", current->countryName);

			else {

				printf("\nGradovi s vecim brojem gradana upisani u fajl '%s' su: ", current->countryName);
				printCitiesA(greaterPopCity, inputPopNum);

				printf("\n");
			}

		}

		current = current->next;
	}

	return EXIT_SUCCESS;
}

positionTreeNodeA findGreaterPopCityA(positionTreeNodeA root, int inputPopNum) {

	if (root == NULL)
		return NULL;

	else if (inputPopNum >= root->cityPopulation)
		return findGreaterPopCityA(root->rightChild, inputPopNum);

	return root;
}

int printCitiesA(positionTreeNodeA root, int inputPopNum) {

	if (root == NULL)
		return NULL;

	else if (inputPopNum >= root->cityPopulation)
		printCitiesA(root->rightChild, inputPopNum);

	else if (inputPopNum < root->cityPopulation) {
		printf("  %s   ", root->cityName);
		printCitiesA(root->leftChild, inputPopNum);
		printCitiesA(root->rightChild, inputPopNum);
	}

	return EXIT_SUCCESS;
}

//b)

int problemB() {
	positionTreeNodeB rootCountryTree = NULL;
	int inputPopNum = 0;
	char wantedCountryName[MAXNAMESIZE] = { '\0' };

	rootCountryTree = makeCountryTreeAndCityList("drzave.txt", rootCountryTree);

	printf("\n---------\n");

	inorderPrintCountryTreeAndCityLists(rootCountryTree);

	printf("---------\n");

	printf("\nUpisite odredenu drzavu i broj stanovnika i mi cemo Vam ispisati imena svih gradova te drzave ovdje upisano s vecim brojem stanovnika!\n");
	scanf("%s %d", wantedCountryName, &inputPopNum);

	printCitiesWithGreaterPopulationB(rootCountryTree, wantedCountryName, inputPopNum);

	free(rootCountryTree);

	return EXIT_SUCCESS;
}

positionTreeNodeB makeCountryTreeAndCityList(char* filename, positionTreeNodeB rootCountryTree) {
	FILE* countriesFile = NULL;
	char buffer[MAXLINESIZE] = { 0 }, countryName[MAXNAMESIZE] = { 0 }, cityFileName[MAXNAMESIZE] = { 0 };


	countriesFile = fopen(filename, "r");

	if (!countriesFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}

	while (!feof(countriesFile)) {
		fgets(buffer, MAXLINESIZE, countriesFile);
		sscanf(buffer, "%s %s", countryName, cityFileName);

		rootCountryTree = countryTreeSortInsert(rootCountryTree, countryName);
		readFromCityFileAndMakeLists(rootCountryTree, countryName, cityFileName);
	}

	fclose(countriesFile);

	return rootCountryTree;
}

positionTreeNodeB countryTreeSortInsert(positionTreeNodeB root, char* countryName) {

	if (root == NULL)
		return createTreeNodeB(countryName);

	else if (strcmp(countryName, root->countryName) < 0)
		root->leftChild = countryTreeSortInsert(root->leftChild, countryName);

	else if (strcmp(countryName, root->countryName) > 0)
		root->rightChild = countryTreeSortInsert(root->rightChild, countryName);

	return root;
}

positionTreeNodeB createTreeNodeB(char* countryName) {
	positionTreeNodeB newNode = NULL;

	newNode = malloc(sizeof(treeNodeB));

	if (!newNode) {
		printf("\nUnable to allocate memory for newNode!\n");
		return NULL;
	}

	strcpy(newNode->countryName, countryName);

	newNode->headCities = malloc(sizeof(listNodeB));
	newNode->headCities->next = NULL;

	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}

int readFromCityFileAndMakeLists(positionTreeNodeB rootCountryTree, char* countryName, char* cityFileName) {
	FILE* cityFile = NULL;
	char buffer[MAXNAMESIZE] = { '\0' }, cityName[MAXNAMESIZE] = { '\0' };
	int cityPopulation = 0;
	positionTreeNodeB countryNode = NULL;

	cityFile = fopen(cityFileName, "r");

	if (!cityFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}


	while (!feof(cityFile)) {
		fgets(buffer, MAXLINESIZE, cityFile);
		sscanf(buffer, "%[^,]%*c%d", cityName, &cityPopulation);	//za ime grada uzima sve do zareza onda uzima i odma odbacuje zarez i zadnje uzima broj za populaciju grada


		countryNode = findCountryNode(rootCountryTree, countryName);
		cityListSortInsert(countryNode->headCities, cityPopulation, cityName);

	}

	fclose(cityFile);

	return EXIT_SUCCESS;
}

positionTreeNodeB findCountryNode(positionTreeNodeB root, char* countryName) {
	if (root == NULL)
		return NULL;

	else if (strcmp(countryName, root->countryName) < 0)
		return findCountryNode(root->leftChild, countryName);

	else if (strcmp(countryName, root->countryName) > 0)
		return findCountryNode(root->rightChild, countryName);

	return root;
}

int cityListSortInsert(positionListNodeB head, int cityPop, char* cityName) {
	positionListNodeB newNode = NULL, current = head;

	newNode = createCityListNode(cityPop, cityName);

	while (current->next != NULL && newNode->cityPopulation > current->next->cityPopulation)
		current = current->next;

	if (current->next == NULL || newNode->cityPopulation != current->next->cityPopulation) {

		newNode->next = current->next;
		current->next = newNode;
	}

	else {

		while (current->next != NULL && strcmp(newNode->cityName, current->next->cityName) > 0)
			current = current->next;

		newNode->next = current->next;
		current->next = newNode;
	}

	return EXIT_SUCCESS;
}

positionListNodeB createCityListNode(int cityPop, char* cityName) {
	positionListNodeB newNode = NULL;

	newNode = malloc(sizeof(listNodeB));

	if (!newNode) {
		printf("\nNeuspjela alokacija memorije za newNode\n");
		exit(-1);
	}

	strcpy(newNode->cityName, cityName);
	newNode->cityPopulation = cityPop;
	newNode->next = NULL;

	return newNode;
}

int inorderPrintCountryTreeAndCityLists(positionTreeNodeB root) {

	if (root != NULL) {
		inorderPrintCountryTreeAndCityLists(root->leftChild);
		printf("%s ->", root->countryName);
		printCityList(root->headCities->next);
		printf("\n");
		inorderPrintCountryTreeAndCityLists(root->rightChild);
	}

	return 0;
}

int printCityList(positionListNodeB firstEl) {
	positionListNodeB current = firstEl;

	if (current == NULL)
		printf("Nema upisanih gradova!\n");

	else {
		while (current != NULL) {

			printf("   %s   ", current->cityName);
			current = current->next;
		}
	}

	return 0;
}

int printCitiesWithGreaterPopulationB(positionTreeNodeB rootCountry,char* wantedCountryName, int inputPopNum) {
	positionListNodeB greaterPopCity = NULL;

	if (rootCountry != NULL) {

		if (strcmp(rootCountry->countryName, wantedCountryName) == 0) {
			greaterPopCity = findGreaterPopCityB(rootCountry->headCities->next, inputPopNum);
			printCitiesB(wantedCountryName, greaterPopCity, rootCountry->headCities->next);
		}

		else if(strcmp(wantedCountryName,rootCountry->countryName)<0)
			printCitiesWithGreaterPopulationB(rootCountry->leftChild, wantedCountryName, inputPopNum);

		else
			printCitiesWithGreaterPopulationB(rootCountry->rightChild, wantedCountryName, inputPopNum);

	}

	return EXIT_SUCCESS;
}

positionListNodeB findGreaterPopCityB(positionListNodeB firstEl, int inputPopNum) {
	positionListNodeB current = firstEl;

	while (current != NULL && inputPopNum >= current->cityPopulation)
		current = current->next;

	return current;
}

int printCitiesB(char* countryName, positionListNodeB firstGreaterPopCity, positionListNodeB firstEl) {
	positionListNodeB current = firstGreaterPopCity;

	if (current == NULL)
		printf("\nNema gradova s vecim brojem stanovnika upisano u fajl '%s'!\n", countryName);

	else {

		printf("\nGradovi s vecim brojem gradana upisani u fajl '%s' su: ", countryName);

		while (current != NULL) {

			printf("   %s	", current->cityName);

			current = current->next;
		}

		printf("\n");
	}

	return 0;
}