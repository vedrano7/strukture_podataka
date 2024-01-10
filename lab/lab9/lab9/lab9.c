/*9. Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u čvorove binarnog stabla.

a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraća
pokazivač na korijen stabla.

b) Napisati funkciju replace koja će svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu (tj. sumom svih potomaka prije zamjene
vrijednosti u tim potomcima). Npr. stablo sa slike Slika 1 transformirat će se u stablo na
slici Slika 2.

c) Prepraviti program na način da umjesto predefiniranog cjelobrojnog polja korištenjem
funkcije rand() generira slučajne brojeve u rasponu <10, 90>. Također, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon korištenja funkcije iz a), zatim b)
dijela zadatka.*/


//U ZADATKU JE BINARNO STABLO PRETRAŽIVANJA NACRTANO ZRCALNO (MANJI ELEMENTI U DESNOM PODSTABLU, A VEĆI U LIJEVOM), ALI GA MI ODI RIŠAVAMO ISTO KAO I INAČE TJ. PRAVIMO SE DA JE NACRTANO KAO I INAČE JER JE TAKO LAKŠE!!!

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>
#include<time.h>


typedef struct TreeNode* position;
typedef struct TreeNode {
	int el;
	position leftChild, rightChild;
}treeNode;


position insert(position root, int newEl);
position createNode(int newEl);
int replace(position root);
int levelorderPrint(position root);
int random();
int randTreeFileInorderPrint(position root,FILE* RTFile);

int main() {
	position root = NULL, rootRand = NULL;
	srand(time(0));
	int i = 0;
	FILE* RTFile = NULL;

	//a)
	root = insert(root, 2);
	insert(root, 5);
	insert(root, 7);
	insert(root, 8);
	insert(root, 11);
	insert(root, 1);
	insert(root, 4);
	insert(root, 2);
	insert(root, 3);
	insert(root, 7);

	levelorderPrint(root);		//za provjeru
	printf("\n");

	//b)
	replace(root);

	levelorderPrint(root);		//za provjeru	
	printf("\n");

	//c)
	rootRand = insert(rootRand, random());

	for (i = 0; i < 9; i++)
		insert(rootRand, random());


	levelorderPrint(rootRand);		//za provjeru
	printf("\n");

	RTFile = fopen("randTree.txt", "w");

	randTreeFileInorderPrint(rootRand, RTFile);

	fprintf(RTFile, "\n");

	replace(rootRand);

	levelorderPrint(rootRand);		//za provjeru
	printf("\n");

	randTreeFileInorderPrint(rootRand, RTFile);

	fclose(RTFile);
	
	return EXIT_SUCCESS;
}

//za provjeru
int levelorderPrint(position root) {
	position current = NULL, queue[100] = { NULL };
	int front = 0, rear = 0;

	queue[rear] = root;

	while (front <= rear) {

		current = queue[front];

		printf("%d ", current->el);

		if (current->leftChild != NULL)
			queue[++rear] = current->leftChild;

		if (current->rightChild != NULL)
			queue[++rear] = current->rightChild;

		front++;
	}

	return EXIT_SUCCESS;
}

//a)
position insert(position root, int newEl) {

	if (root == NULL)
		return createNode(newEl);

	if (newEl < root->el)
		root->leftChild = insert(root->leftChild, newEl);
	//vidimo po slici da mora bit >= , a ne <= jer bi sa <= elementi koji se ponavljaju (2 i 7) bili u suprotnim podstablima nego sto su na slici
	else if (newEl >= root->el)
		root->rightChild = insert(root->rightChild, newEl);

	return root;
}

position createNode(int newEl) {
	position newNode = NULL;

	newNode = malloc(sizeof(treeNode));

	if (!newNode) {
		printf("\nNeuspjela alokacija memorije za newNode!\n");
		return NULL;
	}

	newNode->el = newEl;
	newNode->leftChild = NULL;
	newNode->rightChild = NULL;

	return newNode;
}

//b)
int replace(position root) {
	int leftEl = 0, rightEl = 0, ogEl = 0;

	if (root == NULL)
		return 0;

	ogEl = root->el;

	leftEl = replace(root->leftChild);
	rightEl = replace(root->rightChild);

	root->el = leftEl + rightEl;

	return ogEl + root->el;
}

//c)
int random() {
	return (rand() % (90 - 10 + 1)) + 10;
}

int randTreeFileInorderPrint(position root,FILE * RTFile) {

	if (root != NULL) {
		randTreeFileInorderPrint(root->leftChild, RTFile);
		fprintf(RTFile,"%d ", root->el);
		randTreeFileInorderPrint(root->rightChild, RTFile);
	}

	return EXIT_SUCCESS;

}
