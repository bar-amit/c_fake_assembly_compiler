 CC = gcc
 FLAGS = -ansi -Wall -pedantic -g

compiler: compiler.o get_macros.o macro_routine.o file_line.o stream.o handle_error.o allocate_memory.o util.o make_am_file.o parser.o label_table.o
	$(CC) -g build/compiler.o build/file_line.o build/stream.o build/get_macros.o build/macro_routine.o build/allocate_memory.o build/handle_error.o build/util.o build/make_am_file.o build/parser.o build/label_table.o $(FLAGS) -o $@

compiler.o: build src/compiler.c
	$(CC) -c src/compiler.c $(FLAGS) -o build/$@

get_macros.o: build src/get_macros.c include/get_macros.h
	$(CC) -c src/get_macros.c $(FLAGS) -o build/$@

macro_routine.o: build src/macro_routine.c include/macro_routine.h
	$(CC) -c src/macro_routine.c $(FLAGS) -o build/$@

file_line.o: build src/file_line.c include/file_line.h
	$(CC) -c src/file_line.c $(FLAGS) -o build/$@

stream.o: build src/stream.c include/stream.h
	$(CC) -c src/stream.c $(FLAGS) -o build/$@

handle_error.o: build src/handle_error.c include/handle_error.h
	$(CC) -c src/handle_error.c $(FLAGS) -o build/$@

allocate_memory.o: build src/allocate_memory.c include/allocate_memory.h
	$(CC) -c src/allocate_memory.c $(FLAGS) -o build/$@

util.o: build src/util.c include/util.h
	$(CC) -c src/util.c $(FLAGS) -o build/$@

make_am_file.o: build src/make_am_file.c include/make_am_file.h
	$(CC) -c src/make_am_file.c $(FLAGS) -o build/$@

parser.o: build  src/parser.c include/parser.h
	$(CC) -c src/parser.c $(FLAGS) -o build/$@

label_table.o: build src/label_table.c include/label_table.h
	$(CC) -c src/label_table.c $(FLAGS) -o build/$@

build:
	mkdir build

clean:
	rm -rf core.* *.o *.am *.ob *.ent *.ext compiler
