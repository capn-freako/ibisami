# GNUmakefile - The makefile for the ibisami example models.
#
# Original author: David Banas
# Original date:   May 9, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

SRCDIR   := .
INCDIR   := ../include
LIB_MODS := ibisami_api amimodel ami_tx ami_rx digital_filter fir_filter dfe util
TX_NAME  := example_tx
RX_NAME  := example_rx

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
.PHONY: all all_tx all_rx tx32 tx64 rx32 rx64 targs txami rxami txibs rxibs test_tx test_rx test_all test
all: all_tx all_rx
all_tx: tx32 tx64 txami txibs
all_rx: rx32 rx64 rxami rxibs
test_all: test_tx test_rx

tx32:
	@echo "Building 32-bit Tx model..."
	MACHINE=X86 MODS=$(TX_NAME) $(MAKE) targs

tx64:
	@echo "Building 64-bit Tx model..."
	MACHINE=AMD64 MODS=$(TX_NAME) $(MAKE) targs

rx32:
	@echo "Building 32-bit Rx model..."
	MACHINE=X86 MODS=$(RX_NAME) INCS='amimodel.h ami_rx.h digital_filter.h fir_filter.h dfe.h' $(MAKE) targs

rx64:
	@echo "Building 64-bit Rx model..."
	MACHINE=AMD64 MODS=$(RX_NAME) INCS='amimodel.h ami_rx.h digital_filter.h fir_filter.h dfe.h' $(MAKE) targs

test_tx:
ifeq '$(shell python -c "import platform; print(platform.architecture()[0])")' '32bit'
	@echo "Testing 32-bit Tx model..."
	MACHINE=X86 MODS=$(TX_NAME) TEST_RUNS=test_runs/tx TEST_OUT=test_results/tx/ $(MAKE) test
else
	@echo "Testing 64-bit Tx model..."
	MACHINE=AMD64 MODS=$(TX_NAME) TEST_RUNS=test_runs/tx TEST_OUT=test_results/tx/ $(MAKE) test
endif

test_rx:
ifeq '$(shell python -c "import platform; print(platform.architecture()[0])")' '32bit'
	@echo "Testing 32-bit Rx model..."
	MACHINE=X86 MODS=$(RX_NAME) TEST_RUNS=test_runs/rx TEST_OUT=test_results/rx/ $(MAKE) test
else
	@echo "Testing 64-bit Rx model..."
	MACHINE=AMD64 MODS=$(RX_NAME) TEST_RUNS=test_runs/rx TEST_OUT=test_results/rx/ $(MAKE) test
endif

txami: $(TX_NAME).ami
txibs: $(TX_NAME).ibs

rxami: $(RX_NAME).ami
rxibs: $(RX_NAME).ibs

targs: $(TARGS)

$(TARGS): $(OBJS) $(LIB_DEPS)
	@echo "Building $@..."
	$(RUN_CMD) $(LD) $(LDFLAGS) $(OBJS) $(LIB_OBJS)

test:
	@if [ ! -d $(TEST_OUT) ]; then mkdir -p $(TEST_OUT); fi
	$(RUN_TESTS) -m $(TARGS) -p $(TEST_RUNS) -o $(TEST_OUT)

