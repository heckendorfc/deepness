debug:
	clang -Wall -Wno-unused-function -o deepness source/*.c
release:
	cc -Wall -DRELEASE_MODE -o deepness source/*.c
