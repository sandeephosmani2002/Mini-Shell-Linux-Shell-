
TARGET = mini_shell
SRC = $(wildcard *.c)
OBJ = $(patsubst %.c,%.o,$(SRC))

$(TARGET): $(OBJ)
	gcc -o $@ $^

clean:
	rm -f *.o $(TARGET)
