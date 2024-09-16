.PHONY: clean

main: main.c mxm.c mxm.h
	gcc -g -o main main.c mxm.c

clean:
	-@ rm main