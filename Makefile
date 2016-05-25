TARGETS = ber-dump

CFLAGS ?= -O6

all: $(TARGETS)

clean:
	rm -f *.o $(TARGETS)

PREFIX ?= /usr/local

install: $(TARGETS)
	install -D -d $(DESTDIR)/$(PREFIX)/bin
	install -s -m 0755 $^ $(DESTDIR)/$(PREFIX)/bin

ber-dump: asn1-input.o asn1-int.o
ber-dump: ber-input.o ber-int.o ber-time.o
