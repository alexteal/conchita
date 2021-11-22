all:
	-gcc -Wall -pedantic-errors head.c -o head	
	-gcc -Wall -pedantic-errors env.c -o env
	-gcc -Wall -pedantic-errors wc.c -o wc
	-gcc -Wall -pedantic-errors tail.c -o tail
gambit4:
	gcc -Wall -pedantic-errors tail.c -o tail
	@echo "TEST 1"
	@echo "#######################"
	-./tail -c 50 file file2 file3
	@echo "#######################"
	@echo "TEST 2"
	-./tail -n 4 file file2 file3
	@echo "#######################"
	@echo "TEST 3"	
	-./tail -c 4 -n 5 file file2 file3
	@echo "#######################"

#compilemain: 
#	gcc -Wall -pedantic-errors head.c -o head
gambit3:
	gcc -Wall -pedantic-errors wc.c -o wc
	@echo "TEST 1"
	@echo "#######################"
	-./wc -c file file2 file3
	@echo "#######################"
	@echo "TEST 2"
	-./wc -l file file2 file3
	@echo "#######################"
	@echo "TEST 3"	
	-./wc -w file file2 file3
	@echo "#######################"
	@echo "TEST 4"	
	-./wc file file2 file3 
	@echo "#######################"
gambit:
	gcc -Wall -pedantic-errors head.c -o head
	@echo "TEST 1"
	@echo "#######################"
	-./head -c 50 file file2 file3
	@echo "#######################"
	@echo "TEST 2"
	-./head -n 5 file file2 file3
	@echo "#######################"
	@echo "TEST 3"	
	-./head -c 4 -n 5 file
	@echo "#######################"
	@echo "TEST 4"	
	-./head -c 4 
	@echo "#######################"
	@echo "TEST 5"	
	-./head -n 4 
gambit2:
	gcc -Wall -pedantic-errors tail.c -o tail
	@echo "TEST 1"
	@echo "#######################"
	-./tail -c 50 file file2 file3
	@echo "#######################"
	@echo "TEST 2"
	-./tail -n 4 file file2 file3
	@echo "#######################"
	@echo "TEST 3"	
	-./tail -c 4 -n 5 file file2 file3
	@echo "#######################"
	@echo "TEST 4"	
	-./tail -c 4 
	@echo "#######################"
	@echo "TEST 5"	
	-./tail -n 4 

compile: 
	gcc -Wall -pedantic-errors head.c -o head
run: compile 
	./a.out
clean:
	rm head
