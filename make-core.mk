#
# Colibri Build System
#
# Copyright (c) 2006-2023 Alexei A. Smekalkine <ikle@ikle.ru>
#
# SPDX-License-Identifier: BSD-2-Clause
#

#
# target paths
#

PREFIX	?= /usr
INCDIR	?= $(PREFIX)/include
LIBDIR	?= $(PREFIX)/lib/$(MULTIARCH)
BINDIR	?= $(PREFIX)/bin
SBINDIR	?= $(PREFIX)/sbin
DESTDIR	?=

#
# target dependencies
#

ifneq ($(DEPENDS),)
ifneq ($(SYSROOT),)
CFLAGS	+= `pkg-config --define-prefix $(DEPENDS) --cflags`
LDFLAGS	+= `pkg-config --define-prefix $(DEPENDS) --libs`
endif
CFLAGS	+= `pkg-config $(DEPENDS) --cflags`
LDFLAGS	+= `pkg-config $(DEPENDS) --libs`
endif

#
# guarantie default target
#

.PHONY: all clean install doc

all:

#
# source and target file filters
#

HEADERS	= $(wildcard include/*.h include/*/*.h include/*/*/*.h)
SOURCES	= $(filter-out %-test.c %-tool.c %-service.c %-module.c, $(wildcard *.c))
OBJECTS	= $(patsubst %.c,%.o, $(SOURCES))

TESTS	= $(patsubst %-test.c,%-test, $(wildcard *-test.c))
TOOLS	= $(patsubst %-tool.c,%, $(wildcard *-tool.c))
SERVICES = $(patsubst %-service.c,%, $(wildcard *-service.c))
MODULES	= $(patsubst %-module.c,%, $(wildcard *-module.c))

#
# rules to manage static libraries
#

%.a:
	$(AR) -rc $@ $^

.PHONY: install-headers
.PHONY: build-static clean-static install-static

ifdef LIBNAME

AFILE	= lib$(LIBNAME).a
LIBVER	?= 0
LIBREV	?= 0.1
PCFILE	= $(LIBNAME).pc

install: install-headers

INCROOT	= $(INCDIR)/$(LIBNAME)-$(LIBVER)

define install-header
install-headers:: ; install -Dm 644 $(1) $(DESTDIR)$(INCROOT)/$(1:include/%=%)
endef

$(foreach F,$(HEADERS),$(eval $(call install-header,$(F))))

all:     build-static
install: install-static

$(OBJECTS): CFLAGS += -I$(CURDIR)/include

$(PCFILE):
	@echo "prefix=$(PREFIX)"					>  $@
	@echo "includedir=\$${prefix}$(INCDIR:$(PREFIX)%=%)"		>> $@
	@echo "libdir=\$${prefix}$(LIBDIR:$(PREFIX)%=%)"		>> $@
	@echo								>> $@
	@test -n "$(DESCRIPTION)" && echo "Description: $(DESCRIPTION)"	>> $@
	@test -n "$(URL)" && echo "URL: $(URL)"		>> $@
	@echo "Name: $(LIBNAME)"			>> $@
	@echo "Version: $(LIBVER).$(LIBREV)"		>> $@
ifneq ($(DEPENDS),)
	@echo "Requires: $(DEPENDS)"			>> $@
endif
	@echo "Libs: -L\$${libdir} -l$(LIBNAME)"			>> $@
	@echo "Cflags: -I\$${includedir}$(INCROOT:$(INCDIR)%=%)"	>> $@

install-static: $(AFILE) $(PCFILE)
	install -d $(DESTDIR)$(LIBDIR)/pkgconfig
	install -m 644 $(AFILE) $(DESTDIR)$(LIBDIR)
	install -m 644 $(PCFILE) $(DESTDIR)$(LIBDIR)/pkgconfig

else  # not defined LIBNAME

AFILE	= bundle.a

endif  # LIBNAME

$(AFILE): $(OBJECTS)

build-static: $(AFILE)

clean: clean-static

clean-static:
	$(RM) $(AFILE) $(OBJECTS) $(PCFILE)

#
# rules to manage tests (ordinary programs)
#

ifneq ($(TESTS),)

%-test: %-test.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: build-test clean-tests

all:     build-tests
clean:   clean-tests

$(TESTS): CFLAGS += -I$(CURDIR)/include
$(TESTS): $(AFILE)

build-tests: $(TESTS)
clean-tests:
	$(RM) $(TESTS)

endif  # build TESTS

#
# rules to manage tools (ordinary programs)
#

ifneq ($(TOOLS),)

%: %-tool.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: build-tools clean-tools install-tools

all:     build-tools
clean:   clean-tools
install: install-tools

$(TOOLS): CFLAGS += -I$(CURDIR)/include
$(TOOLS): $(AFILE)

build-tools: $(TOOLS)
clean-tools:
	$(RM) $(TOOLS)

install-tools: build-tools
	install -d $(DESTDIR)$(BINDIR)
	install -s -m 755 $(TOOLS) $(DESTDIR)$(BINDIR)

endif  # build TOOLS

#
# rules to manage services (system programs)
#

ifneq ($(SERVICES),)

%: %-service.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: build-services clean-services install-services

all:     build-services
clean:   clean-services
install: install-services

$(SERVICES): CFLAGS += -I$(CURDIR)/include
$(SERVICES): $(AFILE)

build-services: $(SERVICES)
clean-services:
	$(RM) $(SERVICES)

install-services: build-services
	install -d $(DESTDIR)$(SBINDIR)
	install -s -m 755 $(SERVICES) $(DESTDIR)$(SBINDIR)

endif  # build SERVICES

#
# rules to manage modules (private programs)
#

ifneq ($(MODULES),)
ifdef LIBNAME

%: %-module.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

.PHONY: build-modules clean-modules install-modules

all:     build-modules
clean:   clean-modules
install: install-modules

$(MODULES): CFLAGS += -I$(CURDIR)/include
$(MODULES): $(AFILE)

build-modules: $(MODULES)
clean-modules:
	$(RM) $(MODULES)

install-modules: build-modules
	install -d $(DESTDIR)$(LIBDIR)$(LIBNAME)
	install -s -m 755 $(MODULES) $(DESTDIR)$(LIBDIR)$(LIBNAME)

endif  # LIBNAME
endif  # build MODULES
