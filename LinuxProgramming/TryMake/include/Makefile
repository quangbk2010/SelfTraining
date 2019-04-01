#export_var = 567
sub_var = 
all:
	gcc -c lib.c
	@echo Submake: $(export_var) $(origin export_var) sub_var: $(sub_var) # have to call this Makefile in top-level Makefile

option:
	@echo ---this is option target of Submake
