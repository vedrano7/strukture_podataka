/* 11. Prepraviti zadatak 10 na način da se formira hash tablica država.Tablica ima 11 mjesta, a
funkcija za preslikavanje ključ računa da se zbraja ASCII vrijednost prvih pet slova države zatim
računa ostatak cjelobrojnog dijeljenja te vrijednosti s veličinom tablice.Države s istim ključem se
pohranjuju u vezanu listu sortiranu po nazivu države.Svaki čvor vezane liste sadrži stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada. */

//ovo je zap hash tablica sa zasebnim redovima (separate chaining)

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define HASHTABSIZE 11
#define MAXLINESIZE (1024)
#define MAXNAMESIZE (50)

typedef struct CityTreeNode* positionCityTreeNode;
typedef struct CityTreeNode {

	char cityName[MAXNAMESIZE];
	int cityPopulation;

	positionCityTreeNode leftChild;
	positionCityTreeNode rightChild;
}cityTreeNode;

typedef struct CountryListNode* positionCountryListNode;
typedef struct CountryListNode {

	char countryName[MAXNAMESIZE];
	positionCityTreeNode rootCities;

	positionCountryListNode next;
}countryListNode;

typedef struct HashTable* hashTabPointer;
typedef struct HashTable {

	positionCountryListNode buckets[HASHTABSIZE];

}hashTable;


hashTabPointer createHashT();
int readCountriesFromFileAndFillHashTable(FILE* filename, hashTabPointer hashT);
int hashFunction(char* countryName);
positionCountryListNode insertCountryListNode(positionCountryListNode head, char* countryName);
int readFromCityFileAndMakeTree(positionCountryListNode countryNodePosition, char* cityFileName);
positionCityTreeNode cityTreeSortInsert(positionCityTreeNode root, int cityPop, char* cityName);
positionCityTreeNode createCityTreeNode(char* cityName, int cityPop);
int printCountriesAndCities(hashTabPointer hashT);
int inorderPrintCityTree(positionCityTreeNode root);
positionCountryListNode findCountryInHashT(hashTabPointer hashT, char* wantedCountryName);
positionCityTreeNode findGreaterPopCity(positionCityTreeNode root, int inputPopNum);
int printCities(positionCityTreeNode root, int inputPopNum);


int main() {
	hashTabPointer hashT = NULL;
	int inputPopNum = 0,key=0;
	char wantedCountryName[MAXNAMESIZE] = { '\0' };
	positionCountryListNode wantedCountry = NULL;
	positionCityTreeNode greaterPopCity = NULL;

	hashT = createHashT();

	readCountriesFromFileAndFillHashTable("drzave.txt", hashT);

	printCountriesAndCities(hashT);

	printf("\nUpisite odredenu drzavu i broj stanovnika i mi cemo Vam ispisati imena svih gradova te drzave ovdje upisano s vecim brojem stanovnika!\n");
	scanf("%s %d", wantedCountryName, &inputPopNum);

	wantedCountry = findCountryInHashT(hashT, wantedCountryName);

	greaterPopCity = findGreaterPopCity(wantedCountry->rootCities, inputPopNum);

	if (greaterPopCity == NULL)
		printf("\nNema gradova s vecim brojem stanovnika upisano u fajl '%s'!\n", wantedCountryName);

	else {

		printf("\nGradovi s vecim brojem gradana upisani u fajl '%s' su: ", wantedCountryName);
		printCities(greaterPopCity, inputPopNum);

		printf("\n");
	}

	free(greaterPopCity);
	free(hashT);
	free(wantedCountry);

	return EXIT_SUCCESS;
}

hashTabPointer createHashT() {
	hashTabPointer newHashT = NULL;
	int i = 0;

	newHashT = malloc(sizeof(hashTable));

	if (!newHashT) {
		printf("\nUnable to allocate memory for newHashT!\n");
		exit(EXIT_FAILURE);
	}

	for (i = 0; i < HASHTABSIZE; i++) {
		newHashT->buckets[i] = malloc(sizeof(countryListNode));
		newHashT->buckets[i]->next = NULL;
	}

	return newHashT;
}

int readCountriesFromFileAndFillHashTable(FILE* filename, hashTabPointer hashT) {
	FILE* countriesFile = NULL;
	char buffer[MAXLINESIZE] = { '\0' }, countryName[MAXNAMESIZE] = { '\0' }, cityFileName[MAXNAMESIZE] = { 0 };
	int key = 0;
	positionCountryListNode countryNodePosition = NULL;

	countriesFile = fopen(filename, "r");

	while (!feof(countriesFile)) {
		fgets(buffer, MAXLINESIZE, countriesFile);
		sscanf(buffer, "%s %s", countryName, cityFileName);

		key = hashFunction(countryName);

		countryNodePosition=insertCountryListNode(hashT->buckets[key],countryName);
		readFromCityFileAndMakeTree(countryNodePosition, cityFileName);
	}

	fclose(countriesFile);

	return EXIT_SUCCESS;
}

int hashFunction(char* countryName) {
	int ASCIIRes = 0, i = 0;

	for (; i < 5; i++) {

		if(countryName[i]!='\0')
			ASCIIRes += countryName[i];
	}

	return ASCIIRes % HASHTABSIZE;
}

positionCountryListNode insertCountryListNode(positionCountryListNode head,char* countryName){
	positionCountryListNode newNode = NULL, current = head;

	newNode = malloc(sizeof(countryListNode));

	if (!newNode) {
		printf("\nNeuspjela alokacija memorije za newNode\n");
		exit(-1);
	}

	strcpy(newNode->countryName, countryName);
	newNode->rootCities = NULL;

	while (current->next != NULL && strcmp(newNode->countryName, current->next->countryName) > 0)
		current = current->next;

	newNode->next = current->next;
	current->next = newNode;

	return newNode;
}

int readFromCityFileAndMakeTree(positionCountryListNode countryNodePosition, char* cityFileName) {
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
		sscanf(buffer, "%[^,]%*c%d", cityName, &cityPopulation);

		countryNodePosition->rootCities = cityTreeSortInsert(countryNodePosition->rootCities, cityPopulation, cityName);
	}

	fclose(cityFile);

	return EXIT_SUCCESS;
}

positionCityTreeNode cityTreeSortInsert(positionCityTreeNode root, int cityPop, char* cityName) {

	if (root == NULL)
		return createCityTreeNode(cityName, cityPop);

	else if (cityPop < root->cityPopulation)
		root->leftChild = cityTreeSortInsert(root->leftChild, cityPop, cityName);

	else if (cityPop > root->cityPopulation)
		root->rightChild = cityTreeSortInsert(root->rightChild, cityPop, cityName);

	else {

		if (strcmp(cityName, root->cityName) < 0)
			root->leftChild = cityTreeSortInsert(root->leftChild, cityPop, cityName);

		else if (strcmp(cityName, root->cityName) > 0)
			root->rightChild = cityTreeSortInsert(root->rightChild, cityPop, cityName);

	}

	return root;
}

positionCityTreeNode createCityTreeNode(char* cityName, int cityPop) {
	positionCityTreeNode newNode = NULL;

	newNode = malloc(sizeof(cityTreeNode));

	if (!newNode) {
		printf("\nUnable to allocate memory for newNode!\n");
		return NULL;
	}

	strcpy(newNode->cityName, cityName);
	newNode->cityPopulation = cityPop;

	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}

int printCountriesAndCities(hashTabPointer hashT) {
	int i = 0;
	positionCountryListNode current = NULL;

	for (i=0; i < HASHTABSIZE; i++) {

		if (hashT->buckets[i]->next != NULL) {
			current = hashT->buckets[i]->next;

			while (current != NULL) {

				printf("%s ->", current->countryName);
				inorderPrintCityTree(current->rootCities);
				printf("\n");
				current = current->next;
			}

		}
	}

	return EXIT_SUCCESS;
}

int inorderPrintCityTree(positionCityTreeNode root) {

	if (root != NULL) {
		inorderPrintCityTree(root->leftChild);
		printf("   %s	", root->cityName);
		inorderPrintCityTree(root->rightChild);
	}

	return EXIT_SUCCESS;
}

positionCountryListNode findCountryInHashT(hashTabPointer hashT, char* wantedCountryName) {
	int key = 0;
	positionCountryListNode current = NULL;


	key = hashFunction(wantedCountryName);

	current = hashT->buckets[key]->next;

	while (current != NULL && strcmp(current->countryName, wantedCountryName) != 0) {

		current = current->next;
	}

	return current;

}

positionCityTreeNode findGreaterPopCity(positionCityTreeNode root, int inputPopNum) {

	if (root == NULL)
		return NULL;

	else if (inputPopNum >= root->cityPopulation)
		return findGreaterPopCity(root->rightChild, inputPopNum);

	return root;
}

int printCities(positionCityTreeNode root, int inputPopNum) {

	if (root == NULL)
		return NULL;

	else if (inputPopNum >= root->cityPopulation)
		printCities(root->rightChild, inputPopNum);

	else if (inputPopNum < root->cityPopulation) {
		printf("  %s   ", root->cityName);
		printCities(root->leftChild, inputPopNum);
		printCities(root->rightChild, inputPopNum);
	}

	return EXIT_SUCCESS;
}