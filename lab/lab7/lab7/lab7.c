/*7. Napisati program koji pomoću vezanih listi(stabala) predstavlja strukturu direktorija.
Omogućiti unos novih direktorija i pod - direktorija, ispis sadržaja direktorija i
povratak u prethodni direktorij.Točnije program treba preko menija simulirati
korištenje DOS naredbi : 1 - "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 50

typedef struct DirNode* dirPosition;
typedef struct DirNode {
	char name[MAX_NAME_LENGTH];
	dirPosition nextDir;
	dirPosition subDir;

}dirNode;

typedef struct LevelStackNode* stackPosition;
typedef struct LevelStackNode {
	dirPosition dirLevelEl;
	stackPosition next;

}levelStackNode;

int menu(dirPosition rootDir, stackPosition stackTop);
dirPosition createSubdir(dirPosition currentDir, char* dirName);
int printDirContents(dirPosition currentDir);		//ispise ime direktorija u kojem se nalazimo i imena svih njegovih poddirektorija
dirPosition changeDir(dirPosition currentDir, char* wantedDir);		//pomakne nas u neki poddirektorij direktorija u kojeg se nalazimo


int push(stackPosition stackHead, dirPosition dirLevel);
dirPosition pop(stackPosition stackTop);

int main() {
	dirNode dirHead = { {0},NULL,NULL };
	levelStackNode stackHead = { NULL,NULL };

	dirPosition rootDir = NULL;

	rootDir=createSubdir(&dirHead, "C:");

	push(&stackHead, rootDir);

	menu(rootDir, &stackHead);

	return EXIT_SUCCESS;
}

int menu(dirPosition rootDir, stackPosition stackTop) {
	char pick = '\0', dirName[MAX_NAME_LENGTH] = { 0 };
	dirPosition currentDir = rootDir, result = NULL;

	while (1) {
	
		printf("\nMenu:\n");
		printf("1 - md (Create Directory)\n");
		printf("2 - cd dir (Change Directory)\n");
		printf("3 - cd.. (Go Up)\n");				//pomaknuti se za jedan level gore u dubljini stabla (ic u roditeljski direktorij od onog u kojeg se nalazimo)
		printf("4 - dir (List Contents)\n");
		printf("5 - exit\n");

		printf("Enter your choice: ");
		scanf(" %c", &pick);
	
		switch (pick) {

			case '1':
				printf("\nEnter new directory name: ");
				scanf("%s", dirName);

				result=createSubdir(currentDir, dirName);

				if (result != NULL)
					printf("\nSuccessfully created subdirectory %s\n", dirName);

				else
					printf("\nUnsuccessful creation of subdirectory!\n");

				continue;

			case '2':
				printf("\nEnter destination directory name: ");
				scanf(" %s", dirName);

				result = changeDir(currentDir, dirName);


				if (result != NULL) {
					printf("\nSuccessfully moved to directory %s\n", dirName);
					push(stackTop, result);
					currentDir = result;
				}

				else
					printf("\nDestination directory not found in current directory!\n");

				continue;

			case '3':
				if (currentDir != rootDir) {
					currentDir = pop(stackTop);
					printf("\nSuccessfully moved to directory %s\n", currentDir->name);
				}

				else
					printf("\nAlready in root directory!\n");

				continue;

			case '4':
				printDirContents(currentDir);

				continue;
			case '5':
				printf("\nExiting the program.\n");

				break;

			default:
				printf("\nInvalid input, please try again.\n");

				continue;
		}

		break;
	}


}

dirPosition createSubdir(dirPosition currentDir, char* dirName) {
	dirPosition newDir = NULL;

	newDir = malloc(sizeof(dirNode));

	if (!newDir) {
		printf("\nUnable to allocate memory for newDir!\n");
		return NULL;
	}

	strcpy(newDir->name, dirName);
	newDir->subDir = NULL;

	newDir->nextDir = currentDir->subDir;
	currentDir->subDir = newDir;

	return newDir;
}

int printDirContents(dirPosition currentDir) {
	dirPosition currentSubdir = currentDir->subDir;

	printf("\nContents of directory %s:\n", currentDir->name);

	while (currentSubdir != NULL) {
		printf("%s\n", currentSubdir->name);
		currentSubdir = currentSubdir->nextDir;
	}

	if (currentDir->subDir == NULL)
		printf("(empty)\n");

	return EXIT_SUCCESS;
}

dirPosition changeDir(dirPosition currentDir, char* wantedDir) {
	dirPosition currentSubdir = currentDir->subDir;

	while (currentSubdir != NULL && strcmp(currentSubdir->name, wantedDir) != 0)
		currentSubdir = currentSubdir->nextDir;

	return currentSubdir;
}

int push(stackPosition stackTop, dirPosition dirLevel) {
	stackPosition newStackNode = NULL;

	newStackNode = malloc(sizeof(levelStackNode));

	newStackNode->dirLevelEl = dirLevel;

	newStackNode->next = stackTop->next;
	stackTop->next = newStackNode;

	return EXIT_SUCCESS;
}

dirPosition pop(stackPosition stackTop) {
	dirPosition dirLevel = NULL;
	stackPosition toDel = stackTop->next;

	stackTop->next = toDel->next;
	free(toDel);

	dirLevel = stackTop->next->dirLevelEl;

	return dirLevel;
}

