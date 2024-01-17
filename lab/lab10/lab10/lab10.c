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
#define MAXNAMESIZE (50)

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

	positionTreeNodeA rootCitiesSortedPopulation;
	positionTreeNodeA rootCitiesSortedName;

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

	positionListNodeB headCitiesSortedPopulation;
	positionListNodeB headCitiesSortedName;

	positionTreeNodeB leftChild;
	positionTreeNodeB rightChild;
}treeNodeB;


//a)
int problemA();
int makeCountryListAndCityTrees(char* filename,positionListNodeA headCountryList);
positionListNodeA countryListSortInsert(positionListNodeA head, char* countryName);
int printCountryListAndCityNameTree(positionListNodeA firstEl);
int readFromCityFileAndMakeTrees(positionListNodeA countryNodePosition,char* cityFileName);
positionTreeNodeA cityTreeSortInsert(positionTreeNodeA root, int cityPop, char* cityName, int mode);
positionTreeNodeA createTreeNodeA(char* cityName, int cityPop);
int inorderPrintTreeName(positionTreeNodeA root);
int printCitiesWithGreaterPopulationA(positionListNodeA headCountryList,int inputPopNum);
positionTreeNodeA findGreaterPopCityA(positionTreeNodeA rootCitiesSortedPopulation, int inputPopNum);
int printCitiesA(positionTreeNodeA root, int inputPopNum);


//b)
int problemB();
int makeCountryTreeAndCityLists(char* filename, positionTreeNodeB rootCountryTree);
positionTreeNodeB countryTreeSortInsert(positionTreeNodeB root, char* countryName);
positionTreeNodeB createTreeNodeB(char* countryName);
positionListNodeB createListNode(int cityPop, char* cityName);
int readFromCityFileAndMakeLists(positionTreeNodeB countryNodePosition, char* cityFileName);
positionListNodeB cityListSortInsert(positionListNodeB head, int cityPop, char* cityName, int mode);
int inorderPrintCountryTreeAndCityNameList(positionTreeNodeB root);
int printCityNameList(positionListNodeB firstEl);
int printCitiesWithGreaterPopulationB(positionTreeNodeB rootCountry, int inputPopNum);
positionListNodeB findGreaterPopCityB(positionListNodeB firstEl, int inputPopNum);
int printCitiesB(positionTreeNodeB countryNode, positionListNodeB firstEl);


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
	listNodeA headCountryList = { {0}, NULL,NULL, NULL };
	int inputPopNum = 0;

	makeCountryListAndCityTrees("drzave.txt", &headCountryList);
	
	printCountryListAndCityNameTree(headCountryList.next);

	printf("\nUpisite odredeni broj stanovnika i mi cemo Vam ispisati imena svih gradova ovdje upisano s vecim!\n");
	scanf("%d", &inputPopNum);

	printCitiesWithGreaterPopulationA(headCountryList.next, inputPopNum);

	return EXIT_SUCCESS;
}

int makeCountryListAndCityTrees(char* filename, positionListNodeA headCountryList){
	FILE* countriesFile = NULL;
	char buffer[MAXNAMESIZE] = { 0 }, countryName[MAXNAMESIZE] = { 0 }, cityFileName[MAXNAMESIZE] = { 0 };
	countriesFile = fopen(filename, "r");
	positionListNodeA countryNodePosition = NULL;

	if (!countriesFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}

	while (!feof(countriesFile)) {
		fgets(buffer, MAXLINESIZE, countriesFile);
		sscanf(buffer, "%s %s", countryName, cityFileName);

		countryNodePosition=countryListSortInsert(headCountryList, countryName);
		readFromCityFileAndMakeTrees(countryNodePosition,cityFileName);
	}

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
	newNode->rootCitiesSortedName = NULL;
	newNode->rootCitiesSortedPopulation = NULL;

	while (current->next != NULL && strcmp(newNode->countryName, current->next->countryName)>0 )
		current = current->next;

	newNode->next = current->next;
	current->next = newNode;

	return newNode;
}

int readFromCityFileAndMakeTrees(positionListNodeA countryNodePosition,char* cityFileName) {
	FILE* cityFile = NULL;
	char buffer[MAXNAMESIZE] = { 0 }, cityName[MAXNAMESIZE] = { 0 };
	int cityPopulation = 0;

	cityFile = fopen(cityFileName, "r");

	if (!cityFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}

	
	while (!feof(cityFile)) {
		fgets(buffer, MAXLINESIZE, cityFile);
		sscanf(buffer, "%[^,]%*c%d", cityName, &cityPopulation);	//za ime grada uzima sve do zareza onda uzima i odma odbacuje zarez i zadnje uzima broj za populaciju grada

		countryNodePosition->rootCitiesSortedName = cityTreeSortInsert(countryNodePosition->rootCitiesSortedName, cityPopulation, cityName, 1);
		countryNodePosition->rootCitiesSortedPopulation = cityTreeSortInsert(countryNodePosition->rootCitiesSortedPopulation, cityPopulation, cityName, 2);
	}
	

	return EXIT_SUCCESS;
}

positionTreeNodeA cityTreeSortInsert(positionTreeNodeA root,int cityPop,char* cityName, int mode) {

	if (mode == 1) {

		if (root == NULL)
			return createTreeNodeA(cityName, cityPop);

		else if (strcmp(cityName, root->cityName) < 0)
			root->leftChild = cityTreeSortInsert(root->leftChild, cityPop, cityName, 1);

		else if(strcmp(cityName,root->cityName)>0)
			root->rightChild = cityTreeSortInsert(root->rightChild, cityPop, cityName,1);

	}
	
	
	else {
		if (root == NULL)
			return createTreeNodeA(cityName, cityPop);

		else if (cityPop < root->cityPopulation)
			root->leftChild = cityTreeSortInsert(root->leftChild, cityPop, cityName, 2);

		else if (cityPop >= root->cityPopulation)
			root->rightChild = cityTreeSortInsert(root->rightChild, cityPop, cityName, 2);

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

int printCountryListAndCityNameTree(positionListNodeA firstEl) {
	positionListNodeA current = firstEl;

	printf("\n---------\n");

	if (current == NULL)
		printf("Lista je prazna!\n");

	else {
		while (current != NULL) {

			printf("%s ->", current->countryName);
			inorderPrintTreeName(current->rootCitiesSortedName);
			printf("\n");
			current = current->next;
		}
	}

	printf("---------\n");

	return 0;
}

int inorderPrintTreeName(positionTreeNodeA root) {

	if (root != NULL) {
		inorderPrintTreeName(root->leftChild);
		printf("   %s	", root->cityName);
		inorderPrintTreeName(root->rightChild);
	}

	return EXIT_SUCCESS;
}

int printCitiesWithGreaterPopulationA(positionListNodeA firstCountryInList,int inputPopNum) {
	positionTreeNodeA greaterPopCity = NULL;
	positionListNodeA current = firstCountryInList;

	while (current != NULL) {
		greaterPopCity = NULL;

		greaterPopCity = findGreaterPopCityA(current->rootCitiesSortedPopulation, inputPopNum);

		if (greaterPopCity == NULL)
			printf("\nNema gradova s vecim brojem stanovnika upisano u fajl '%s'!\n", current->countryName);

		else {

			printf("\nGradovi s vecim brojem gradana upisani u fajl '%s' su: ", current->countryName);
			printCitiesA(greaterPopCity, inputPopNum);

			printf("\n");
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
	treeNodeB rootCountryTree = { {0}, NULL,NULL, NULL,NULL };
	int inputPopNum = 0;

	makeCountryTreeAndCityLists("drzave.txt", &rootCountryTree);

	inorderPrintCountryTreeAndCityNameList(&rootCountryTree);

	printf("\nUpisite odredeni broj stanovnika i mi cemo Vam ispisati imena svih gradova ovdje upisano s vecim!\n");
	scanf("%d", &inputPopNum);

	printCitiesWithGreaterPopulationB(&rootCountryTree, inputPopNum);

	return EXIT_SUCCESS;
}

int makeCountryTreeAndCityLists(char* filename, positionTreeNodeB rootCountryTree) {
	FILE* countriesFile = NULL;
	char buffer[MAXNAMESIZE] = { 0 }, countryName[MAXNAMESIZE] = { 0 }, cityFileName[MAXNAMESIZE] = { 0 };
	countriesFile = fopen(filename, "r");
	positionTreeNodeB countryNodePosition = NULL;

	if (!countriesFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}

	while (!feof(countriesFile)) {
		fgets(buffer, MAXLINESIZE, countriesFile);
		sscanf(buffer, "%s %s", countryName, cityFileName);

		countryNodePosition = countryTreeSortInsert(rootCountryTree, countryName);
		readFromCityFileAndMakeLists(countryNodePosition, cityFileName);
	}

	return EXIT_SUCCESS;
}

positionTreeNodeB countryTreeSortInsert(positionTreeNodeB root,char* countryName) {

	if (root == NULL)
		return createTreeNodeB(countryName);

	else if (strcmp(countryName, root->countryName) < 0)
		root->leftChild = countryTreeSortInsert(root->leftChild, countryName);

	else if (strcmp(countryName, root->countryName) > 0)
		root->rightChild = countryTreeSortInsert(root->rightChild,countryName);

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

	newNode->headCitiesSortedName = createListNode(0, NULL);
	newNode->headCitiesSortedPopulation = createListNode(0, NULL);

	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}

positionListNodeB createListNode(int cityPop, char* cityName) {
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

int readFromCityFileAndMakeLists(positionTreeNodeB countryNodePosition, char* cityFileName) {
	FILE* cityFile = NULL;
	char buffer[MAXNAMESIZE] = { 0 }, cityName[MAXNAMESIZE] = { 0 };
	int cityPopulation = 0;

	cityFile = fopen(cityFileName, "r");

	if (!cityFile) {
		printf("\nUnable to open file countriesFile!\n");
		return EXIT_FAILURE;
	}


	while (!feof(cityFile)) {
		fgets(buffer, MAXLINESIZE, cityFile);
		sscanf(buffer, "%[^,]%*c%d", cityName, &cityPopulation);	//za ime grada uzima sve do zareza onda uzima i odma odbacuje zarez i zadnje uzima broj za populaciju grada

		countryNodePosition->headCitiesSortedName = cityListSortInsert(countryNodePosition->headCitiesSortedName, cityPopulation, cityName, 1);
		countryNodePosition->headCitiesSortedPopulation = cityListSortInsert(countryNodePosition->headCitiesSortedName, cityPopulation, cityName, 2);
	}


	return EXIT_SUCCESS;
}

positionListNodeB cityListSortInsert(positionListNodeB head, int cityPop, char* cityName, int mode) {
	positionListNodeB newNode = NULL, current = head;

	newNode = createListNode(cityPop, cityName);

	if (mode == 1) {

		while (current->next != NULL && strcmp(newNode->cityName, current->next->cityName) > 0)
			current = current->next;

		newNode->next = current->next;
		current->next = newNode;
	}
	
	else {

		while (current->next != NULL && newNode->cityPopulation>current->next->cityPopulation)
			current = current->next;

		newNode->next = current->next;
		current->next = newNode;
	}

	return newNode;
}

int inorderPrintCountryTreeAndCityNameList(positionTreeNodeB root) {

	printf("\n---------\n");

	if (root != NULL) {
		inorderPrintCountryTreeAndCityNameList(root->leftChild);
		printf("%s ->", root->countryName);
		printCityNameList(root->headCitiesSortedName->next);
		printf("\n");
		inorderPrintCountryTreeAndCityNameList(root->rightChild);
	}

	printf("---------\n");

	return 0;
}

int printCityNameList(positionListNodeB firstEl) {
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

int printCitiesWithGreaterPopulationB(positionTreeNodeB rootCountry, int inputPopNum) {
	positionListNodeB greaterPopCity = NULL;

	if (rootCountry != NULL) {
		printCitiesWithGreaterPopulationB(rootCountry->leftChild, inputPopNum);
		greaterPopCity=findGreaterPopCityB(rootCountry->headCitiesSortedPopulation->next, inputPopNum);
		printCitiesB(rootCountry->headCitiesSortedPopulation->next,greaterPopCity);
		printCitiesWithGreaterPopulationB(rootCountry->rightChild, inputPopNum);
	}

	return EXIT_SUCCESS;
}

positionListNodeB findGreaterPopCityB(positionListNodeB firstEl, int inputPopNum) {
	positionListNodeB current = firstEl;

	while (current != NULL && current->cityPopulation <= inputPopNum) {

		current = current->next;
	}

	return current;

}

int printCitiesB(positionTreeNodeB countryNode, positionListNodeB firstEl) {
	positionListNodeB current = firstEl;

	if (current == NULL)
		printf("\nNema gradova s vecim brojem stanovnika upisano u fajl '%s'!\n", countryNode->countryName);

	else {

		while (current != NULL) {
			printf("\nGradovi s vecim brojem gradana upisani u fajl '%s' su: ", countryNode->countryName);
			printf("   %s	", current->cityName);

			current = current->next;
		}

		printf("\n");
	}

	return 0;
}
