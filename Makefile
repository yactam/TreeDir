all : treedir

treedir : ./Sources/main.o ./Sources/commands.o ./Sources/liste_utils.o ./Sources/string_utils.o
	gcc -o treedir ./Sources/main.o ./Sources/commands.o ./Sources/liste_utils.o ./Sources/string_utils.o 

./Sources/main.o : ./Sources/main.c
	gcc -Wall -o ./Sources/main.o -c ./Sources/main.c

./Sources/commands.o : ./Sources/commands.c
	gcc -Wall -o ./Sources/commands.o -c ./Sources/commands.c 

./Sources/liste_utils.o : ./Sources/liste_utils.c
	gcc -Wall -o ./Sources/liste_utils.o -c ./Sources/liste_utils.c 

./Sources/string_utils.o : ./Sources/string_utils.c
	gcc -Wall -o ./Sources/string_utils.o -c ./Sources/string_utils.c 

clean :
	rm ./Sources/*.o
	rm treedir
