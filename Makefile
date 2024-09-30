.PHONY: clean debug release data benchmark

sizes := 2 4 8 16 32 64 128 256 512 1024 2048 4096 8192 16384 32768

benchmark: release
	echo "" > result.txt
	find ../data -type f | xargs -n 1 -I {} time -ao result.txt ./main {} {} out

release: main.c mxm.c mxm.h
	$(CC) -o main main.c mxm.c -O3

debug: main.c mxm.c mxm.h
	$(CC) -g -o main main.c mxm.c

matrix_gen: matrix_gen.c
	$(CC) -o matrix_gen matrix_gen.c -O3

$(sizes:%=\%/%.dat)&: matrix_gen
	./matrix_gen $*

clean:
	-@ rm -r main mg *.dSYM 2> /dev/null || true
