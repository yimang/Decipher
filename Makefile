decipher.out : decipher.o
	g++ -ansi -Wall -g -o decipher.out decipher.o

decipher.o : decipher.cpp
	g++ -ansi -Wall -g -c decipher.cpp
	
clean :
	rm -f FileFind.out decipher.o