# Compiler
CC = gcc

# -Wall and -Wextra: for warnings
# -g: for debug
CFLAGS = -Wall -Wextra -g

# libs (e.g., -lm)
LIBS = 
# name of the final executable file
TARGET = trab3

# generate the executable file
$(TARGET): main.o str.o
	$(CC) $(CFLAGS) -o $(TARGET) main.o str.o $(LIBS)

# generate object files
main.o: main.c str.h
	$(CC) $(CFLAGS) -c main.c

str.o: str.c str.h 
	$(CC) $(CFLAGS) -c str.c

# cleans object files and the executable
.PHONY: clean
clean:
	rm -f *.o $(TARGET)