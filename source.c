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


//-----operations&operands-----
int is_operand(char* ch) {
	if (strlen(ch) > 2) return 0;
	if (ch[0] >= 'A' && ch[0] <= 'Z') return 1;
	return 0;
}

int get_priority(char* operation) {
	if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) return 1;
	if (strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) return 2;
	if (strcmp(operation, "^") == 0) return 3;
	if (strcmp(operation, "ln") == 0 || strcmp(operation, "tg") == 0 || strcmp(operation, "sin") == 0 || strcmp(operation, "cos") == 0) return 4;
	if (strcmp(operation, "(-)") == 0 || strcmp(operation, "abs") == 0) return 4;
	if (strcmp(operation, "min") == 0) return 4;
	return -1;
}

int is_operation(char* operation) {
	if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) return 1;
	if (strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) return 2;
	if (strcmp(operation, "^") == 0) return 3;
	if (strcmp(operation, "ln") == 0 || strcmp(operation, "tg") == 0 || strcmp(operation, "sin") == 0 || strcmp(operation, "cos") == 0) return 4;
	if (strcmp(operation, "(-)") == 0 || strcmp(operation, "abs") == 0) return 4;
	if (strcmp(operation, "min") == 0) return 4;
	return -1;
}

int get_rang(char* operation) {
	if (strcmp(operation, "+") == 0 || strcmp(operation, "-") == 0) return 1;
	if (strcmp(operation, "*") == 0 || strcmp(operation, "/") == 0) return 1;
	if (strcmp(operation, "^") == 0) return 1;
	if (strcmp(operation, "ln") == 0) return 0;
	if (strcmp(operation, "(-)") == 0 || strcmp(operation, "tg") == 0) return 0;
	if (strcmp(operation, "sin") == 0 || strcmp(operation, "cos") == 0 || strcmp(operation, "abs")) return 0;
	if (strcmp(operation, "min") == 0) return 0;
	return 0;
}
//-----operations&operands end-----


//-----Tree and Nodes-----
struct Node {
	char sym[5];
	double val;
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
}

struct Node* ndstack_pop(struct NodeStack* stack)
{
	if (ndstack_is_empty(stack)) {
		return NULL;
	}
	return stack->array[stack->top--];
}

struct Node* ndstack_peek(struct NodeStack* stack) {
	if (ndstack_is_empty(stack)) {
		return NULL;
	}
	return stack->array[stack->top];
}

double calculate_value(struct Node* node);
double val(struct Node* node);

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
			if (get_rang(ptr) == 1 || strcmp(ptr, "min") == 0) {
				t2 = ndstack_pop(stack);
			}
			else t2 = NULL;

			if (t2) {
				p->right = t1;
				p->left = t2;
			}
			else {
				p->right = t2;
				p->left = t1;
			}


			ndstack_push(stack, p);
		}
		ptr = strtok(NULL, delim);
	}

	p = ndstack_pop(stack);
	free(stack);
	return p;
}

void inorder(struct Node* node) {
	if (node) {
		inorder(node->left);
		printf("%s ", node->sym);
		inorder(node->right);
	}
}

void pretty_print_subtree(struct Node* node, const char* prefix);

void pretty_print_tree(struct Node * node) {
	if (node == NULL) return;
	printf("%s\n", node->sym);
	pretty_print_subtree(node, "");
	printf("\n");
}

void pretty_print_subtree(struct Node* node, char* prefix) {
	if (node == NULL) return;

	int hasLeft = (node->left != NULL);
	int hasRight = (node->right != NULL);

	if (!hasLeft && !hasRight) return;

	printf("%s", prefix);
	printf("%s", (hasLeft && hasRight) ? "|---- " : "");
	printf("%s", (!hasLeft && hasRight) ? "|---- " : "");

	if (hasRight) {
		int printStrand = (hasLeft && hasRight && (node->right->right != NULL || node->right->left != NULL));
		char newPrefix[MAX_CAP];
		strcpy(newPrefix, prefix);
		strcat(newPrefix, (printStrand ? "|     " : "	 "));
		printf("%s\n", node->right->sym);
		pretty_print_subtree(node->right, strcat(newPrefix, "	"));
	}

	if (hasLeft) {
		printf("%s|---- %s\n", (hasRight ? prefix : ""), node->left->sym);
		char newPrefix[MAX_CAP * MAX_CAP];
		strcpy(newPrefix, prefix);
		pretty_print_subtree(node->left, strcat(newPrefix, "	"));
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
	free(queue);
}

void postorder(struct Node* node) {
	if (node == NULL) return;
	struct NodeStack* stack = create_node_stack(MAX_CAP);

	do {
		while (node) {
			if (node->right)
				ndstack_push(stack, node->right);
			ndstack_push(stack, node);
			node = node->left;
		}

		node = ndstack_pop(stack);
		if (node->right && ndstack_peek(stack) == node->right) {
			ndstack_pop(stack);
			ndstack_push(stack, node);
			node = node->right;
		}
		else {
			printf("%s ", node->sym);
			node = NULL;
		}

	} while (!ndstack_is_empty(stack));
	free(stack);
}

void preorder(struct Node* node) {
	if (node == NULL) return;
	struct NodeStack* stack = create_node_stack(MAX_CAP);
	struct CharStack* chstack = create_char_stack(MAX_CAP);

	do {
		while (node) {
			if (node->left)
				ndstack_push(stack, node->left);
			ndstack_push(stack, node);
			node = node->right;
		}

		node = ndstack_pop(stack);
		if (node->left && ndstack_peek(stack) == node->left) {
			ndstack_pop(stack);
			ndstack_push(stack, node);
			node = node->left;
		}
		else {
			chstack_push(chstack, node->sym);
			node = NULL;
		}

	} while (!ndstack_is_empty(stack));

	while (!chstack_is_empty(chstack)) {
		printf("%s ", chstack_pop(chstack));
	}
	free(stack);
	free(chstack);
}

double calculate_value(struct Node* node) {

	if (is_operand(node->sym)) return node->val;

	if (is_operation(node->sym)) {
		if (strcmp(node->sym, "+") == 0) {
			return val(node->left) + val (node->right);
		} 
		else if (strcmp(node->sym, "-") == 0) {
			return val(node->left) - val(node->right);
		}
		else if (strcmp(node->sym, "*") == 0) {
			return val(node->left) * val(node->right);
		}
		else if (strcmp(node->sym, "/") == 0) {
			return val(node->left) / val(node->right);
		}
		else if (strcmp(node->sym, "^") == 0) {
			return pow(val(node->left), val(node->right));
		}
		else if (strcmp(node->sym, "ln") == 0) {
			return log(val(node->left));
		}
		else if (strcmp(node->sym, "tg") == 0) {
			return tan(val(node->left));
		}
		else if (strcmp(node->sym, "(-)") == 0) {
			return -val(node->left);
		}
		else if (strcmp(node->sym, "sin") == 0) {
			return sin(val( node->left));
		}
		else if (strcmp(node->sym, "cos") == 0) {
			return cos(val(node->left));
		}
		else if (strcmp(node->sym, "abs") == 0) {
			if (val(node->left) < 0) return -val(node->left);
			return val(node->left);
		}
		else if (strcmp(node->sym, "min") == 0) {
			if (val(node->left) < val(node->right)) return val(node->left);
			else return val(node->right);
		}
	}
	printf("Invalid tree calculation!");
	exit(1);
}


void update_tree_values(struct Node* node, char* sym, double val) {
	struct NodeQueue* queue = create_node_queue(MAX_CAP);
	ndqueue_insert(queue, node);
	ndqueue_insert(queue, NULL);
	struct Node* p = NULL;

	while (!ndqueue_is_empty(queue)) {
		p = ndqueue_delete(queue);
		if (!p) {
			if (!ndqueue_is_empty(queue))
				ndqueue_insert(queue, NULL);
			continue;
		}
		if (strcmp(p->sym, sym) == 0) p->val = val;
		if (p->left) ndqueue_insert(queue, p->left);
		if (p->right) ndqueue_insert(queue, p->right);
	}
	free(queue);
}

void enter_operand_values(struct Node* root, char** chArr) {

	int i = 0;
	while (chArr[i]) {
		printf("Enter val of %s :", chArr[i]);
		double k;
		scanf("%lf", &k);
		update_tree_values(root, chArr[i], k);
		printf("\n");
		i++;
	}
	printf("\n\nTree node values updated: \n");
}
double val(struct Node* node) { return calculate_value(node); }

void delete_tree(struct Node* node) {
	if (node == NULL) return;
	struct NodeQueue* queue = create_node_queue(MAX_CAP);
	ndqueue_insert(queue, node);
	struct Node* p = NULL;

	while (!ndqueue_is_empty(queue)) {
		p = ndqueue_delete(queue);
		if (p->left) ndqueue_insert(queue, p->left);
		if (p->right) ndqueue_insert(queue, p->right);
		printf("%s deleted\n", p->sym);
		free (p);
	}
	node = NULL;
	free(queue);
}
//-----Tree and Nodes End-----


//-----formatting-----
char* expr_wrapper(char* expression, char** operands) {
	int i = -1; 
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
		if (strcmp(ptr, ",") == 0) {
			ptr = strtok(NULL, delim);
			continue;
		}
		if (is_operand(ptr)) {
			int present = 0;
			for (int k = 0; k < i; k++) {
				if (strcmp(operands[k], ptr) == 0) {
					present = 1;
					break;
				}
			}
			if (!present) {
				char* tmp = malloc(MAX_CAP * (sizeof(char)));
				strcpy(tmp, ptr);
				strcpy(operands[++i], tmp);
				free(tmp);
			} 

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

	operands[++i] = NULL;
	free(stack);
	return res;
}
//-----formatting end-----


void console_output_start() {
	printf("Welcome to Expression_Tree application!\n\n");
	printf("Using this you can evaluate mathematical expressions using binary tree!\n");
	printf("Uppercase letters are reserved for operands\nLowercase letters are reserved for functions\nSymbols are reserved for operations\n");
	printf("Additionally, unary - operator is coded with \'(-)\'\n");
	printf("\nAny valid combination of operators, functions and operands is allowed\n");
	printf("Any invalid combination of operators, functions and operands leads to program termination!\n\n");
	printf("Enter ENTER to continue!");
	char c;
	c = fgetc(stdin);
	while (c != '\n') c = fgetc(stdin);
	printf("\n");
}

void console_output() {
	printf("Available commands:\n\n");
	printf("1.	Create binary tree from expression\n");
	printf("2.	Print binary tree (pretty print)\n");
	printf("3.	Print binary tree (modified level order)\n");
	printf("4.	Print prefix and postfix notation of given expression\n");
	printf("5.	Enter operand values\n");
	printf("6.	Calculate expression\n");
	printf("7.	Delete existing tree (required for new expression)\n");
	printf("0.	Exit program\n");
	printf("\n\n");
	printf("Enter command: ");
}


int main() {

	console_output_start();

	struct Node* root = NULL;
	int menu;
	char res[MAX_CAP];
	char expression[MAX_CAP];
	char util[MAX_CAP];
	char** operands = NULL;
	int vals_initialized = 0;

	do {
		console_output();
		scanf("%d", &menu);
		printf("\n\n");

		switch (menu) {
		case 1:
			if (root) {
				printf("Tree already exists! Delete it first to enter new expression!\n\n");
				break;
			}
			
			printf("Enter mathematical expression(c/p not supported, expression must be typed):\n");
			char* util2 = fgets(util, MAX_CAP, stdin);
			char* str_test = fgets(expression, MAX_CAP, stdin);
			if (!str_test) {
				printf("Invalid expression! Terminating program...");
				exit(1);
			}
			expression[strcspn(expression, "\n")] = 0;
			printf("You entered:\n%s\n\n", expression);
			operands = (char**)malloc(MAX_CAP * sizeof(char*));
			for (int q = 0; q < MAX_CAP; q++) {
				operands[q] = (char*)malloc(MAX_CAP * sizeof(char));
			}

			strcpy(res, expr_wrapper(expression, operands));
			root = make_tree(res);
			printf("Tree has been made!\n\n");
			break; 

		case 2:
			if (!root) {
				printf("Tree is not initialized!\n\n");
				break;
			}
			printf("Pretty print:\n\n");
			pretty_print_tree(root);
			printf("\n\n");
			break;

		case 3:
			if (!root) {
				printf("Tree is not initialized!\n\n");
				break;
			}
			printf("Modified level order:\n\n");
			levelorder(root);
			printf("\n\n");
			break;

		case 4:
			if (!root) {
				printf("Tree is not initialized!\n\n");
				break;
			}
			printf("Postfix notation:\n");
			postorder(root);
			printf("\nPrefix notation:\n");
			preorder(root);
			printf("\n\n");
			break;

		case 5:
			if (!root) {
				printf("Tree is not initialized!\n\n");
				break;
			}
			enter_operand_values(root, operands);
			printf("Operand values sucessfuly initialized! \n\n");
			vals_initialized = 1;
			break;

		case 6:
			if (!root) {
				printf("Tree is not initialized!\n\n");
				break;
			}
			if (!vals_initialized) {
				printf("Operand values not initialized!!\n\n");
				break;
			}
			printf("Calculating value...\n");
			printf("Final result: %lf\n\n", calculate_value(root));
			vals_initialized = 0;
			break;

		case 7:
			if (!root) {
				printf("Tree is not initialized!\n\n");
				break;
			}
			printf("Deleting tree...\n");
			delete_tree(root);
			root = NULL;
			vals_initialized = 0;
			printf("Tree sucessfuly deleted!\n\n");
			break;
		
		case 0:
			printf("\n\nTerminating program... Bye Bye ;)\n\n\n");
			exit(0);

		default:
			printf("Invalid command!\n\n");
			break;
		}

	} while (menu != 0);
}