all: main maind

main: main.c mxm.c $(INCLUDE)/sizes.h
	@ echo "Compiling naive"
	@ $(CC) $(CC_FLAGS) -o main main.c mxm.c -O3

maind: main.c mxm.c $(INCLUDE)/sizes.h
	@ echo "Compiling naive in debug mode"
	@ $(CC) $(CC_FLAGS) -o maind main.c mxm.c -g

$(DATA_DIR)/%:
	@ $(MAKE) $@ -C ..

$(WORK_DIR)/%.naive.out: main $(DATA_DIR)/%.dat $(DATA_DIR)/%.ident
	@ ./main $(DATA_DIR)/$*.ident $(DATA_DIR)/$*.dat $(WORK_DIR)/$*.naive.out

verify: $(SIZES:%=%.diff)

%.diff: $(WORK_DIR)/%.naive.out $(DATA_DIR)/%.dat
	@ diff -u $(WORK_DIR)/$*.naive.out $(DATA_DIR)/$*.dat

benchmark: main result $(SIZES:%=bench%)

bench%: main $(DATA_DIR)/%.dat
	@ echo "Benchmarking naive $*"
	@ time -ao result.txt timeout 1h ./main $(word 2,$+) $(word 2,$+) $(WORK_DIR)/naive.out

result:
	@ echo "Wiping last result"
	@ echo "" > result.txt

clean:
	-@ rm -r main maind *.dSYM 2> /dev/null || true

.PHONY: all clean verify %.diff benchmark bench% result
.SECONDARY: main maind $(SIZES:%=$(DATA_DIR)/%.dat) $(SIZES:%=$(DATA_DIR)/%.ident) $(SIZES:%=$(WORK_DIR)/%.naive.out)
