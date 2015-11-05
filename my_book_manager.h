struct book {
	long l_book_id;
	char *ptr_title;
	int i_year;
	float f_quality;
	long l_author_id;
	char *ptr_name;
	char *ptr_surname;
	struct book *next;
};

//Prototypes
void showMenu(void);
void insert(void);
struct book *new_book(void);
