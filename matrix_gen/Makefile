mg: matrix_gen.c $(wildcard $(INCLUDE)/*.c) $(wildcard $(INCLUDE)/*.h)
	@ echo "Compiling mg"
	@ $(CC) $(CC_FLAGS) -o mg matrix_gen.c -O3

$(DATA_DIR)/%.dat: mg
	@ echo "Generating $*.dat"
	@ ./mg $(DATA_DIR)/$*.dat $*

$(DATA_DIR)/%.ident: mg
	@ echo "Generating $*.ident"
	@ ./mg $(DATA_DIR)/$*.ident $* ident

clean:
	-@ rm -r mg *.dSYM 2> /dev/null || true

.SECONDARY: mg
