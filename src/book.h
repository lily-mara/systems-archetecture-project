/**CHeaderFile*****************************************************************

  FileName    book.h

  Synopsis    public definitions for book and book list logic

  SeeAlso     book.c

  Copyright   [Copyright (c) 2015 Carlos III University of Madrid
  All rights reserved.

  Permission is hereby granted, without written agreement and without license
  or royalty fees, to use, copy, modify, and distribute this software and its
  documentation for any purpose, provided that the above copyright notice and
  the following two paragraphs appear in all copies of this software.

  IN NO EVENT SHALL THE CARLOS III UNIVERSITY OF MADRID BE LIABLE TO ANY PARTY
  FOR DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING
  OUT OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF CARLOS III
  UNIVERSITY OF MADRID HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  CARLOS III UNIVERSITY OF MADRID SPECIFICALLY DISCLAIMS ANY WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
  FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS ON AN
  "AS IS" BASIS, AND CARLOS III UNIVERSITY OF MADRID HAS NO OBLIGATION TO
  PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS.]

******************************************************************************/
#ifndef _BOOK_H_
#define _BOOK_H_

#include <stdlib.h>

/*---------------------------------------------------------------------------*/
/* Structure declarations                                                    */
/*---------------------------------------------------------------------------*/
/**Struct**********************************************************************

  Synopsis    One book record.

******************************************************************************/
struct book {
	long l_book_id;
	char *ptr_title;
	int i_year;
	int i_numb_pages;
	float f_quality;
	long l_author_id;
	char *ptr_name;
	char *ptr_surname;
};

/**Struct**********************************************************************

  Synopsis    One node of a book element in a singly-linked list.

******************************************************************************/
struct book_node
{
	struct book *book;
	struct book_node *next;
};

/**Struct**********************************************************************

  Synopsis    One node of a pairing of books in a unique set of books.

******************************************************************************/
struct book_pair
{
	struct book *x;
	struct book *y;
	struct book_pair *next;
};

/*---------------------------------------------------------------------------*/
/* Function prototypes                                                       */
/*---------------------------------------------------------------------------*/
void free_list(struct book_node *);
void free_list_node(struct book_node *);
void free_book(struct book *);
struct book_node *remove_first_with_id(struct book_node *, long);
struct book *find_by_id(struct book_node *head_node, long id);
struct book *new_book();
struct book_node *append(struct book_node *, struct book *);
void display_corrupt_records(struct book_node *);
void print_books(struct book_node *);
void print_book(struct book *);
size_t book_list_length(struct book_node *);
struct book_node *remove_book(struct book_node *, struct book *);
struct book_node *duplicate_ids(struct book_node *head);
struct book_pair *corrupt_authors(struct book_node *head);
void print_corrupt_authors(struct book_node *head);
void free_pairs(struct book_pair *head);
struct book_pair *add_pair(struct book_pair *head, struct book *x, struct book *y);

#endif /* _BOOK_H_ */
