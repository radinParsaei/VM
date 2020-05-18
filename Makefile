INCLUDES=-IBigNumber/src/BigNumber -IValue
ifeq ($(OS),Windows_NT)
DEFINES=-DUSE_UTILS -DSTD_INCLUDED
else
DEFINES=-DUSE_UTILS -DSTD_INCLUDED -DUSE_READLINE
endif
CFLAGS=-std=c++14 $(DEFINES) $(INCLUDES) $(FLAGS)
OBJS = number.o BigNumber.o
LDFLAGS=

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

ifeq ($(OS),Windows_NT)
VM: $(OBJS) VM.o main.o
	$(CXX) $(CFLAGS) VM.o main.o $(OBJS) -o VM $(LDFLAGS)
else
VM: $(OBJS) VM.o main.o
	$(CXX) $(CFLAGS) VM.o main.o $(OBJS) -o VM -pthread -ldl $(LDFLAGS)
endif

assembler: number.o BigNumber.o VM.o assembler.o
	$(CXX) $(CFLAGS) assembler.o VM.o $(OBJS) -o assembler $(LDFLAGS)

disassembler: number.o BigNumber.o VM.o disassembler.o
	$(CXX) $(CFLAGS) disassembler.o VM.o $(OBJS) -o disassembler $(LDFLAGS)

mkcc: number.o BigNumber.o VM.o mkcc.o
	$(CXX) $(CFLAGS) mkcc.o VM.o $(OBJS) -o mkcc $(LDFLAGS)

ifeq ($(OS),Windows_NT)
repl: number.o BigNumber.o repl.o VM.o
	$(CXX) $(CFLAGS) repl.o VM.o $(OBJS) -o repl $(LDFLAGS)
else
repl: number.o BigNumber.o repl.o VM.o
	$(CXX) $(CFLAGS) repl.o VM.o $(OBJS) -o repl -lreadline -ldl -pthread $(LDFLAGS)
endif

number.o: BigNumber/src/BigNumber/number.c BigNumber/src/BigNumber/number.h
	$(CC) -c BigNumber/src/BigNumber/number.c

BigNumber.o: BigNumber/src/BigNumber/BigNumber.cpp BigNumber/src/BigNumber/BigNumber.h
	$(CXX) -c BigNumber/src/BigNumber/BigNumber.cpp $(DEFINES)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

clean:
	$(RM) *.o VM assembler disassembler repl mkcc
.PHONY: clean
