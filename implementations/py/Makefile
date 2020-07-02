INCLUDES:=-I. -I../.. -I../../Value
override LDFLAGS:=$(LDFLAGS) -shared -fPIC

ifeq ($(USE_GMP_LIB),1)
override LDFLAGS:=$(LDFLAGS) -lgmp -lgmpxx
override CFLAGS:=$(CFLAGS) -DUSE_GMP_LIB
else
override INCLUDES:=$(INCLUDES) -I../../BigNumber/src/BigNumber
override CFLAGS:=$(CFLAGS) ../../number.o ../../BigNumber.o
endif

override CFLAGS:=$(CFLAGS) $(INCLUDES)

ifeq ($(OS),Windows_NT)
	EXT:=dll
	NAME:=VMPy
else
		NAME:=libVMPy
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
			EXT:=so
    endif
    ifeq ($(UNAME),Darwin)
			EXT=dylib
    endif
endif

all: VMPy.o $(NAME).$(EXT)
.PHONY: all

VMPy.o: VMPy.cpp
	$(CXX) -c $(CFLAGS) $<

$(NAME).$(EXT):
	$(CXX) $(CFLAGS) VMPy.o ../../VM.cpp $(LDFLAGS) -o $(NAME).$(EXT)

clean:
	$(RM) $(classes) *.o *.$(EXT)
