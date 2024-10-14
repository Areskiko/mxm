CWD := $(shell pwd)
export INCLUDE := $(CWD)/include
export CC_FLAGS := -I$(INCLUDE) -fopenmp=libomp
export CC := /opt/homebrew/opt/llvm/bin/clang
export DATA_DIR := $(CWD)/data
export WORK_DIR := $(CWD)/workdir
export SIZES := 128 256 512 1024 2048 4096 8192 16384 32768 65536

SUBDIRS := naive dynamic_naive tiling dynamic_tiling matrix_gen
IMPLEMENTATIONS := $(wordlist 3,4, $(SUBDIRS))

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


.PHONY: all $(SUBDIRS)
.SECONDARY: $(SIZES:%=$(DATA_DIR)/%.dat) $(SIZES:%=$(DATA_DIR)/%.ident)
