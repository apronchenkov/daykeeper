SUBDIR=

SRC=\
buffer.cpp\
warper.cpp\
client.cpp\
main.cpp\
inet_addr.cpp\
switch.cpp\
sgnl.cpp\

CPPFLAGS+=-I../inc

.SILENT: all clean $(SUBDIR) $(addsuffix .clean, $(SUBDIR))

.PHONY: all clean $(SUBDIR) $(addsuffix .clean, $(SUBDIR))

all: $(SUBDIR) $(patsubst %.cpp, %.o, $(SRC))

clean: $(addsuffix .clean, $(SUBDIR))
	rm -f $(patsubst %.cpp, %.o, $(SRC))

$(SUBDIR): 
	make -C $@ all

$(addsuffix .clean, $(SUBDIR)): 
	make -C $(patsubst %.clean, %, $@) clean
