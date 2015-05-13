# GNUmakefile - The makefile for the ibisami example models.
#
# Original author: David Banas
# Original date:   May 9, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

SRCDIR := .
INCDIR := .
INCS := 
MODS := example_tx
LIB_MODS := ibisami_api amimodel ami_tx digital_filter

# Check for proper IBISAMI_ROOT definition.
IBISAMI_ROOT ?= ""
ifeq ($(IBISAMI_ROOT), "")
    $(error You must define environment variable IBISAMI_ROOT.)
endif

include $(IBISAMI_ROOT)/defs.mak

# Handle Windows vs. Linux target naming differences.
ifeq ($(OS), Windows_NT)
    TARGS := $(MODS:%=%_$(SUFFIX).dll)
else
    TARGS := $(MODS:%=%_$(SUFFIX).so)
endif

# Targets
all: $(TARGS)

LIB_OBJS = $(LIB_MODS:%="$(IBISAMI_ROOT_DOS)\\%_$(SUFFIX).obj")
$(TARGS): $(OBJS)
	@echo "Building $@..."
#	$(RUN_CMD) $(LD) $(LDFLAGS) "$(IBISAMI_ROOT_DOS)\$(IBISAMI_LIB)" "$(IBISAMI_ROOT_DOS)\$(IBISAMI_LIB:%.lib=%.exp)" $^
	$(RUN_CMD) $(LD) $(LDFLAGS) $(LIB_OBJS) $^

