cc = g++
target = test
objects = array.o rangearray.o main.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

.PHONY : clean
clean :
	rm $(target) $(objects)
