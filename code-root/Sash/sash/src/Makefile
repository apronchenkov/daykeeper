SUBDIR=

SRC=\
    io.c\
    main.c\
    pool.c\
    scrout.c\
    shell.c\
    signl.c\
    my_string.c\
    my_strerror.c\
    warnout.c\
    shellout.c\
    my_exec.c\
    warper.c\
    pool_list.c

CPPFLAGS+=-I../inc

.SILENT: all clean $(SUBDIR) $(addsuffix .clean, $(SUBDIR))

.PHONY: all clean $(SUBDIR) $(addsuffix .clean, $(SUBDIR))

all: $(SUBDIR) $(patsubst %.c, %.o, $(SRC))

clean: $(addsuffix .clean, $(SUBDIR))
	rm -f $(patsubst %.c, %.o, $(SRC))

$(SUBDIR): 
	make -C $@ all

$(addsuffix .clean, $(SUBDIR)): 
	make -C $(patsubst %.clean, %, $@) clean
