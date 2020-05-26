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

%.vmso: %.o number.o BigNumber.o VM.o
	$(CXX) -shared -fPIC -o $@ $(CFLAGS) $< number.o BigNumber.o VM.o $(LDFLAGS)

number.o:
	$(CC) -fPIC -c $(VM_ROOT)/BigNumber/src/BigNumber/number.c

BigNumber.o:
	$(CXX) $(CFLAGS) -fPIC -c $(VM_ROOT)/BigNumber/src/BigNumber/BigNumber.cpp

VM.o:
	$(CXX) $(CFLAGS) -fPIC -c $(VM_ROOT)/VM.cpp

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -fPIC

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
