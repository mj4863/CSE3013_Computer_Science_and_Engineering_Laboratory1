cc=gcc
cflags = -W -g
target = main
objects = main.o Count.o PS.o

$(target) : $(objects)
	$(cc) $(cflags) -o $(target) $(objects)

%.o : %.c
	$(cc) $(cflags) -c -o $@ $<

$(objects) : Header.h

.PHONY : clean
clean :
	rm $(target) $(objects)