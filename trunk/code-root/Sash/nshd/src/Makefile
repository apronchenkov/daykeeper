SUBDIR=

SRC=\
warper.cpp\
main.cpp\
sgnl.cpp\
buffer.cpp\
link.cpp\
server.cpp\
switch.cpp

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
