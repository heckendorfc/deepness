debug:
	clang -Wall -Wno-unused-function -o deepness source/*.c
release:
	cc -Wall -o deepness source/*.c
