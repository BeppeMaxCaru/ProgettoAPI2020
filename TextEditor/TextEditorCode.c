#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

struct StringList {
	char **str;
	int size;
};

struct Node {
	struct StringList list;
	struct Node *prev;
	struct Node *next;
	int position;
};

struct Tape {
	struct Node *head;
	struct Node *tail;
	int size;
};

int position = 0;
struct Node *current;

void insert(struct Tape*, struct StringList);
void freeTape(struct Tape);
void freeList(struct StringList);
void freeFromCurrent(struct Tape*);
void correctPosition(struct Tape*);

int main() {
	struct Tape tape;
	struct StringList list;
	int countUndo = 0, countPrint = 0, max = 0;
	int n, n1, n2, n3, np;
	int i;
	char s[1030];
	char *p;

	while (fgets(s, 1030, stdin)) {
		n = strlen(s) - 2;
		if (n == 0) {
			break;
		}
		if (s[n] == 'c') {
			n1 = atoi(s);
			p = strchr(s, ',');
			n2 = atoi(p + 1);
			if (n2 > max) {
				max = n2;
			}
			while (fgets(s, 1030, stdin)) {
				if (strlen(s) == 2 && s[0] == '.') {
					break;
				}
			}
		}
		else if (s[n] == 'u') {
			countUndo++;
		}
		else if (s[n] == 'p') {
			countPrint++;
		}
	}
	rewind(stdin);

	np = countPrint;
	if (np == 0)
		return 0;

	list.str = calloc(max, sizeof(char*));
	list.size = 0;

	if (countUndo == 0) {
		countPrint = 0;
		while (fgets(s, 1030, stdin)) {
			n = strlen(s) - 2;
			if (s[n] == 'c') {
				n1 = atoi(s) - 1;
				p = strchr(s, ',');
				n2 = atoi(p + 1);

				while (fgets(s, 1030, stdin)) {
					n = strlen(s);
					if (s[0] == '.' && s[1] == '\n') {
						break;
					}
					if (list.str[n1]) {
						free(list.str[n1]);
					}
					list.str[n1] = malloc(n + 1);
					strcpy(list.str[n1], s);
					n1++;
				}
				if (n2 > list.size) {
					list.size = n2;
				}
			}
			else if (s[n] == 'p') {
				n1 = atoi(s);
				p = strchr(s, ',');
				n2 = atoi(p + 1);
				if (n2 == 0) {
					printf(".\n");
				}
				else {
					for (n1--; n1 < n2 && n1 < list.size; n1++) {
						printf("%s", list.str[n1]);
					}
					for ( ; n1 < n2; n1++) {
						printf(".\n");
					}
				}
				countPrint++;
				if (countPrint == np) {
					break;
				}
			}
			else if (s[n] == 'd') {
				n1 = atoi(s) - 1;
				p = strchr(s, ',');
				n2 = atoi(p + 1);
				if (n2 > list.size) {
					n2 = list.size;
				}
				if (n1 < list.size) {
					for (n3 = n2; n3 < list.size; n1++, n3++) {
						free(list.str[n1]);
						list.str[n1] = malloc(strlen(list.str[n3]) + 1);
						strcpy(list.str[n1], list.str[n3]);
					}
					list.size = n1;
				}
			}
		}
		freeList(list);
		return 0;
	}

	tape.head = tape.tail = 0;
	tape.size = 0;

	insert(&tape, list);
	current = tape.head;

	countPrint = 0;
	while (fgets(s, 1030, stdin)) {
		n = strlen(s) - 2;
		if (s[n] == 'c') {
			n1 = atoi(s) - 1;
			p = strchr(s, ',');
			n2 = atoi(p + 1);

			correctPosition(&tape);
			struct StringList list;

			list.size = current->list.size;
			list.str = calloc(max, sizeof(char*));

			if (list.size > 0) {
				for (i = 0; i < list.size; i++) {
					list.str[i] = malloc(strlen(current->list.str[i]) + 1);
					strcpy(list.str[i], current->list.str[i]);
				}
			}

			while (fgets(s, 1030, stdin)) {
				n = strlen(s);
				if (s[0] == '.' && s[1] == '\n') {
					break;
				}
				if (list.str[n1]) {
					free(list.str[n1]);
				}
				list.str[n1] = malloc(n + 1);
				strcpy(list.str[n1], s);
				n1++;
			}
			if (n2 > list.size) {
				list.size = n2;
			}
			insert(&tape, list);
			freeList(list);
		}
		else if (s[n] == 'p') {
			n1 = atoi(s);
			p = strchr(s, ',');
			n2 = atoi(p + 1);
			if (n2 == 0) {
				printf(".\n");
			}
			else {
				correctPosition(&tape);
				for (n1--; n1 < n2 && n1 < current->list.size; n1++) {
					printf("%s", current->list.str[n1]);
				}
				for ( ; n1 < n2; n1++) {
					printf(".\n");
				}
				countPrint++;
				if (countPrint == np) {
					break;
				}
			}
		}
		else if (s[n] == 'd') {
			n1 = atoi(s) - 1;
			p = strchr(s, ',');
			n2 = atoi(p + 1);

			correctPosition(&tape);
			struct StringList list;
			int originalSize;

			list.str = calloc(max, sizeof(char*));
			originalSize = list.size = current->list.size;
			for (i = 0; i < list.size; i++) {
				list.str[i] = calloc(strlen(current->list.str[i]) + 1, 1);
				strcpy(list.str[i], current->list.str[i]);
			}

			if (n2 > list.size) {
				n2 = list.size;
			}
			if (n1 < list.size) {
				for (n3 = n2; n3 < list.size; n1++, n3++) {
					free(list.str[n1]);
					list.str[n1] = malloc(strlen(list.str[n3]) + 1);
					strcpy(list.str[n1], list.str[n3]);
				}
				list.size = n1;
			}
			insert(&tape, list);

			list.size = originalSize;
			freeList(list);
		}
		else if (s[n] == 'u') {
			n = atoi(s);
			if (position - n < 0) {
				position = 0;
			}
			else {
				position -= n;
			}
		}
		else if (s[n] == 'r') {
			n = atoi(s);
			if (position + n >= tape.size) {
				position = tape.size - 1;
			}
			else {
				position += n;
			}
		}
	}

	freeTape(tape);
	freeList(list);

	return 0;
}

void insert(struct Tape *tape, struct StringList list) {
	struct Node *node;
	struct StringList newList;
	int i;

	if (list.size == 0) {
		newList.size = 0;
		newList.str = 0;
	}
	else {
		newList.size = list.size;
		newList.str = malloc(newList.size * sizeof(char*));
		for (i = 0; i < newList.size; i++) {
			newList.str[i] = malloc(strlen(list.str[i]) + 1);
			strcpy(newList.str[i], list.str[i]);
		}
	}

	node = malloc(sizeof(struct Node));
	node->list = newList;
	node->prev = node->next = 0;
	node->position = position;

	if (!tape->head) {
		tape->head = tape->tail = node;
		tape->head->position = position;
	}
	else {
		freeFromCurrent(tape);
		current->next = node;
		node->prev = current;
		tape->tail = node;
		position++;
		tape->tail->position = position;
	}
	tape->size = position + 1;
	current = tape->tail;
}

void correctPosition(struct Tape *tape) {
	if (current->position < position) {
		while (current->position != position) {
			 current = current->next;
		}
	}
	else if (current->position > position) {
		while (current->position != position) {
			 current = current->prev;
		}
	}
}

void freeFromCurrent(struct Tape *tape) {
	struct Node *curr;
	struct Node *t;

	curr = current->next;
	while (curr) {
		freeList(curr->list);
		t = curr;
		curr = curr->next;
		free(t);
	}
}

void freeTape(struct Tape tape) {
	struct Node *curr;
	struct Node *t;

	curr = tape.head;
	while (curr) {
		freeList(curr->list);
		t = curr;
		curr = curr->next;
		free(t);
	}
}

void freeList(struct StringList list) {
	int i;

	for (i = 0; i < list.size; i++) {
		free(list.str[i]);
	}
	free(list.str);
}
