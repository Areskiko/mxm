.PHONY: clean

debug: main.c mxm.c mxm.h
	gcc -g -o main main.c mxm.c

release: main.c mxm.c mxm.h
	gcc -o main main.c mxm.c -O3

clean:
	-@ rm main
