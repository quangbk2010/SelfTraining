flag0 = subst_func_list
flag1 = #origin_func#subst_func#call_func#wildcard_func #Note the 'space' after variable
define defined_var =
echo $(flag1)
echo flag1
endef

all: 

ifeq ($(flag1),runMakefile)
	cc -o main main.c a.c
else
ifdef flag1
#$(origin flag1) is 'file' if it is an empty variable (it is still defined). if it is not defined, it results 'undefined'
#@echo ---eval function
	$(eval var = aaa aa a)
	@echo $(var)
endif
endif
#-----------------------------------------------------------------
ifeq ($(flag1), $(flag0))
	@echo ---compare
endif

ifeq ($(flag1),eval)
	@echo ---eval function
	$(eval var = aaa aa a)
	@echo $(var)
endif

ifeq ($(flag1),subst_ref)
	@echo --- substitution reference
	@echo $(g_var: c = b)
	@echo $(g_var:c=b)
endif
#-----------------------------------------------------------------
ifeq ($(flag0),subst_func_list)
	@echo *** function list for string substitution

ifeq ($(flag1),subst_func)
	@echo ---subst function
#@echo $(subst a,d,$(var))
	@echo $(subst %a,%b,%aa aa a)
#endif

#ifeq ($(flag1),patsubst_func)
	@echo ---patsubst function
#@echo $(patsubst %a,%b,aaaa aaa aa a) 
	@echo $(patsubst %a,%b,%aa aa a)
#var = aaa aaa aa a
	#var = aaa aaa aa a # this line will be printed?
	#@echo $(patsubst aa,b,aaaa aaa aa a)
	#@echo $(patsubst %.c, %.o, x.c.c bar.c)
endif
endif
#-----------------------------------------------------------------
ifeq ($(flag1),filter_func)
	@echo ---filter function
	$(eval var = aaaa abaa aaba aaab bb cc aff)
	$(eval option = aaab bb)
	@echo filter1: $(filter %a %b,$(var))
	@echo filter2: $(filter $(option),$(var))
	@echo filter-out: $(filter-out $(option),$(var))
endif

ifeq ($(flag1), sort_func)
	@echo ---sort_func
	$(eval var = a d c v b bb c)
	@echo $(sort $(var))
endif
#-----------------------------------------------------------------
ifeq ($(flag0), filename_func_list)
	@echo ***prefix_func_list
ifeq ($(flag1), dir_func)
	@echo ---dir_func
	@echo dir: $(dir include/Makefile main.c main.o)
	@echo not-dir: $(notdir include/Makefile main.c main.o)
endif

ifeq ($(flag1), prefix_func)
	@echo ---prefix_func
	@echo dir: $(addprefix include/, foo bar)
endif
endif
#-----------------------------------------------------------------
ifeq ($(flag0), condition_func_list)
	@echo ---condition_func_list
ifeq ($(flag1), wildcard_func)
	@echo ---wildcard_func
	@echo $(wildcard *.c)
	@echo $(wildcard .config)

ifneq (.config, $(wildcard .config))
	@echo noooooo
else
include .config #not exist yet
endif

endif
endif
#-----------------------------------------------------------------
reverse = $(2) $(1)
foo = $(call reverse,a,b)
pathsearch = $(firstword $(wildcard $(1)))
LS := $(call pathsearch,*.o)
call: 
ifeq ($(flag1),call_func)
	@echo x = -$(foo)-
	@echo pathsearch: -$(LS)-
endif
foo = $(call reverse,c,d)
#-----------------------------------------------------------------
bletch = a
origin:
ifeq ($(flag1),origin_func)
	@echo ---how variable was defined: $(origin bletch) - $(origin PATH) - $(origin var) - $(origin CC)- $(origin CFLAGS) - $(origin V) #use 'make V=1' to see all of the options
endif
#-----------------------------------------------------------------
error:
ifeq ($(flag1),error_func)
	$(error this is an error)
endif
#-----------------------------------------------------------------
shell:
	@echo Content of ./include/Makefile: $(shell cat ./include/Makefile)
	@echo $(shell cd include; ls -al)
#-----------------------------------------------------------------
sinclude def
#include abc  #cause error
-include bcd # run without error # other way: sinclude
include_file:
	@echo ---include_file
	@echo $(CONST_var1)
#-----------------------------------------------------------------
export_var = 123
export export_var
export_file:
	$(if $(export_var),@echo ---export_file,@echo ---please declare export_var)
#$(MAKE) -C ./include/ all#ok
	@echo Mainmake: $(export_var)
	$(MAKE) include1 || exit $$? #this line means that we use shell command: 'make include' -> make will run to the target: 'include'.
include1:
	@echo ---include target
	$(MAKE) -C include
%_sub:
	@echo ---$@ target
	$(MAKE) -C include $(@:_sub=) sub_var = 2
#-----------------------------------------------------------------
check_shell:
	cd include && \
	ls 
	@ls -al #@ is used to avoid print out the command
#-----------------------------------------------------------------
# can call: 'make one' or 'make target' (the same)
one target:
	@echo ---one target $(CURDIR) ---$@

define_target:
	$(defined_var)

.PHONY: clean
clean:
	rm *.o
