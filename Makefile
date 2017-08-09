EXE_FILE = cryptlib-test
CRYPTLIB = -lcl
LIBS += $(CRYPTLIB)
CFLAGS = -g -Wall -O0

.PHONY: all
.PHONY: clean

all: $(EXE_FILE)
%: %.o
	$(LINK.o) -o $@ $^ $(LIBS)

%.o: %.c %.h
	$(COMPILE.c) -o $@ $<

%.o: %.c
	$(COMPILE.c) -o $@ $<

clean:
	$(RM) -rf *.o $(EXE_FILE)
