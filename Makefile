CFLAGS=-std=c++17
LDFLAGS=-ldl -pthread

all: VM assembler disassembler mkll repl

VM: VM.o main.o assembler_functions.o
	$(CXX) $(CFLAGS) VM.o main.o assembler_functions.o -o VM $(LDFLAGS)

assembler: VM.o assembler_functions.o assembler.cpp
	$(CXX) $(CFLAGS) assembler.cpp VM.o assembler_functions.o -o assembler $(LDFLAGS)

disassembler: VM.o assembler_functions.o disassembler.cpp
	$(CXX) $(CFLAGS) disassembler.cpp assembler_functions.o VM.o -o disassembler $(LDFLAGS)

mkll: VM.o mkll.cpp VM_functions.o
	$(CXX) $(CFLAGS) mkll.cpp VM.o -o mkll $(LDFLAGS)

mkcc: VM.o mkcc.cpp VM_functions.o
	$(CXX) $(CFLAGS) mkcc.cpp VM.o -o mkcc $(LDFLAGS)

VM_functions.o: VM_functions.cpp VM_functions.h
	$(CXX) $(CFLAGS) -c VM_functions.cpp $(LDFLAGS)

repl: repl.cpp assembler_functions.o VM.o
	$(CXX) $(CFLAGS) repl.cpp assembler_functions.o VM.o -o repl $(LDFLAGS)

main.o: main.cpp
	$(CXX) $(CFLAGS) -c main.cpp $(LDFLAGS)

VM.o: VM.h VM.cpp VM_confs.h
	$(CXX) $(CFLAGS) -c VM.cpp $(LDFLAGS)

assembler_functions.o:
	$(CXX) $(CFLAGS) -c assembler_functions.cpp $(LDFLAGS)

clean:
	$(RM) *.o VM assembler disassembler mkll repl
