all: $(SIZES:%=main_%) $(SIZES:%=maind_%)

main_%: main.c mxm.c $(INCLUDE)/sizes.h
	@ echo "Compilnig lto main_$*"
	@ $(CC) $(CC_FLAGS) -o $@ main.c mxm.c -O3 -DN=$*

maind_%: main.c mxm.c $(INCLUDE)/sizes.h
	@ echo "Compilnig lto in debug mode"
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
	@ echo "Benchmarking dynamic tiling $*"
	@ time -ao result.txt timeout 1h ./main_$* $(word 2,$+) $(word 2,$+) $(WORK_DIR)/lto_tiling.out

result:
	@ echo "Wiping last result"
	@ echo "" > result.txt

clean:
	-@ rm -r main_* maind_* libmxm*.so libmxm*.o *.dSYM 2> /dev/null || true

.PHONY: all clean verify %.diff benchmark bench% result
.SECONDARY: main maind $(SIZES:%=$(DATA_DIR)/%.dat) $(SIZES:%=$(DATA_DIR)/%.ident) $(SIZES:%=$(WORK_DIR)/%.dynamic_tiling.out)
