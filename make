.PHONY: all clean

all: main.exe

clean:
	del main.exe *.o *.a

main.exe: main.o libgraphlist.a
	$(GCC) $(CFLAGS) $^ -o $@

%.o: %.c
	$(GCC) $(CFLAGS) -c $^ -o $@

libgraphlist.a: graphlist.o
	$(AR) -rsc $@ $^

CFLAGS= -O1

AR=ar
GCC=gcc
