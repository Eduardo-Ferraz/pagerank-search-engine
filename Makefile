
trab2: main.o linked_list.o rb_tree.o page.o
	gcc -Wall -Wextra -g -o trab2 main.o linked_list.o rb_tree.o page.o



main.o: main.c page.h rb_tree.h linked_list.h
	gcc -Wall -Wextra -g -c main.c


linked_list.o: linked_list.c linked_list.h
	gcc -Wall -Wextra -g -c linked_list.c

rb_tree.o: rb_tree.c rb_tree.h linked_list.h
	gcc -Wall -Wextra -g -c rb_tree.c


page.o: page.c page.h linked_list.h
	gcc -Wall -Wextra -g -c page.c

# cleans object files and the executable
.PHONY: clean
clean:
	rm -f *.o trab2
