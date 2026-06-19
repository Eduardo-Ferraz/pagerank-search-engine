# Compiler
CC = gcc

# -Wall and -Wextra: for warnings
# -g: for debug
CFLAGS = -Wall -Wextra -g

# libs (e.g., -lm)
LIBS = 
# name of the final executable file
TARGET = trab2

# generate the executable file
$(TARGET): main.o str.o linked_list.o rb_tree.o page.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o str.o linked_list.o rb_tree.o page.o $(LIBS)

# generate object files
main.o: main.c str.h
	$(CC) $(CFLAGS) -c main.c

str.o: str.c str.h 
	$(CC) $(CFLAGS) -c str.c

linked_list.o: linked_list.c linked_list.h 
	$(CC) $(CFLAGS) -c linked_list.c

rb_tree.o: rb_tree.c rb_tree.h 
	$(CC) $(CFLAGS) -c rb_tree.c

page.o: page.c page.h 
	$(CC) $(CFLAGS) -c page.c

# cleans object files and the executable
.PHONY: clean
clean:
	rm -f *.o $(TARGET)