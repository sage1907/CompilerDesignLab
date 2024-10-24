.DEFAULT_GOAL := run

hello:
	@echo "Assignment 5 : Sagar Basak (2021CSB008)"

generate: hello
	@echo "Generating Required Files"
	flex test.l
	bison test.y -d
	gcc y.tab.c lex.yy.c symTable.c syntaxTree.c -ll
	@echo "Now do 'make run' to run"

info:
	@echo "Open test.output"
	bison test.y -v

run: generate
	@echo "Running program"
	./a.out < file.c

clean: hello
	@echo "Cleaning up..."
	rm lex.yy.c
	rm test.tab.c
	rm test.tab.h
	rm ./a.out
	rm test.output