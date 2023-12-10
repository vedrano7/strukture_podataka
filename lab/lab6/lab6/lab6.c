/*6. Napisati program koji čita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini račun.Na početku svake datoteke je zapisan datum u kojem vremenu je
račun izdat u formatu YYYY - MM - DD.Svaki sljedeći red u datoteci predstavlja artikl u formatu
naziv, količina, cijena.Potrebno je formirati vezanu listu računa sortiranu po datumu.Svaki čvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla.Nakon toga potrebno je
omogućiti upit kojim će korisnik saznati koliko je novaca sveukupno potrošeno na specifični
artikl u određenom vremenskom razdoblju i u kojoj je količini isti kupljen*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LENGTH (50)
#define MAX_DATE_LENGTH (11)
#define MAX_LINE (1024)
#define FILE_NOT_OPEN (-1)
#define FAILED_MEMORY_ALLOCATION (NULL)


typedef struct _article* articlePosition;
typedef struct _article{
	char name[MAX_LENGTH];
	int quantity;
	int price;
	articlePosition next;
}article;

typedef struct _receipt* receiptPosition;
typedef struct _receipt{
	char receiptDate[MAX_DATE_LENGTH];
	article articleHead;
	receiptPosition next;

}receipt;

int readAndCreateReceiptList(receiptPosition receiptHead);
receiptPosition createAndFillReceiptNode(char* receiptName);
articlePosition createAndFillArticleNode(char* articleName, int articleQuantity, int articlePrice);
int articleSortedInput(articlePosition articleHead, articlePosition newArticleNode);
int receiptSortedInput(receiptPosition receiptHead, receiptPosition newReceiptNode);
int compareDates(char* date1, char* date2);
int mergeReceipts(receiptPosition newReceiptNode,receiptPosition currentReceiptInList);
int findAndPrintPriceAndQuantityOfArticleInTimeframe(receiptPosition receiptHead, char* wantedArticleName, char* dateFrom, char* dateTo);
int printPriceAndQuantityOfArticleInTimeFrame(receiptPosition startReceipt, receiptPosition endReceipt,char* wantedArticleName);

int printReceiptList(receiptPosition firstEl);
int printArticleList(articlePosition head);


int main() {
	receipt receiptHead = { {0},{{0},0,0,NULL},NULL };
	char dateFrom[MAX_LENGTH] = { '\0' }, dateTo[MAX_LENGTH] = { '\0' }, articleName[MAX_LENGTH] = { '\0' };

	readAndCreateReceiptList(&receiptHead); 
	
	printf("\nZa saznati koliko je sveukupno novca potroseno i na koliku kolicina odredenog artikla u nekom vremenskom razdoblju upisite ime tog artikla i trazeno vremensko razdoblje u formatu 'ime YYYY-MM-DD YYYY-MM-DD' : ");
	scanf("%s %s %s", articleName, dateFrom, dateTo);

	findAndPrintPriceAndQuantityOfArticleInTimeframe(&receiptHead, articleName, dateFrom, dateTo);

	return EXIT_SUCCESS;
}


int readAndCreateReceiptList(receiptPosition receiptHead) {
	FILE* filename = NULL;
	char receiptName[MAX_LENGTH] = { '\0' };
	receiptPosition newReceiptNode = NULL;

	filename = fopen("racuni.txt", "r");

	if (!filename) {
		printf("\nCan't open file!\n");
		return FILE_NOT_OPEN;
	}

	while (!feof(filename)) {
		fscanf(filename, " %s ", receiptName);
		newReceiptNode = createAndFillReceiptNode(receiptName);
		receiptSortedInput(receiptHead, newReceiptNode);
	}

	fclose(filename);

	return EXIT_SUCCESS;
}


receiptPosition createAndFillReceiptNode(char* receiptName) {
	FILE* filename = NULL;
	char receiptDate[MAX_LENGTH] = { 0 }, buffer[MAX_LINE] = { 0 }, articleName[MAX_LENGTH];
	int articleQuantity = 0, articlePrice = 0;
	receiptPosition newReceiptNode = NULL;
	articlePosition newArticleNode = NULL;
	
	filename = fopen(receiptName, "r");

	fscanf(filename, "%s ", receiptDate);

	newReceiptNode = malloc(sizeof(receipt));

	strcpy(newReceiptNode->receiptDate, receiptDate);
	newReceiptNode->next = NULL;
	newReceiptNode->articleHead.next = NULL;

	while (!feof(filename)) {
		fscanf(filename," %s %d %d ",articleName,&articleQuantity,&articlePrice);
		newArticleNode = createAndFillArticleNode(articleName, articleQuantity, articlePrice);
		articleSortedInput(&newReceiptNode->articleHead, newArticleNode);
	}


	//printf("\n%s\n", newReceiptNode->receiptDate);

	//printArticleList(&newReceiptNode->articleHead);

	fclose(filename);

	return newReceiptNode;
}


articlePosition createAndFillArticleNode(char* articleName,int articleQuantity, int articlePrice) {
	articlePosition newArticleNode = NULL;

	newArticleNode = malloc(sizeof(article));

	strcpy(newArticleNode->name, articleName);
	newArticleNode->quantity = articleQuantity;
	newArticleNode->price = articlePrice;
	newArticleNode->next = NULL;

	//printf("\n%s %d %d\n", newArticleNode->name, newArticleNode->quantity, newArticleNode->price);
	
	return newArticleNode;
}


int articleSortedInput(articlePosition articleHead, articlePosition newArticleNode) {
	articlePosition current = articleHead;

	while (current->next != NULL && strcmp(newArticleNode->name, current->next->name) > 0 )
		current = current->next;
	
	if (current->next != NULL && strcmp(newArticleNode->name, current->next->name) == 0) {
		current->next->quantity += newArticleNode->quantity;
		current->next->price += newArticleNode->price;
	}

	else {
		newArticleNode->next = current->next;
		current->next = newArticleNode;
	}

	return EXIT_SUCCESS;
}


int receiptSortedInput(receiptPosition receiptHead, receiptPosition newReceiptNode) {
	char receiptDate[MAX_LENGTH] = { '\0' };
	receiptPosition current = receiptHead;

	while (current->next != NULL && compareDates(newReceiptNode->receiptDate, current->next->receiptDate)==1)
		current = current->next;


	if (current->next != NULL && compareDates(newReceiptNode->receiptDate, current->next->receiptDate) == 0) {
		mergeReceipts(newReceiptNode, current->next);
		free(newReceiptNode);
	}

	else {
		newReceiptNode->next = current->next;
		current->next = newReceiptNode;
	}

	return EXIT_SUCCESS;
}


int compareDates(char* date1, char* date2) {
	int year1 = 0, year2 = 0, month1 = 0, month2 = 0, day1 = 0, day2 = 0;

	sscanf(date1, "%d-%d-%d", &year1, &month1, &day1);
	sscanf(date2, "%d-%d-%d", &year2, &month2, &day2);

	
	if (year1 > year2)
		return 1;
	else if (year1 < year2)
		return -1;

	if (month1 > month2)
		return 1;
	else if (month1 < month2)
		return -1;

	if (day1 > day2)
		return 1;
	else if (day1 < day2)
		return -1;

	return 0;
}


int mergeReceipts(receiptPosition newReceiptNode, receiptPosition currentReceiptInList) {
	articlePosition current = &newReceiptNode->articleHead, temp = NULL;

	while (current->next != NULL) {
		temp=createAndFillArticleNode(current->next->name, current->next->quantity, current->next->price);

		articleSortedInput(&currentReceiptInList->articleHead, temp);

		current = current->next;
	}

	return EXIT_SUCCESS;
}


int findAndPrintPriceAndQuantityOfArticleInTimeframe(receiptPosition receiptHead, char* wantedArticleName, char* dateFrom, char* dateTo) {
	receiptPosition current = receiptHead, startReceipt = NULL, endReceipt = NULL;

	while (current->next != NULL) {

		if (compareDates(dateFrom, current->next->receiptDate) != 1) {
			startReceipt = current->next;
			break;
		}


		current = current->next;
	}


	current = receiptHead;

	while (current->next != NULL) {
		if (compareDates(dateTo, current->next->receiptDate) == 1)
			endReceipt = current->next;

		current = current->next;
	}

	//printArticleList(&startReceipt->articleHead);
	//printArticleList(&endReceipt->articleHead);

	if (startReceipt==NULL || endReceipt == NULL)
		printf("\nNema racuna u trazenom vremenskom razdoblju!\n");

	else
		printPriceAndQuantityOfArticleInTimeFrame(startReceipt, endReceipt,wantedArticleName);

	return EXIT_SUCCESS;
}


int printPriceAndQuantityOfArticleInTimeFrame(receiptPosition startReceipt, receiptPosition endReceipt,char* wantedArticleName) {
	receiptPosition receiptCurrent = startReceipt;
	articlePosition articleCurrent = NULL;
	int totalQuantity = 0, totalPrice = 0;

	if (startReceipt == endReceipt) {
		articleCurrent = &receiptCurrent->articleHead;

		while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name,wantedArticleName)!=0)
			articleCurrent = articleCurrent->next;
		
		if (articleCurrent->next != NULL)
			printf("\nUkupna kolicina artikla je bila %d i kostala je %d eura!\n", articleCurrent->next->quantity, articleCurrent->next->price);

		else
			printf("\nArtikl nije kupljen u trazenom vremenskom razdoblju!\n");
	}

	else {

		while (receiptCurrent!= endReceipt->next) {

			articleCurrent = &receiptCurrent->articleHead;

			while (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName)!=0)
				articleCurrent = articleCurrent->next;


			if (articleCurrent->next != NULL && strcmp(articleCurrent->next->name, wantedArticleName) == 0) {
				totalQuantity += articleCurrent->next->quantity;
				totalPrice += articleCurrent->next->price;
			}

			receiptCurrent = receiptCurrent->next;
		}

		if (totalQuantity != 0)
			printf("\nUkupna kolicina artikla je bila %d i kostala je %d eura!\n", totalQuantity, totalPrice);


		else
			printf("\nArtikl nije kupljen u trazenom vremenskom razdoblju!\n");
	}
	
	return EXIT_SUCCESS;
}
