#include<stdlib.h>
#include<string.h>

#include "book.h"

struct book *find_by_id(struct book *head, long id)
{
	while (head != NULL)
	{
		if (head->l_book_id == id)
		{
			return head;
		}
		head = head->next;
	}

	return NULL;
}

struct book *find_by_author_id(struct book *head, long id)
{
	while (head != NULL)
	{
		if (head->l_author_id == id)
		{
			return head;
		}
		head = head->next;
	}

	return NULL;
}

void free_list(struct book *head)
{
	struct book *temp;

	while (head != NULL)
	{
		temp = head;
		head = head->next;

		free_node(temp);
	}
}

struct book *remove_by_id(struct book *head, long id)
{
	struct book *temp = head, *prev = head;

	while (head != NULL)
	{
		prev = temp;
		temp = head;
		head = head->next;

		if (temp->l_book_id == id)
		{
			free_node(temp);

			prev->next = head;
		}
	}

	return head;
}

void free_node(struct book *node)
{
	free(node->ptr_name);
	free(node->ptr_title);
	free(node->ptr_surname);
	free(node);
}


struct book *new_book()
{
	struct book *new = calloc(sizeof(struct book), 1);

	return new;
}

void append(struct book **head, struct book *to_append)
{
	struct book *tmp = *head;

	if (*head == NULL)
	{
		*head = to_append;
	}
	else
	{
		while (tmp->next != NULL)
		{
			tmp = tmp->next;
		}
		tmp->next = to_append;
	}
}
