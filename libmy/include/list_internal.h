#pragma once

typedef struct s_list_node
{
	void*			data;
	t_list_node*	next;
} t_list_node;

typedef struct s_list
{
	int				size;
	t_list_node*	head;
	t_list_node*	tail;
	int				(*compare)(void*, void*);
	void			(*destroy)(void*);
} t_list;