# GNUmakefile - The makefile for the ibisami library.
#
# Original author: David Banas
# Original date:   May 2, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

SRCDIR := src
INCDIR := include
INCS := ami_tx.h ami_rx.h amimodel.h digital_filter.h dfe.h fir_filter.h util.h
MODS := ibisami_api amimodel ami_tx ami_rx digital_filter dfe fir_filter util

# Bring in common definitions.
include ./defs.mak

# Targets
.PHONY: all x32 x64 targs
all: x32 x64

x32:
	@echo "Building 32-bit ibisami library..."
	MACHINE=X86 $(MAKE) targs

x64:
	@echo "Building 64-bit ibisami library..."
	MACHINE=AMD64 $(MAKE) targs

targs: $(IBISAMI_LIB)

$(IBISAMI_LIB): $(OBJS)
	@echo "Built: $(OBJS)."
