#include <stdlib.h>
#include "list.h"
#include "list_internal.h"

t_list * list_create(void)
{
	t_list*	list = malloc(sizeof(t_list));

	if (list == NULL)
		return NULL;

	list->size = 0;
	list->head = NULL;
	list->tail = NULL;
	list->compare = NULL;
	list->destroy = NULL;

	return list;
}

void list_init(t_list * list, int (* compare)(void * , void *),
	void (* destroy)(void *))
{
	if (list == NULL)
		return;

	list->compare = compare;
	list->destroy = destroy;
}

t_list_node* list_head(t_list const* list)
{
	return list != NULL ? list->head : NULL;
}

t_list_node* list_tail(t_list const* list)
{
	return list != NULL ? list->tail : NULL;
}

bool list_node_is_head(t_list const* list, t_list_node const * node)
{
	return list ? list->head == node : false;
}

bool list_node_is_tail(t_list const* list, t_list_node const * node)
{
	return list ? list->tail == node && node->next == NULL : false;
}

t_list_node * list_node_next(t_list_node const * node)
{
	return node == NULL ? NULL : node->next;
}

void * list_node_data(t_list_node const * node)
{
	return node == NULL ? NULL : node->data;
}

unsigned int list_size(t_list const * list)
{
	return list ? list->size : 0;
}

void list_insert_next(t_list * list, t_list_node * node, void * data)
{
	if (list == NULL)
		return;

	t_list_node*	newNode = malloc(sizeof(t_list_node));

	if (newNode == NULL)
		return;

	newNode->data = data;
	newNode->next = NULL;

	if (node == NULL)
	{
		if (list->head != NULL)
			newNode->next = list->head;
		else
			list->tail = newNode;

		list->head = newNode;
	}
	else
	{
		if (node->next != NULL)
			newNode->next = node->next;
		else
			list->tail = newNode;

		node->next = newNode;
	}
	
	list->size++;
}

void list_remove_next(t_list * list, t_list_node * node, void ** data)
{
	if (list == NULL || node == NULL || node->next == NULL)
	{
		if (data != NULL)
			*data = NULL;

		return;
	}

	t_list_node*	newNext = node->next->next;

	if (data != NULL)
		*data = node->next->data;
	else if (list->destroy != NULL)
		list->destroy(node->next->data);

	free(node->next);

	node->next = newNext;

	if (newNext == NULL)
		list->tail = node;

	list->size--;
}

t_list_node * list_find_if(t_list const * list, bool (*predicate)(void*))
{
	if (list == NULL || list->size == 0 || predicate == NULL)
		return NULL;

	t_list_node*	node = list->head;

	for (int i = 0; i < list->size; i++)
	{
		if (predicate(node->data))
			return node;

		node = node->next;
	}

	return NULL;
}

void list_foreach(t_list const * list, void (*action)(void*))
{
	if (list == NULL || action == NULL)
		return;

	t_list_node* node = list->head;

	while (node != NULL)
	{
		action(node->data);
		node = node->next;
	}
}

void list_sort(t_list * list)
{
	if (list == NULL || list->size == 0 || list->compare == NULL)
		return;

	int n = list->size;
	int newN = 0;

	void*	tmp = NULL;

	t_list_node*	prevNode;
	t_list_node*	node;

	do
	{
		newN = 0;
		prevNode = list->head;
		node = prevNode->next;

		for (int i = 1; i < n; i++)
		{
			if (list->compare(prevNode->data, node->data) > 0)
			{
				tmp = node->data;
				node->data = prevNode->data;
				prevNode->data = tmp;

				newN = i;
			}

			prevNode = node;
			node = prevNode->next;
		}

		n = newN;
	} while (n > 1);
}

void list_reverse(t_list * list)
{
	if (list == NULL || list->size == 0)
		return;

	t_list_node*	prevNode = NULL;
	t_list_node*	node = list->head;
	t_list_node*	nextNode = NULL;

	for(int i = 0; i < list->size; i++)
	{
		nextNode = node->next;

		node->next = prevNode;

		prevNode = node;
		node = nextNode;
	}

	list->head = prevNode;
}

t_list * list_merge(t_list * list1, t_list * list2)
{
	if (list1 == NULL || list2 == NULL || list1 == list2)
		return NULL;

	t_list*	list = list_create();

	list_init(list, list1->compare, list1->destroy);
	
	t_list_node* node = list1->head;

	while (node != NULL)
	{
		list_insert_next(list, list->tail, node->data);
		node = node == list1->tail ? list2->head : node->next;
	}

	return list;
}

void list_destroy(t_list ** list)
{
	if (list == NULL || *list == NULL)
		return;

	if ((*list)->head != NULL)
	{
		t_list_node* node = (*list)->head;
		t_list_node* nextNode = NULL;

		while(node != NULL)
		{
			nextNode = node->next;

			if ((*list)->destroy != NULL)
				(*list)->destroy(node->data);

			free(node);

			node = nextNode;
		}
	}

	free((*list));
	*list = NULL;
}