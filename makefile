 CC = gcc
 FLAGS = -ansi -Wall -pedantic -g

compiler: ./compiler.o ./get_macros.o ./macro_routine.o ./file_line.o ./stream.o ./handle_error.o ./allocate_memory.o ./util.o ./make_am_file.o ./parser.o
	$(CC) -g compiler.o file_line.o stream.o get_macros.o macro_routine.o allocate_memory.o handle_error.o util.o ./make_am_file.o  ./parser.o $(FLAGS) -o $@

compiler.o:  ./src/compiler.c
	$(CC) -c src/compiler.c $(FLAGS) -o $@

get_macros.o:  ./src/get_macros.c ./include/get_macros.h
	$(CC) -c src/get_macros.c $(FLAGS) -o $@

macro_routine.o:  ./src/macro_routine.c ./include/macro_routine.h
	$(CC) -c src/macro_routine.c $(FLAGS) -o $@

file_line.o:  ./src/file_line.c ./include/file_line.h
	$(CC) -c ./src/file_line.c $(FLAGS) -o $@

stream.o: ./src/stream.c ./include/stream.h
	$(CC) -c ./src/stream.c $(FLAGS) -o $@

handle_error.o: ./src/handle_error.c ./include/handle_error.h
	$(CC) -c ./src/handle_error.c $(FLAGS) -o $@

allocate_memory.o: ./src/allocate_memory.c ./include/allocate_memory.h
	$(CC) -c ./src/allocate_memory.c $(FLAGS) -o $@

util.o: ./src/util.c ./include/util.h
	$(CC) -c ./src/util.c $(FLAGS) -o $@

make_am_file.o: ./src/make_am_file.c ./include/make_am_file.h
	$(CC) -c ./src/make_am_file.c $(FLAGS) -o $@

parser.o: ./src/parser.c ./include/parser.h
	$(CC) -c ./src/parser.c $(FLAGS) -o $@

clean:
	rm -rf core.* *.o *.am *.ob *.ent *.ext compiler
