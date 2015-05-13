# GNUmakefile - The makefile for the ibisami library.
#
# Original author: David Banas
# Original date:   May 2, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

SRCDIR := src
INCDIR := include
INCS := ami_tx.h amimodel.h digital_filter.h
MODS := ibisami_api amimodel ami_tx digital_filter

# Check for proper IBISAMI_ROOT definition.
IBISAMI_ROOT ?= ""
ifeq ($(IBISAMI_ROOT), "")
    $(error You must define environment variable IBISAMI_ROOT.)
endif

# Bring in common definitions.
include $(IBISAMI_ROOT)/defs.mak

# Default Target
# (This is the static library that device specific DLL/SO builds will link against.)
all: $(IBISAMI_LIB)

# Remaining Targets
$(IBISAMI_LIB): $(OBJS)
	@echo "Building $@..."
	$(RUN_CMD) $(LIB) $(LIBFLAGS) $^

