INCLUDES:=-I. -I../.. -I../../Value
override LDFLAGS:=$(LDFLAGS) -shared -fPIC

ifeq ($(USE_GMP_LIB),1)
override LDFLAGS:=$(LDFLAGS) -lgmp -lgmpxx
override CFLAGS:=$(CFLAGS) -DUSE_GMP_LIB -fPIC
else
override INCLUDES:=$(INCLUDES) -I../../BigNumber/src/BigNumber
override CFLAGS:=$(CFLAGS) -fPIC
objs:=../../number.o ../../BigNumber.o
rmobjs=rmobjs
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

all: VMPy.o $(rmobjs) $(objs) $(NAME).$(EXT)
.PHONY: all

rmobjs:
	$(RM) ../../number.o ../../BigNumber.o

%.o:
	$(MAKE) -C ../../ $(subst ../../,,$@) EXT_CFLAGS=-fPIC

VMPy.o: VMPy.cpp
	$(CXX) -c $(CFLAGS) $<

$(NAME).$(EXT):
	$(CXX) $(CFLAGS) $(objs) VMPy.o ../../VM.cpp $(LDFLAGS) -o $(NAME).$(EXT)

clean:
	$(RM) $(classes) *.o *.$(EXT)
