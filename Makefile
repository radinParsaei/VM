INCLUDES=-IBigNumber/src/BigNumber -IValue
ifeq ($(OS),Windows_NT)
DEFINES=-DUSE_UTILS -DSTD_INCLUDED
else
DEFINES=-DSTD_INCLUDED -DUSE_READLINE
endif
CFLAGS=-std=c++14 $(DEFINES) $(INCLUDES) $(EXT_CFLAGS)
OBJS = number.o BigNumber.o
LDFLAGS=$(EXT_LDFLAGS)

ifeq ($(OS),Windows_NT)
	override CFLAGS += -fdata-sections -ffunction-sections
	override LDFLAGS += -Wl,--gc-sections
else
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
			override CFLAGS += -fdata-sections -ffunction-sections
      override LDFLAGS += -Wl,--gc-sections
    endif
    ifeq ($(UNAME),Darwin)
			override LDFLAGS += -dead_strip -dead_strip_dylibs
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
	$(CC) -c BigNumber/src/BigNumber/number.c $(EXT_CFLAGS)

BigNumber.o: BigNumber/src/BigNumber/BigNumber.cpp BigNumber/src/BigNumber/BigNumber.h
	$(CXX) -c BigNumber/src/BigNumber/BigNumber.cpp $(DEFINES) $(EXT_CFLAGS)

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $<

clean:
	$(RM) -r *.o VM assembler disassembler repl mkcc x86_64/ arm64/
.PHONY: clean
apple-universal2:
	$(MAKE) EXT_CFLAGS="$(EXT_CFLAGS) -target x86_64-apple-macos10.12" EXT_LDFLAGS=$(EXT_LDFLAGS)
	-mkdir x86_64
	mv {*.o,VM,assembler,disassembler,repl,mkcc} x86_64
	$(MAKE) EXT_CFLAGS="$(EXT_CFLAGS) -target arm64-apple-macos11" EXT_LDFLAGS=$(EXT_LDFLAGS)
	-mkdir arm64
	mv {*.o,VM,assembler,disassembler,repl,mkcc} arm64
	lipo -create -output VM arm64/VM x86_64/VM
	lipo -create -output assembler arm64/assembler x86_64/assembler
	lipo -create -output disassembler arm64/disassembler x86_64/disassembler
	lipo -create -output mkcc arm64/mkcc x86_64/mkcc
	lipo -create -output repl arm64/repl x86_64/repl
.PHONY: apple-universal2
