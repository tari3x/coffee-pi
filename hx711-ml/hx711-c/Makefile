
LIB_SRC = precisionTiming.c hx711.c
LIB_OBJ = $(LIB_SRC:.c=.o)

SRC = $(LIB_SRC) main.c

LIB = libhx711.a

all: a.out $(LIB)

a.out: $(SRC)
	gcc -lwiringPi $^ -o $@

$(LIB): $(LIB_OBJ)
	ar rcs $@ $^
