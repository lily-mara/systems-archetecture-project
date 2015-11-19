file(GLOB SOURCES *.c)

add_executable(my_book_manager ${SOURCES})
add_library(books io.c book.c)
