CWD := $(shell pwd)
export INCLUDE := $(CWD)/include
export CC_FLAGS := -I$(INCLUDE) -fopenmp=libomp -flto -march=native
export CC := /opt/homebrew/opt/llvm/bin/clang
#export CC := /opt/homebrew/opt/gcc/bin/gcc-14
export DATA_DIR := $(CWD)/data
export WORK_DIR := $(CWD)/workdir
export SIZES := 128 256 512 1024 2048 4096 8192 16384 32768 65536 131072 262144

SUBDIRS := naive dynamic_naive tiling dynamic_tiling lto_tiling perfect_tiling matrix_gen
IMPLEMENTATIONS := $(wordlist 3,6, $(SUBDIRS))

all: $(SUBDIRS)

$(SUBDIRS):
	@ echo "Making $@"
	@ $(MAKE) -C $@

$(DATA_DIR)/%.dat:
	@ $(MAKE) $@ -C matrix_gen

$(DATA_DIR)/%.ident:
	@ $(MAKE) $@ -C matrix_gen

%.txt: %
	@ echo "Generating text file $@"
	@ hexdump -e '/1 "%u\n"' -v $* > $*.txt

clean: $(SUBDIRS:%=clean_%)
	@ echo "Cleaning data files"
	@- rm **/*.out **/*.dat **/*.ident **/*.txt 2> /dev/null || true

clean_%:
	@ echo "Cleaning $*"
	@ $(MAKE) clean -C $*

benchmark: $(IMPLEMENTATIONS:%=bench_%)

bench_%:
	@ echo "Benchmarking $*"
	@ $(MAKE) benchmark -C $*

verify: $(IMPLEMENTATIONS:%=verify_%)

verify_%:
	@ echo "Verifying $*"
	@ $(MAKE) verify -C $*

extensive.csv:
	echo "Implementation$(SIZES:%=,%)" > $@

extensive: clean extensive.csv $(IMPLEMENTATIONS) $(SIZES:%=$(DATA_DIR)/%.dat)
	i=0 ; while [[ $$i -le 60 ]]; do \
		for impl in $(IMPLEMENTATIONS); do \
			$(MAKE) bench_$$impl; \
			cat $$impl/result.txt | \
			grep -oP "(\d+:?)+(\d+\.\d+)?elapsed" | \
			sed "s/elapsed//" | \
			tr "\n" "," | \
			sed "s/$$/\n/" | \
			sed "s/,$$//" | \
			sed "s/^/$$impl,/" >> extensive.csv ; \
		done; \
        ((i=i+1)) ; \
	done;


.PHONY: all extensive extensive.csv clean clean_% $(SUBDIRS)
.SECONDARY: $(SIZES:%=$(DATA_DIR)/%.dat) $(SIZES:%=$(DATA_DIR)/%.ident)
