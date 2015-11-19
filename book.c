#include<stdlib.h>
#include<string.h>

#include "book.h"

void free_node(struct book *);

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

struct book *remove_by_id(struct book *head, int id)
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
