trab2: main.o linked_list.o rb_tree.o page.o indexer.o graph.o pagerank.o
	gcc -Wall -Wextra -g -o trab2 main.o linked_list.o rb_tree.o page.o indexer.o graph.o pagerank.o -lm

main.o: main.c page.h rb_tree.h linked_list.h
	gcc -Wall -Wextra -g -c main.c

linked_list.o: linked_list.c linked_list.h
	gcc -Wall -Wextra -g -c linked_list.c

rb_tree.o: rb_tree.c rb_tree.h linked_list.h
	gcc -Wall -Wextra -g -c rb_tree.c

page.o: page.c page.h linked_list.h
	gcc -Wall -Wextra -g -c page.c

indexer.o: indexer.c indexer.h
	gcc -Wall -Wextra -g -c indexer.c

graph.o: graph.c graph.h
	gcc -Wall -Wextra -g -c graph.c

pagerank.o: pagerank.c pagerank.h
	gcc -Wall -Wextra -g -c pagerank.c

# cleans object files and the executable
.PHONY: clean
clean:
	rm -f *.o trab2
