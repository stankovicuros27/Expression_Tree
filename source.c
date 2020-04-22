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
	//printf("%s pushed to stack\n", item);
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
	if (strcmp(operation, "ln") == 0) return 0;
	if (strcmp(operation, "(-)") == 0 || strcmp(operation, "tg") == 0) return 0;
	return 0;
}



//-----Tree and Nodes-----
struct Node {
	char sym[5];
	int val;
	struct Node* left, * right, *parent;
};

struct Node* create_node(char* sym) {
	struct Node* node = (struct Node*)malloc(sizeof(struct Node));
	strcpy(node->sym, sym);
	node->parent = NULL;
	node->left = NULL;
	node->right = NULL;
	return node;
}

struct NodeStack {
	int top;
	unsigned capacity;
	struct Node** array;
};

struct NodeStack* create_node_stack(unsigned capacity) {
	struct NodeStack* stack = (struct NodeStack*)malloc(sizeof(struct NodeStack));
	stack->capacity = capacity;
	stack->top = -1;
	stack->array = (struct Node**)malloc(stack->capacity * sizeof(struct Node*));
	return stack;
}

int ndstack_is_full(struct NodeStack* stack) {
	return stack->top == stack->capacity - 1;
}

int ndstack_is_empty(struct NodeStack* stack) {
	return stack->top == -1;
}

void ndstack_push(struct NodeStack* stack, struct Node* item)
{
	if (ndstack_is_full(stack))
		return;
	stack->array[++stack->top] = item;
	//printf("%s pushed to stack\n", item);
}

struct Node* ndstack_pop(struct NodeStack* stack)
{
	if (ndstack_is_empty(stack)) {
		return NULL;
	}
	return stack->array[stack->top--];
}

const char* ndstack_peek(struct NodeStack* stack) {
	if (ndstack_is_empty(stack)) {
		return NULL;
	}
	return stack->array[stack->top];
}

struct NodeQueue {
	int front, rear, size;
	unsigned capacity;
	struct Node** array;
};

struct NodeQueue* create_node_queue(unsigned capacity) {
	struct NodeQueue* queue = (struct NodeQueue*)malloc(sizeof(struct NodeQueue));
	queue->capacity = capacity;
	queue->front = queue->size = 0;
	queue->rear = capacity - 1;
	queue->array = (struct Node**)malloc(queue->capacity * sizeof(struct Node*));
	return queue;
}

int ndqueue_is_full(struct NodeQueue* queue) {
	return (queue->size == queue->capacity);
}

int ndqueue_is_empty(struct NodeQueue* queue) {
	return (queue->size == 0);
}

void ndqueue_insert(struct NodeQueue* queue, struct Node* item) {
	if (ndqueue_is_full(queue))
		return;
	queue->rear = (queue->rear + 1) % queue->capacity;
	queue->array[queue->rear] = item;
	queue->size = queue->size + 1;
	//printf("%d enqueued to queue\n", item);
}

struct Node* ndqueue_delete(struct NodeQueue* queue)
{
	if (ndqueue_is_empty(queue))
		return NULL;
	struct Node* item = queue->array[queue->front];
	queue->front = (queue->front + 1) % queue->capacity;
	queue->size = queue->size - 1;
	return item;
}

struct Node* make_tree(char* postfix) {
	struct NodeStack* stack = create_node_stack(MAX_CAP);
	if (!stack) {
		return NULL;
	}

	struct Node* p = NULL, *t1, *t2;

	char delim[] = " ";

	char* ptr = strtok(postfix, delim);

	while (ptr != NULL) {
		if (is_operand(ptr)) {
			p = create_node(ptr);
			ndstack_push(stack, p);
		}
		else {
			p = create_node(ptr);
			t1 = ndstack_pop(stack);
			t2 = ndstack_pop(stack);

			p->right = t1;
			p->left = t2;

			ndstack_push(stack, p);
		}
		ptr = strtok(NULL, delim);
	}

	p = ndstack_pop(stack);
	return p;
}

//recursive, but used only for testing!
void inorder(struct Node* node) {
	if (node) {
		inorder(node->left);
		printf("%s ", node->sym);
		inorder(node->right);
	}
}

void levelorder(struct Node* node) {
	struct NodeQueue* queue = create_node_queue(MAX_CAP);
	ndqueue_insert(queue, node);
	ndqueue_insert(queue, NULL);
	struct Node* p = NULL;

	while (!ndqueue_is_empty(queue)) {
		p = ndqueue_delete(queue);
		if (!p) {
			if (!ndqueue_is_empty(queue))
				ndqueue_insert(queue, NULL);
			printf("\n");
			continue;
		}
		printf("%s ", p->sym);
		if (p->left) ndqueue_insert(queue, p->left);
		if (p->right) ndqueue_insert(queue, p->right);
	}
}
//-----Tree and Nodes End-----



char* infix_to_postfix(char* expression) {
	int i = 0;
	int rang = 0;

	struct CharStack* stack = create_char_stack(MAX_CAP);
	if (!stack) {
		return NULL;
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
	char expression[MAX_CAP];
	printf("Enter mathematical expression:\n");
	char* str_test = fgets(expression, MAX_CAP, stdin);
	if (!str_test) {
		printf("Invalid expression! Terminating program...");
		exit(1);
	}
	expression[strcspn(expression, "\n")] = 0;
	printf("You entered:\n%s\n\n", expression);

	char res[MAX_CAP];

	strcpy(res, infix_to_postfix(expression));
	printf("Postfix : \n %s\n\n", res);

	struct Node* root = make_tree(res);
	printf("Inorder:\n\n");
	inorder(root);
	printf("\n\nLevelOrder:\n\n");
	levelorder(root);
}