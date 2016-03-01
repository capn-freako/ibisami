# defs.mak - Common makefile definitions.
#
# Original author: David Banas
# Original date:   May 9, 2015
#
# Copyright (c) 2015 David Banas; all rights reserved World wide.

# Establish default target.
.PHONY: all clean rebuild
all:

clean:
	@echo "Cleaning up previous build..."
	-rm *.o *.obj *.lib *.manifest *.so *.exe 2>/dev/null

rebuild:
	@$(MAKE) clean
	@$(MAKE) all

# Prevent implicit rule searching for makefiles.
$(MAKEFILE_LIST): ;

# Infrastructure tools
AMI_CONFIG := python -m pyibisami.ami_config
RUN_TESTS  := python -m pyibisami.run_tests

# Machine dependent definitions
MACHINE ?= X86
ifeq ($(MACHINE), X86)
    SUFFIX := x86
else
    ifeq ($(MACHINE), AMD64)
        SUFFIX := x86_amd64
    else
        $(error Unrecognized machine type: $(MACHINE))
    endif
endif

# Handle Windows vs. Linux differences.
OS ?= Linux
ifeq ($(OS), Windows_NT)
    OBJS := $(MODS:%=%_$(SUFFIX).obj)
    ENV_SETTER := $(MSVC_BASE_DOS)\vcvarsall.bat
    RUN_CMD := cmd /C "$(ENV_SETTER)" $(SUFFIX) '&&' 
    CC := cl.exe
    CXX := cl.exe
    LIB := lib.exe
    LD := link.exe
    CFLAGS := /EHsc /Gy /W3 /nologo /c /I. /I"$(IBISAMI_ROOT_DOS)" /I"$(BOOST_ROOT)" /D "WIN32"
    LIBFLAGS = /OUT:$@ /DEF
    LDFLAGS = /INCREMENTAL:NO /NOLOGO /DLL /SUBSYSTEM:WINDOWS /OUT:$@
    ifeq ($(MACHINE), X86)
        LDFLAGS += /MACHINE:X86
    else
        LDFLAGS += /MACHINE:X64
    endif
    ifdef DEBUG
        CFLAGS += /Zi /Od /MTd /D "DEBUG"
        LDFLAGS += /DEBUG
    else
        CFLAGS += /Oi /MT /O2 /D "NDEBUG"
    endif
    CXXFLAGS = $(CFLAGS)
    LDLIBS := kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib \
	      oleaut32.lib uuid.lib odbc32.lib dbccp32.lib
    IBISAMI_LIB := ibisami_$(SUFFIX).lib
else
    OBJS = $(MODS:%=%_$(SUFFIX).o)
    RUN_CMD =
    BIN := /usr/bin
    CC := gcc
    CXX := g++
    LIB := $(CXX)
    LD := $(CXX)
    CFLAGS := -c -fPIC -std=gnu++11 -I. -I"$(IBISAMI_ROOT)" -I"$(BOOST_ROOT)"
    LDFLAGS = -o $@ -shared
    ifeq ($(MACHINE), X86)
        LDFLAGS += -m32
        CFLAGS += -m32
    else
        LDFLAGS += -m64
        CFLAGS += -m64
    endif
    ifdef DEBUG
        CFLAGS += -g
        LDFLAGS += -g
    else
        UNAME_S := $(shell uname -s)
        ifeq ($(UNAME_S), Linux)
            LDFLAGS += -s -static-libgcc -static-libstdc++ $(@:%_$(SUFFIX).so=%.exp)
        else
            ifeq ($(UNAME_S), Darwin)
                LDFLAGS += -static-libstdc++ $(@:%_$(SUFFIX).so=%.exp)
            else
                $(error Unsupported OS: $(UNAME_S))
            endif
        endif
    endif
    CXXFLAGS := $(CFLAGS)
    IBISAMI_LIB := libibisami_$(SUFFIX).a
endif

# Default rules
LIN_CMD = $(RUN_CMD) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< -o $@
%_$(SUFFIX).o : $(SRCDIR)/%.cc
	$(LIN_CMD)
%_$(SUFFIX).o : $(SRCDIR)/%.cpp
	$(LIN_CMD)
%_$(SUFFIX).o : $(SRCDIR)/%.cxx
	$(LIN_CMD)

%_$(SUFFIX).o : $(SRCDIR)/%.c
	$(RUN_CMD) $(CC) $(CPPFLAGS) $(CFLAGS) $< -o $@

WIN_CMD = $(RUN_CMD) $(CXX) $(CPPFLAGS) $(CXXFLAGS) $< /Fo$@
%_$(SUFFIX).obj : $(SRCDIR)/%.cc
	$(WIN_CMD)
%_$(SUFFIX).obj : $(SRCDIR)/%.cpp
	$(WIN_CMD)
%_$(SUFFIX).obj : $(SRCDIR)/%.cxx
	$(WIN_CMD)

%_$(SUFFIX).obj : $(SRCDIR)/%.c
	$(RUN_CMD) $(CC) $(CPPFLAGS) $(CFLAGS) $< /Fo$@

# Support Python model configurator.
%.cpp : %.cpp.em %_config.py
	$(AMI_CONFIG) $@ $^

%.ami : %.ami.em %_config.py
	$(AMI_CONFIG) $@ $^

# Establish object file dependency on include files.
$(OBJS): $(INCS:%=$(INCDIR)/%)

