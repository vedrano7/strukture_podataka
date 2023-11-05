/*4. Napisati program za zbrajanje i množenje polinoma.Koeficijenti i eksponenti se
čitaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nužno sortirani.*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)
#define MAX_LINE (1024)

typedef struct element* position;
typedef struct element {
	int coeff;
	int exp;
	position next;
}Element;

int readPolyFromFile(position headPoly1, position headPoly2);
int parseStringIntoList(position headPoly, char buffer[MAX_LINE]);
position createElement(int coeff, int exp);
int sortedInput(position headPoly, position newElement);
int mergeElements(position prevElementInList, position newElement);
int printPoly(char* polyName,position firstEl);
int polySum(position firstElPoly1, position firstElPoly2, position headPolySum);
int polyProduct(position firstElPoly1, position firstElPoly2, position headPolyProduct);

int main() {
	Element headPoly1 = { 0,0,NULL }, headPoly2 = { 0,0,NULL }, headPolySum = {0,0,NULL}, headPolyProduct = { 0,0,NULL };

	if (readPolyFromFile(&headPoly1, &headPoly2) == EXIT_SUCCESS) {

		printPoly("Prvi polinom: " ,headPoly1.next);
		printPoly("Drugi polinom: ",headPoly2.next);
	}

	polySum(headPoly1.next, headPoly2.next, &headPolySum);

	printPoly("Suma polinoma: ", headPolySum.next);

	polyProduct(headPoly1.next, headPoly2.next, &headPolyProduct);

	printPoly("Umnozak polinoma: ", headPolyProduct.next);

	return EXIT_SUCCESS;
}

int readPolyFromFile(position headPoly1, position headPoly2) {
	FILE* filePointer = NULL;
	char buffer[MAX_LINE] = { 0 };
	int status = EXIT_SUCCESS;


	filePointer = fopen("polinomi.txt", "r");

	if (!filePointer) {
		printf("\nCan't open file polinomi.txt!\n");
		return FILE_NOT_OPEN;
	}

	fgets(buffer, MAX_LINE, filePointer);		//uzme prvi cili line (redak) teksta sta je zap cili prvi polinom

	status = parseStringIntoList(headPoly1,buffer);

	if (status != EXIT_SUCCESS)
		return EXIT_FAILURE;

	fgets(buffer, MAX_LINE, filePointer);

	status = parseStringIntoList(headPoly2, buffer);

	if (status != EXIT_SUCCESS)
		return EXIT_FAILURE;
	
	fclose(filePointer);

	return EXIT_SUCCESS;
}

int parseStringIntoList(position headPoly, char buffer[MAX_LINE]) {
	int coeff = 0, exp = 0,numBytes=0;
	int status = EXIT_SUCCESS;
	position newElement = NULL;

	while (strlen(buffer)>0) {
		status=sscanf(buffer, " %dx^%d %n", &coeff, &exp, &numBytes);

		if (status != 2) {							//zato sto sscanf vraca broj podataka skeniranih, a u ovom slucaju bi to trebalo bit 2
			printf("\nThis file is not good!\n");
			return EXIT_FAILURE;
		}
			
		newElement=createElement(coeff, exp);

		if (!newElement)
			return EXIT_FAILURE;

		sortedInput(headPoly, newElement);


		buffer += numBytes;
	}


	return EXIT_SUCCESS;
}


position createElement(int coeff, int exp) {
	position newElement = NULL;

	newElement = malloc(sizeof(Element));

	if (!newElement) {
		printf("\nUnable to locate memmory for newElement!\n");
		return FAILED_MEMORY_ALLOCATION;
	}

	newElement->coeff = coeff;
	newElement->exp = exp;
	newElement->next = NULL;

	return newElement;
}

int sortedInput(position headPoly, position newElement) {
	position current = headPoly;

	while (current->next!=NULL && newElement->exp < current->next->exp)
		current = current->next;

	if (current->next==NULL || newElement->exp > current->next->exp){

		newElement->next = current->next;
		current->next = newElement;
	}


	else{
		
		mergeElements(current, newElement);
	}
			
	return EXIT_SUCCESS;
}

int mergeElements(position prevElementInList, position newElement) {
	int resultCoeff = 0;
	position temp = NULL;

	resultCoeff = prevElementInList->next->coeff + newElement->coeff;

	if (resultCoeff != 0)
		prevElementInList->next->coeff = resultCoeff;

	else {
		temp = prevElementInList->next;
		prevElementInList->next = temp->next;

		free(temp);
	
	}

	free(newElement);		// ovo radimo jer ako mergeamo onda ne moramo dodavat novi element polinoma u listu nego samo nadodajemo na postojeci pa nam je newElement beskorisan


	return EXIT_SUCCESS;
}

int polySum(position firstElPoly1, position firstElPoly2, position headPolySum) {
	position currentPoly1 = firstElPoly1, currentPoly2 = firstElPoly2, newElement = NULL,temp=NULL;
	int coeff = 0, exp = 0;


	while (currentPoly1 != NULL && currentPoly2 != NULL) {

		if (currentPoly1->exp == currentPoly2->exp) {
			exp = currentPoly1->exp;
			coeff = currentPoly1->coeff + currentPoly2->coeff;
			
			newElement = createElement(coeff, exp);
			sortedInput(headPolySum, newElement);
		}

		else {
			exp = currentPoly1->exp;
			coeff = currentPoly1->coeff;

			newElement = createElement(coeff, exp);
			sortedInput(headPolySum, newElement);

			exp = currentPoly2->exp;
			coeff = currentPoly2->coeff;

			newElement = createElement(coeff, exp);
			sortedInput(headPolySum, newElement);
		}

		currentPoly1 = currentPoly1->next;
		currentPoly2 = currentPoly2->next;

	}

	if (currentPoly1 == NULL)
		temp = currentPoly2;

	else
		temp = currentPoly1;

	while (temp != NULL) {

		exp = temp->exp;
		coeff = temp->coeff;

		newElement = createElement(coeff, exp);
		sortedInput(headPolySum, newElement);

		temp = temp->next;
	}

	return EXIT_SUCCESS;
}

int polyProduct(position firstElPoly1, position firstElPoly2, position headPolyProduct) {
	position currentPoly1 = firstElPoly1, currentPoly2 = firstElPoly2,newElement=NULL;
	int coeff = 0, exp = 0;

	while (currentPoly1 != NULL) {

		while (currentPoly2 != NULL) {
	
			coeff = currentPoly1->coeff * currentPoly2->coeff;
			exp = currentPoly1->exp + currentPoly2->exp;

			newElement=createElement(coeff, exp);
			sortedInput(headPolyProduct, newElement);

			currentPoly2 = currentPoly2->next;
		
		}

		currentPoly1 = currentPoly1->next;

		currentPoly2 = firstElPoly2;
	}


	return EXIT_SUCCESS;
}


int printPoly(char* polyName,position firstEl) {
	position current = firstEl;

	printf("\n----------------\n %s:  ",polyName);

	while (current != NULL) {

		if (current->next == NULL) {
			printf("%dx^%d", current->coeff, current->exp);
			current = current->next;
		}

		else {
			printf("%dx^%d + ", current->coeff, current->exp);
			current = current->next;
		}
	}

	printf("\n----------------\n");

	return EXIT_SUCCESS;
}


