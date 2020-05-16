INCLUDES=-IBigNumber/src/BigNumber -IValue
DEFINES=-DUSE_UTILS -DSTD_INCLUDED
CFLAGS=-std=c++17 $(DEFINES) $(INCLUDES) number.o BigNumber.o
LDFLAGS=-ldl -pthread -lreadline

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

all: VM assembler disassembler mkcc repl
.PHONY: all

VM: number.o BigNumber.o VM.o main.o
	echo $(LDFLAGS)
	$(CXX) $(CFLAGS) -ffunction-sections -fdata-sections VM.o main.o -o VM $(LDFLAGS)

assembler: number.o BigNumber.o VM.o assembler.cpp
	$(CXX) $(CFLAGS) assembler.cpp VM.o -o assembler $(LDFLAGS)

disassembler: number.o BigNumber.o VM.o disassembler.cpp
	$(CXX) $(CFLAGS) disassembler.cpp VM.o -o disassembler $(LDFLAGS)

mkcc: number.o BigNumber.o VM.o mkcc.cpp
	$(CXX) $(CFLAGS) mkcc.cpp VM.o -o mkcc $(LDFLAGS)

repl: number.o BigNumber.o repl.cpp VM.o
	$(CXX) $(CFLAGS) repl.cpp VM.o -o repl $(LDFLAGS)

number.o: BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/number.h
	$(CC) -c BigNumber/src/BigNumber/number.c

BigNumber.o: BigNumber/src/BigNumber/BigNumber.cpp BigNumber/src/BigNumber/BigNumber.h
	$(CXX) -c BigNumber/src/BigNumber/BigNumber.cpp $(DEFINES)

main.o: main.cpp number.o BigNumber.o
	$(CXX) $(CFLAGS) -c main.cpp $(LDFLAGS)

VM.o: VM.h VM.cpp VM_confs.h number.o BigNumber.o
	$(CXX) $(CFLAGS) -c VM.cpp number.o BigNumber.o $(LDFLAGS)

clean:
	$(RM) *.o VM assembler disassembler repl mkcc
.PHONY: clean
