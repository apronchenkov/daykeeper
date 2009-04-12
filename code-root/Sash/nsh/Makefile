SUBDIR=\
    src

SRC=

CPPFLAGS+=-Iinc

.SILENT: all clean $(SUBDIR) $(addsuffix .clean, $(SUBDIR))

.PHONY: all clean $(SUBDIR) $(addsuffix .clean, $(SUBDIR))

all: $(SUBDIR) $(patsubst %.c, %.o, $(SRC))

clean: $(addsuffix .clean, $(SUBDIR))
	rm -f $(patsubst %.c, %.o, $(SRC))

$(SUBDIR):
	make -C $@ all

$(addsuffix .clean, $(SUBDIR)):
	make -C $(patsubst %.clean, %, $@) clean
