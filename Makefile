EXE=translate
OBJS=main.o task1.o task2.o task3.o task4.o

$(EXE): $(OBJS)
	cc -O3 -Wall -o $(EXE) $(OBJS)

%.o: %.c tasks.h
	cc -O3 -Wall -c $<

format:
	clang-format -style=file -i *.c *.h

clean:
	rm -f $(EXE) *.o