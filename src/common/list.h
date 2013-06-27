#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

struct _list_item {
	char * value;
	struct _list_item * next_item;
};

typedef struct _list_item ListItem;

struct _list {
	ListItem * items;
	int item_count;
	char * (*get)(struct _list *, int);
	void (*set)(struct _list *, int, char *);
	void (*push)(struct _list *, char *);
	int (*length)(struct _list *);
};

typedef struct _list List;

List * _list_init();
char * _list_get(List *, int);
void _list_set(List *, int, char *);
void _list_push(List *, char *);
int _list_length(List *);

#endif
