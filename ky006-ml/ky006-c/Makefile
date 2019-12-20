
LIB_SRC = ky006.c
LIB_OBJ = $(LIB_SRC:.c=.o)

SRC = $(LIB_SRC) main.c

LIB = libky006.a

all: a.out $(LIB)

a.out: $(SRC)
	gcc -lwiringPi $^ -o $@

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^
