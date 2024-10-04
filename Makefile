sizes := 2 4 8 16 32 64 128 256 512 1024 2048 4096 # 8192 16384 32768
data_dir := data

main: main.c mxm.c mxm.h sizes.h
	$(CC) -o main main.c mxm.c -O3

maind: main.c mxm.c mxm.h sizes.h
	$(CC) -g -o maind main.c mxm.c

matrix_gen: matrix_gen.c sizes.h
	$(CC) -o matrix_gen matrix_gen.c -O3

$(data_dir)/%.dat: matrix_gen
	./matrix_gen $(data_dir)/$*.dat $*

$(data_dir)/%.ident: matrix_gen
	./matrix_gen $(data_dir)/$*.ident $* ident

verify: $(sizes:%=%.diff)

$(data_dir)/%.out: main $(data_dir)/%.dat $(data_dir)/%.ident
	./main $(data_dir)/$*.ident $(data_dir)/$*.dat $(data_dir)/$*.out

$(data_dir)/%.txt: $(data_dir)/%
	hexdump -e '/8 "%u\n"' -v $(data_dir)/$* > $(data_dir)/$*.txt

%.diff: $(data_dir)/%.out.txt $(data_dir)/%.dat.txt
	diff -u $(data_dir)/$*.out.txt $(data_dir)/$*.dat.txt > $*.diff
	rm $*.diff

clean:
	-@ rm -r *.diff *.dSYM 2> /dev/null || true

purge: clean
	-@ rm main matrix_gen $(data_dir)/* 2> /dev/null || true

benchmark: main result $(sizes:%=bench%)

bench%: main $(data_dir)/%.dat
	time -ao result.txt ./main $(word 2,$+) $(word 2,$+) out

result:
	echo "" > result.txt

.PHONY: clean benchmark $(sizes:%=verify%) purge result
.SECONDARY: $(data_dir)/$(sizes:%=%.dat) $(data_dir)/$(sizes:%=%.out) $(data_dir)/$(sizes:%=%.ident)
