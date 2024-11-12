all: $(SIZES:%=main_%)

main_%: main.c mxm.c $(INCLUDE)/sizes.h
	@ echo "Compilnig perfect tiling main_$*"
	@ $(CC) $(CC_FLAGS) -o $@ main.c mxm.c -O3 -DN=$*

maind_%: main.c mxm.c $(INCLUDE)/sizes.h
	@ echo "Compilnig perfect tiling in debug mode"
	@ $(CC) $(CC_FLAGS) -o $@ main.c mxm.c -g -DN=$*

$(DATA_DIR)/%:
	@ $(MAKE) $@ -C ..

$(WORK_DIR)/%.dynamic_tiling.out: main $(DATA_DIR)/%.dat $(DATA_DIR)/%.ident
	@ ./main $(DATA_DIR)/$*.ident $(DATA_DIR)/$*.dat $(WORK_DIR)/$*.dynamic_tiling.out

verify: $(SIZES:%=%.diff)

%.diff: $(WORK_DIR)/%.dynamic_tiling.out $(DATA_DIR)/%.dat
	@ diff -u $(WORK_DIR)/$*.dynamic_tiling.out $(DATA_DIR)/$*.dat

benchmark: result $(SIZES:%=bench%)

bench%: main_% $(DATA_DIR)/%.dat
	@ echo "Benchmarking perfect tiling $*"
	@ time -ao result.txt timeout 1h ./main_$* $(word 2,$+) $(word 2,$+) $(WORK_DIR)/perfect_tiling.out

../instrument.csv:
	$(MAKE) instrument.csv -C ..

instrument: $(SIZES:%=instr_%)

instr_%: main_% $(DATA_DIR)/%.dat ../instrument.csv
	@ echo "Instrumenting perfect tiling $*"
	@ timeout 1h ./main_$* $(word 2,$+) $(word 2,$+) $(WORK_DIR)/perfect_tiling.out >> ../instrument.csv

result:
	@ echo "Wiping last result"
	@ echo "" > result.txt

clean:
	-@ rm -r main_* maind_* libmxm*.so libmxm*.o *.dSYM 2> /dev/null || true

.PHONY: all clean verify %.diff benchmark bench% result
.SECONDARY: main maind $(SIZES:%=$(DATA_DIR)/%.dat) $(SIZES:%=$(DATA_DIR)/%.ident) $(SIZES:%=$(WORK_DIR)/%.dynamic_tiling.out)
