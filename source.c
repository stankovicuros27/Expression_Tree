#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>

#define MAX_CAP 100


//-----Character Stack-----
struct CharStack {
	int top;
	unsigned capacity;
	char** array;
};

struct CharStack* create_char_stack(unsigned capacity) {
	struct CharStack* stack = (struct CharStack*)malloc(sizeof(struct CharStack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (char**)malloc(stack->capacity * sizeof(char*));
	return stack;
}

int chstack_is_full(struct CharStack* stack) {
	return stack->top == stack->capacity - 1;
}

int chstack_is_empty(struct CharStack* stack) {
	return stack->top == -1;
}

void chstack_push(struct CharStack* stack, char* item)
{
	if (chstack_is_full(stack))
		return;
	stack->array[++stack->top] = item;
	printf("%s pushed to stack\n", item);
}

const char* chstack_pop(struct CharStack* stack)
{
	if (chstack_is_empty(stack)) {
		return "";
	}
	return stack->array[stack->top--];
}

const char* chstack_peek(struct CharStack* stack) {
	if (chstack_is_empty(stack)) {
		return "";
	}
	return stack->array[stack->top];
}
//-----Character Stack End-----

int is_operand(char* ch) {
	if (strlen(ch) > 2) return 0;
	if (ch[0] >= 'A' && ch[0] <= 'Z') return 1;
	return 0;
}

int get_priority(char* operation) {
	if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) return 1;
	if (strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) return 2;
	if (strcmp(operation, "^") == 0) return 3;
	if (strcmp(operation, "ln") == 0 || strcmp(operation, "tg") == 0) return 4;
	if (strcmp(operation, "(-)") == 0) return 4;
	return -1;
}

int is_operation(char* operation) {
	if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) return 1;
	if (strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) return 2;
	if (strcmp(operation, "^") == 0) return 3;
	if (strcmp(operation, "ln") == 0 || strcmp(operation, "tg") == 0) return 4;
	if (strcmp(operation, "(-)") == 0) return 4;
	return -1;
}

int get_rang(char* operation) {
	if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) return 1;
	if (strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) return 1;
	if (strcmp(operation, "^") == 0) return 1;
	if (strcmp(operation, "ln") == 0 ) return 0;
	if (strcmp(operation, "(-)") == 0 || strcmp(operation, "tg") == 0) return 0;
	return 0;
}

char* infix_to_postfix(char* expression) {
	int i = 0;
	int rang = 0;

	struct CharStack* stack = create_char_stack(MAX_CAP);
	if (!stack) {
		return -1;
	}

	char delim[] = " ";

	char* ptr = strtok(expression, delim);

	char res[MAX_CAP] = "";

	while (ptr != NULL) 
	{
		if (is_operand(ptr)) {
			rang++;
			strcat(res, ptr);
			strcat(res, " ");
		}
		else if (strcmp(ptr, "(") == 0)
			chstack_push(stack, ptr);
		else if (strcmp(ptr, ")") == 0) {
			while (!chstack_is_empty(stack) && strcmp(chstack_peek(stack), "(")) {
				strcat(res, chstack_peek(stack));
				strcat(res, " ");
				rang -= get_rang(chstack_pop(stack));
				if (rang < 1) {
					printf("Invalid expression!\n");
					exit(1);
				}
			}
			if (!chstack_is_empty(stack) && strcmp(chstack_peek(stack), "(")) {
				printf("Invalid expression!\n");
				exit(1);
			}
			else { chstack_pop(stack); }
		}
		else {
			while (!chstack_is_empty(stack) && get_priority(ptr) <= get_priority(chstack_peek(stack))) {
				strcat(res, chstack_peek(stack));
				strcat(res, " ");
				rang -= get_rang(chstack_pop(stack));
				if (rang < 1) {
					printf("Invalid expression!\n");
					exit(1);
				}
			}
			chstack_push(stack, ptr);
		}
		ptr = strtok(NULL, delim);
	}

	while (!chstack_is_empty(stack)) {
		strcat(res, chstack_pop(stack));
		strcat(res, " ");
	}

	return res;
}

int main() {
	struct CharStack* operationStack = create_char_stack(MAX_CAP);
	struct CharStack* operandStack = create_char_stack(MAX_CAP);
	char expression[MAX_CAP];
	printf("Enter mathematical expression:\n");
	char* str_test = fgets(expression, MAX_CAP, stdin);
	if (!str_test) {
		printf("Invalid expression! Terminating program...");
		exit(1);
	}
	expression[strcspn(expression, "\n")] = 0;
	printf("%s\n\n", expression);

	char res[MAX_CAP];

	strcpy(res, infix_to_postfix(expression));
	printf("%s", res);
}