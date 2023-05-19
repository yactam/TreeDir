all : treedir

treedir : ./Sources/main.o ./Sources/commands.o ./Sources/liste_utils.o ./Sources/string_utils.o ./Sources/file_utils.o
	gcc -Wall -DNDEBUG -g -o treedir ./Sources/main.o ./Sources/commands.o ./Sources/liste_utils.o ./Sources/string_utils.o ./Sources/file_utils.o

./Sources/main.o : ./Sources/main.c
	gcc -Wall -DNDEBUG -g -o ./Sources/main.o -c ./Sources/main.c

./Sources/file_utils.o : ./Sources/file_utils.c
	gcc -Wall -DNDEBUG -g -o ./Sources/file_utils.o -c ./Sources/file_utils.c

./Sources/commands.o : ./Sources/commands.c
	gcc -Wall -DNDEBUG -g -o ./Sources/commands.o -c ./Sources/commands.c 

./Sources/liste_utils.o : ./Sources/liste_utils.c
	gcc -Wall -DNDEBUG -g -o ./Sources/liste_utils.o -c ./Sources/liste_utils.c 

./Sources/string_utils.o : ./Sources/string_utils.c
	gcc -Wall -DNDEBUG -g -o ./Sources/string_utils.o -c ./Sources/string_utils.c 

clean :
	rm ./Sources/*.o 2> log/makefile.log
	rm treedir 2> log/makefile.log
	rm valgrind* 2> log/makefile.log

valgrind:
	valgrind --leak-check=full \
         --log-file=log/valgrind-out.txt \
		 --track-origins=yes \
         ./treedir Tests/$(ARGS) 2> log/prog_log.log

run:
	./treedir Tests/$(ARGS) 2> log/prog_log.log
