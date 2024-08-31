 CC = gcc
 FLAGS = -ansi -Wall -pedantic -g

assembler: main.o get_macros.o macro_routine.o file_line.o stream.o handle_error.o util.o make_am_file.o parser.o data_table.o entry_table.o make_ob_file.o make_extra_files.o
	$(CC) -g build/main.o build/file_line.o build/stream.o build/get_macros.o build/macro_routine.o build/handle_error.o build/util.o build/make_am_file.o build/parser.o build/data_table.o build/entry_table.o build/make_ob_file.o build/make_extra_files.o $(FLAGS) -o $@

main.o: build src/main.c
	$(CC) -c src/main.c $(FLAGS) -o build/$@

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

util.o: build src/util.c include/util.h
	$(CC) -c src/util.c $(FLAGS) -o build/$@

make_am_file.o: build src/make_am_file.c include/make_am_file.h
	$(CC) -c src/make_am_file.c $(FLAGS) -o build/$@

parser.o: build  src/parser.c include/parser.h
	$(CC) -c src/parser.c $(FLAGS) -o build/$@

data_table.o: build src/data_table.c include/data_table.h
	$(CC) -c src/data_table.c $(FLAGS) -o build/$@

entry_table.o: build src/entry_table.c include/entry_table.h
	$(CC) -c src/entry_table.c $(FLAGS) -o build/$@

make_ob_file.o: build src/make_ob_file.c include/make_ob_file.h
	$(CC) -c src/make_ob_file.c $(FLAGS) -o build/$@

make_extra_files.o: build src/make_extra_files.c include/make_extra_files.h
	$(CC) -c src/make_extra_files.c $(FLAGS) -o build/$@

build:
	mkdir build

clean:
	rm -rf core.* *.o *.am *.ob *.ent *.ext assembler build
