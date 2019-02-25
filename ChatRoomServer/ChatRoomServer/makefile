all:
	g++ -c main.cpp -lpthread -o main.o
	g++ -c User.cpp -o User.o
	g++ -c SimpleDataBase.cpp -liostream -lfstream -lstdlib -lstring -o SimpleDataBase.o
	g++ main.o SimpleDataBase.o User.o -lpthread -o server
