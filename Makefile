INCLUDES=-IBigNumber/src/BigNumber -IValue
DEFINES=-DUSE_UTILS -DSTD_INCLUDED
CFLAGS=-std=c++17 $(DEFINES) $(INCLUDES) number.o BigNumber.o
LDFLAGS=-ldl -pthread

all: VM assembler disassembler mkcc repl
.PHONY: all

VM: number.o BigNumber.o VM.o main.o
	$(CXX) $(CFLAGS) VM.o main.o -o VM $(LDFLAGS)

assembler: number.o BigNumber.o VM.o assembler_functions.o assembler.cpp
	$(CXX) $(CFLAGS) assembler.cpp VM.o assembler_functions.o -o assembler $(LDFLAGS)

disassembler: number.o BigNumber.o VM.o assembler_functions.o disassembler.cpp
	$(CXX) $(CFLAGS) disassembler.cpp assembler_functions.o VM.o -o disassembler $(LDFLAGS)

mkcc: number.o BigNumber.o VM.o mkcc.cpp
	$(CXX) $(CFLAGS) mkcc.cpp VM.o -o mkcc $(LDFLAGS)

repl: number.o BigNumber.o repl.cpp assembler_functions.o VM.o
	$(CXX) $(CFLAGS) repl.cpp assembler_functions.o VM.o -o repl $(LDFLAGS)

number.o: BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/number.h
	$(CC) -c BigNumber/src/BigNumber/number.c

BigNumber.o: BigNumber/src/BigNumber/BigNumber.cpp BigNumber/src/BigNumber/BigNumber.h
	$(CXX) -c BigNumber/src/BigNumber/BigNumber.cpp $(DEFINES)

main.o: main.cpp number.o BigNumber.o
	$(CXX) $(CFLAGS) -c main.cpp $(LDFLAGS)

VM.o: VM.h VM.cpp VM_confs.h number.o BigNumber.o
	$(CXX) $(CFLAGS) -c VM.cpp number.o BigNumber.o $(LDFLAGS)

assembler_functions.o: number.o BigNumber.o assembler_functions.h assembler_functions.cpp
	$(CXX) $(CFLAGS) -c assembler_functions.cpp $(LDFLAGS)

clean:
	$(RM) *.o VM assembler disassembler repl mkcc
.PHONY: clean
