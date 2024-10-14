all: main maind

main: main.c mxm.c
	@ echo "Compiling dynamic naive"
	@ $(CC) $(CC_FLAGS) -o main main.c -O3

maind: main.c mxm.c
	@ echo "Compiling dynamic naive in debug mode"
	@ $(CC) $(CC_FLAGS) -o maind main.c -g

$(DATA_DIR)/%:
	@ $(MAKE) $@ -C ..

$(WORK_DIR)/%.dynamic_naive.out: main $(DATA_DIR)/%.dat $(DATA_DIR)/%.ident
	@ ./main $(DATA_DIR)/$*.ident $(DATA_DIR)/$*.dat $(WORK_DIR)/$*.dynamic_naive.out

verify: $(SIZES:%=%.diff)

%.diff: $(WORK_DIR)/%.dynamic_naive.out $(DATA_DIR)/%.dat
	@ diff -u $(WORK_DIR)/$*.dynamic_naive.out $(DATA_DIR)/$*.dat

benchmark: main result $(SIZES:%=bench%)

bench%: main $(DATA_DIR)/%.dat
	@ echo "Benchmarking dynamic naive $*"
	@ time -ao result.txt ./main $(word 2,$+) $(word 2,$+) $(WORK_DIR)/dynamic_naive.out

result:
	@ echo "Wiping last result"
	@ echo "" > result.txt

clean:
	-@ rm -r main maind *.dSYM 2> /dev/null || true

.PHONY: all clean verify %.diff benchmark bench% result
.SECONDARY: main maind $(SIZES:%=$(DATA_DIR)/%.dat) $(SIZES:%=$(DATA_DIR)/%.ident) $(SIZES:%=$(WORK_DIR)/%.dynamic_naive.out)
