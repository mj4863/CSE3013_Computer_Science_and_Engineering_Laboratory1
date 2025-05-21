CC = gcc
target = animal_exe
objects = main.o dog.o blackcow.o turtle.o

$(target) : $(objects)
	$(CC) -o $(target) $(objects)

.PHONY : clean
clean:
	rm $(target) $(objects)
