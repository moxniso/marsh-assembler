CC := gcc

CFLAGS := -std=gnu99 -O2

HDRS := src/libmarsh.h src/marshdefs.h src/label.h src/vector.h

SRCS := src/main.c src/assembler.c src/libmarsh.c src/outputROM.c src/label.c src/vector.c

OBJS := $(SRCS:.c=.o)

EXEC := mas

all: $(EXEC)

$(EXEC): $(OBJS) $(HDRS) Makefile
	$(CC) -o $@ $(OBJS) $(CFLAGS)

$(OBJS): $(@:.o=.c) $(HDRS) Makefile
	$(CC) -o $@ $(@:.o=.c) -c $(CFLAGS)

clean:
	rm -f $(EXEC) $(OBJS)
install:
	cp $(EXEC) /usr/bin

.PHONY: all install clean
