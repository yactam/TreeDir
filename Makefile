all : treedir

treedir : ./Sources/main.o ./Sources/commands.o ./Sources/liste_utils.o ./Sources/string_utils.o ./Sources/file_utils.o
	gcc -Wall -g -o treedir ./Sources/main.o ./Sources/commands.o ./Sources/liste_utils.o ./Sources/string_utils.o ./Sources/file_utils.o

./Sources/main.o : ./Sources/main.c
	gcc -Wall -g -o ./Sources/main.o -c ./Sources/main.c

./Sources/file_utils.o : ./Sources/file_utils.c
	gcc -Wall -g -o ./Sources/file_utils.o -c ./Sources/file_utils.c

./Sources/commands.o : ./Sources/commands.c
	gcc -Wall -g -o ./Sources/commands.o -c ./Sources/commands.c 

./Sources/liste_utils.o : ./Sources/liste_utils.c
	gcc -Wall -g -o ./Sources/liste_utils.o -c ./Sources/liste_utils.c 

./Sources/string_utils.o : ./Sources/string_utils.c
	gcc -Wall -g -o ./Sources/string_utils.o -c ./Sources/string_utils.c 

clean :
	rm ./Sources/*.o
	rm treedir
