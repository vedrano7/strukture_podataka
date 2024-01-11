/*8. Napisati program koji omogućava rad s binarnim stablom pretraživanja.Treba
omogućiti unošenje novog elementa u stablo, ispis elemenata(inorder, preorder, postorder i
	level order), brisanje i pronalaženje nekog elementa*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<stdlib.h>

typedef struct TreeNode* position;
typedef struct TreeNode {
	int el;
	position leftChild, rightChild;
}treeNode;

typedef struct QueueNode*  qPosition;			//čvor za red za levelorder ispis
typedef struct QueueNode {
	position treeNode;
	qPosition nextQNode;
}queueNode;


int menu(position root);
position find(position root, int wantedEl);
position insert(position root, int newEl);
position createNode(int newEl);
position delete(position root, int elToDel);
int inorderPrint(position root);
int postorderPrint(position root);
int preorderPrint(position root);
int levelorderPrint(position root);
qPosition createQNode(position treeNode);


int main() {
	//treeNode treeHead = { 0,NULL,NULL };		//mozemo bez head-a her nam je redudantan
	position root = NULL;
	int rootVal = 0;

	printf("\nUnesite vrijednost korijena: ");
	scanf("%d", &rootVal);

	root = insert(root, rootVal);

	//treeHead.leftChild = &root;

	menu(root);

	return EXIT_SUCCESS;
}

int menu(position root) {
	char pick = '\0';
	int el = 0;
	position result = NULL;

	while (1) {
		printf("\nUpisite '1' za naci odreden element u stablu, '2' za dodati novi element u stablo, '3' za izbrisati element iz stabla, '4' za ispisati stablo u inorder obliku, '5' za ispisati stablo u postorder obliku, '6' za ispisati stablo u preorder obliku,'7' za ispisati stablo u levelorder obliku a 0 za izaci iz programa\n");
		scanf(" %c", &pick);

		switch (pick) {
			case '0':

				printf("\nKraj programa!\n");

				break;

			case '1':

				printf("\nUnesite vrijednost elementa kojeg zelite pronaci u stablu: ");
				scanf("%d", &el);

				result = find(root, el);

				if (result != NULL)
					printf("\Element %d se nalazi na adresi %p\n", el, result);

				else
					printf("\nNeuspjelo pronalazanje elementa %d!\n", el);

				continue;

			case '2':

				printf("\nUnesite vrijednost elementa kojeg zelite dodati u stablo: ");
				scanf("%d", &el);

				root=insert(root, el);

				continue;

			case '3':

				printf("\nUnesite vrijednost elementa kojeg zelite izbrisati iz stabla: ");
				scanf("%d", &el);

				root=delete(root, el);

				continue;

			case '4':

				inorderPrint(root);

				continue;


			case '5':

				postorderPrint(root);

				continue;

			case '6':

				preorderPrint(root);

				continue;

			case '7':

				levelorderPrint(root);

				continue;

			default:

				printf("\nKrivi unos!!\n");

				continue;
			}

		break;
	}

	return EXIT_SUCCESS;
}


position find(position root, int wantedEl) {
	if (root == NULL)
		return NULL;

	else if (wantedEl < root->el)
		return find(root->leftChild, wantedEl);

	else if (wantedEl > root->el)
		return find(root->rightChild, wantedEl);

	return root;
}

position insert(position root, int newEl) {

	if (root == NULL)
		return createNode(newEl);

	if (newEl <= root->el)
		root->leftChild = insert(root->leftChild, newEl);

	else if (newEl > root->el)
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

position delete(position root, int elToDel) {
	position temp;

	if (root == NULL)
		printf("\nElement kojeg zelite obrisati se ne nalazi u stablu!\n");


	else if (elToDel < root->el)
		root->leftChild = delete(root->leftChild, elToDel);		

	else if (elToDel > root->el)
		root->rightChild = delete(root->rightChild, elToDel);	


	else if (root->leftChild != NULL && root->rightChild != NULL) {	

		temp = root->rightChild;
	
		while (temp->leftChild != NULL)
			temp = temp->leftChild;

		root->el = temp->el;

		root->rightChild = delete(root->rightChild, temp->el);

	}

	else {		

		temp = root;

		if (root->leftChild != NULL)
			root = root->leftChild;
		
		else
			root = root->rightChild;
		
		free(temp);

	}

	return root;
}

int inorderPrint(position root) {

	if (root != NULL) {
		inorderPrint(root->leftChild);
		printf("%d ", root->el);
		inorderPrint(root->rightChild);
	}

	return EXIT_SUCCESS;
}

int postorderPrint(position root) {

	if (root != NULL) {
		postorderPrint(root->leftChild);
		postorderPrint(root->rightChild);
		printf("%d ", root->el);

	}

	return EXIT_SUCCESS;
}

int preorderPrint(position root) {

	if (root != NULL) {
		printf("%d ", root->el);
		preorderPrint(root->leftChild);
		preorderPrint(root->rightChild);

	}

	return EXIT_SUCCESS;
}

int levelorderPrint(position root) {
	position currentTreeNode = NULL;
	qPosition temp = NULL, front = NULL, rear = NULL;

	front = rear =createQNode(root);
	
	while (front!=NULL) {

		currentTreeNode = front->treeNode;

		printf("%d ", currentTreeNode->el);

		if (currentTreeNode->leftChild != NULL) {
			//zapravo enqueue
			temp = rear;
			rear = createQNode(currentTreeNode->leftChild);
			temp->nextQNode = rear;
		}

		if (currentTreeNode->rightChild != NULL) {
			//zapravo enqueue
			temp = rear;
			rear = createQNode(currentTreeNode->rightChild);
			temp->nextQNode = rear;
		}
		
		//zapravo dequeue
		temp = front;
		front = front->nextQNode;
		free(temp);
	}

	return EXIT_SUCCESS;
}

qPosition createQNode(position treeNode) {
	qPosition temp, newNode = NULL;

	newNode = malloc(sizeof(queueNode));

	if (!newNode) {
		printf("\nNeuspjela alokacija memorije za newNode!\n");
		return NULL;
	}

	newNode->treeNode = treeNode;
	newNode->nextQNode = NULL;

	return newNode;
}
