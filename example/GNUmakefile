# GNUmakefile - The makefile for the ibisami example models.
#
# Original author: David Banas
# Original date:   May 9, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

SRCDIR := .
INCDIR := ../include
#INCS := 
LIB_MODS := ibisami_api amimodel ami_tx ami_rx digital_filter fir_filter dfe util

# Check for proper IBISAMI_ROOT definition.
IBISAMI_ROOT ?= ""
ifeq ($(IBISAMI_ROOT), "")
    $(error You must define environment variable IBISAMI_ROOT.)
endif
include $(IBISAMI_ROOT)/defs.mak

# Handle Windows vs. Linux target naming differences.
ifeq ($(OS), Windows_NT)
    TARGS := $(MODS:%=%_$(SUFFIX).dll)
    LIB_OBJS = $(LIB_MODS:%="$(IBISAMI_ROOT_DOS)\\%_$(SUFFIX).obj")
    LIB_DEPS = $(LIB_MODS:%=$(IBISAMI_ROOT)/%_$(SUFFIX).obj)
else
    TARGS := $(MODS:%=%_$(SUFFIX).so)
    LIB_OBJS = $(LIB_MODS:%=$(IBISAMI_ROOT)/%_$(SUFFIX).o)
    LIB_DEPS = $(LIB_MODS:%=$(IBISAMI_ROOT)/%_$(SUFFIX).o)
endif

# Targets
.PHONY: all tx32 tx64 rx32 rx64 targs
all: tx32 tx64 rx32 rx64

tx32:
	@echo "Building 32-bit Tx model..."
	MACHINE=X86 MODS='example_tx' $(MAKE) targs

tx64:
	@echo "Building 64-bit Tx model..."
	MACHINE=AMD64 MODS='example_tx' $(MAKE) targs

rx32:
	@echo "Building 32-bit Rx model..."
	MACHINE=X86 MODS='example_rx' INCS='amimodel.h ami_rx.h digital_filter.h fir_filter.h dfe.h' $(MAKE) targs

rx64:
	@echo "Building 64-bit Rx model..."
	MACHINE=AMD64 MODS='example_rx' INCS='amimodel.h ami_rx.h digital_filter.h fir_filter.h dfe.h' $(MAKE) targs

targs: $(TARGS)

$(TARGS): $(OBJS) $(LIB_DEPS)
	@echo "Building $@..."
#	$(RUN_CMD) $(LD) $(LDFLAGS) "$(IBISAMI_ROOT_DOS)\$(IBISAMI_LIB)" "$(IBISAMI_ROOT_DOS)\$(IBISAMI_LIB:%.lib=%.exp)" $^
	$(RUN_CMD) $(LD) $(LDFLAGS) $(OBJS) $(LIB_OBJS)

