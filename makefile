compile:
	-gcc -Wall -pedantic-errors conchita.c read.c -o shell
check:
	@echo "###########################################################"
	-gcc -Wall -pedantic-errors conchita.c read.c -o shell
	@echo "###########################################################"
	-./shell
	@echo "###########################################################"
	-leaks -atExit -- ./shell | grep LEAK:
	@echo "###########################################################"
