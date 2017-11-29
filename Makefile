all:
	gcc -o prac2 prac2.c -pthread -Wall -Werror
	
run:
	./prac2 7 3 4 14
	
clean:
	rm -f prac2
	 
