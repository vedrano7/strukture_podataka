/*5. Napisati program koji iz datoteke čita postfiks izraz i zatim korištenjem stoga računa
rezultat.Stog je potrebno realizirati preko vezane liste.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE (1024)
#define MAX_LENGHT (1024)


typedef struct stackElement* position;

typedef struct stackElement {
	double number;
	position next;

}StackElement;


int readFromFileAndCalculatePostfix(position head,double* result,char* postfixEq);
int push(double number, position head);
int checkStackAndExtractResultUsingOperator(position head, char operator);
int pop(head);

int main() {
	StackElement head = { 0,NULL };
	char postfixEq[MAX_LENGHT] = { 0 };
	double result = 0;

	if (readFromFileAndCalculatePostfix(&head, &result,postfixEq) == EXIT_SUCCESS)
		printf("\nRezultat je: %lf\n", result);


	return EXIT_SUCCESS;
}

int readFromFileAndCalculatePostfix(position head, double* result, char* postfixEq) {
	FILE* fileName = NULL;
	int numBytes = 0, status = 0;
	double number = 0;
	char operator='\0';

	fileName = fopen("postfix_eq.txt", "r");

	if (!fileName) {
		printf("\nUnable to open file postfix_eq.txt\n");

		return EXIT_FAILURE;
	}

	fgets(postfixEq, MAX_LINE, fileName);

	while (strlen(postfixEq) > 0) {

		status = sscanf(postfixEq, " %lf %n", &number, &numBytes);

		if (status == 1) {

			push(number, head);
		}

		else {

			sscanf(postfixEq," %c %n", &operator, &numBytes);

			checkStackAndExtractResultUsingOperator(head, operator);
		
		}

		postfixEq += numBytes;
	}

	*result = head->next->number;
	pop(head);

	fclose(fileName);

	return EXIT_SUCCESS;
}

int checkStackAndExtractResultUsingOperator(position head, char operator) {
	position current = head;
	double result = 0;
	int counter = 0;

	switch (operator) {

		case '+':

			result = 0;

			for (counter = 0; counter < 2; counter++) {

				current = head;

				while(current->next != NULL)
					current = current->next;

				result += current->number;

				pop(head);
			}

			push(result, head);

			break;

		case '-':

			result = 0;

			for (counter = 0; counter < 2; counter++) {

				current = head;

				while (current->next != NULL)
					current = current->next;

				if (result == 0)
					result = current->number;

				else
					result = current->number - result;

				pop(head);
			}

			push(result, head);

			break;

		case '*':

			result = 0;

			for (counter = 0; counter < 2; counter++) {

				current = head;

				while (current->next != NULL)
					current = current->next;

				if (result == 0)
					result = current->number;

				else
					result *= current->number;

				pop(head);
			}

			push(result, head);

			break;

		case '/':

			result = 0;

			for (counter = 0; counter < 2; counter++) {

				current = head;

				while (current->next != NULL)
					current = current->next;

				if (result == 0)
					result = current->number;

				else
					result = current->number / result;

				pop(head);
			}

			push(result, head);

			break;

		default:
			printf("\nOperation %c not supported yet!\r\n", operator);
			return EXIT_FAILURE;

	}


	return EXIT_SUCCESS;
}

int push(double number, position head) {
	position newElement = NULL, current = head;

	newElement = malloc(sizeof(StackElement));

	if (!newElement) {
		printf("\nUnable to allocate memory for newElement!\n");

		return EXIT_FAILURE;
	}

	newElement->number = number;

	while (current->next != NULL)
		current = current->next;

	newElement->next = current->next;
	current->next = newElement;


	return EXIT_SUCCESS;
}

int pop(head) {
	position current = head, temp = NULL;

	while (current->next->next != NULL)
		current = current->next;

	temp = current->next;
	current->next = temp->next;

	free(temp);

	return EXIT_SUCCESS;
}



