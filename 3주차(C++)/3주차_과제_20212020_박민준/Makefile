cc = g++
target = main
objects = main.o str.o

$(target) : $(objects)
	$(cc) -o $(target) $(objects)

%.o : %.cpp
	$(cc) -c -o $@ $^

.PHONY : clean
clean : 
	rm $(target) $(objects)
