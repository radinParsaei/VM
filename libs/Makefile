CFLAGS=-DUSE_UTILS -DSTD_INCLUDED -std=c++14 -I$(VM_ROOT) -I$(VM_ROOT)/libs -I$(VM_ROOT)/BigNumber/src -I$(VM_ROOT)/Value $(FLAGS)
LDFLAGS?=

ifeq ($(OS),Windows_NT)
	CFLAGS += -fdata-sections -ffunction-sections
	LDFLAGS += -Wl,--gc-sections
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
			CFLAGS += -fdata-sections -ffunction-sections
      LDFLAGS += -Wl,--gc-sections
    endif
    ifeq ($(UNAME),Darwin)
			LDFLAGS += -dead_strip -dead_strip_dylibs
    endif
endif

all: compile
.PHONY: all

compile: check_VM_ROOT check_LIBNAME $(LIBNAME).vmso

%.vmso: %.o $(VM_ROOT)/number.o $(VM_ROOT)/BigNumber.o $(VM_ROOT)/VM.o
	$(CXX) -shared -fPIC -o $@ $(CFLAGS) $< $(VM_ROOT)/number.o $(VM_ROOT)/BigNumber.o $(VM_ROOT)/VM.o $(LDFLAGS)

$(VM_ROOT)/number.o:
	(cd $(VM_ROOT) && $(MAKE) number.o)

$(VM_ROOT)/BigNumber.o:
	(cd $(VM_ROOT) && $(MAKE) BigNumber.o)

$(VM_ROOT)/VM.o:
	(cd $(VM_ROOT) && $(MAKE) VM.o)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

ifeq ($(VM_ROOT),)
check_VM_ROOT:
	@echo please set VM_ROOT
	@false
else
check_VM_ROOT:
endif

ifeq ($(LIBNAME),)
check_LIBNAME:
	@echo please set LIBNAME
	@false
else
check_LIBNAME:
endif

clean:
	$(RM) *.o *.vmso
.PHONY: clean

.PRECIOUS: %.o
