classes=$(subst .java,, $(foreach java-source, $(wildcard *.java), $(java-source).class))
INCLUDES:=-I. -I../.. -I../../Value -I$(JAVA_HOME)/include -I$(JAVA_HOME)/include/darwin -I$(JAVA_HOME)/include/linux -I$(JAVA_HOME)/include/win32 -I/usr/lib/jvm/default-java/include -I/usr/lib/jvm/default-java/include/linux
override LDFLAGS:=$(LDFLAGS) -shared -fPIC

ifeq ($(USE_GMP_LIB),1)
override LDFLAGS:=$(LDFLAGS) -lgmp -lgmpxx
override CFLAGS:=$(CFLAGS) -DUSE_GMP_LIB
else
override INCLUDES:=$(INCLUDES) -I../../BigNumber/src/BigNumber
objs=../../number.o ../../BigNumber.o
rmobjs=rmobjs
endif

override CFLAGS:=$(CFLAGS) $(INCLUDES)

ifeq ($(OS),Windows_NT)
	EXT:=dll
	NAME:=VM
else
		NAME:=libVM
    UNAME := $(shell uname -s)
    ifeq ($(UNAME),Linux)
			EXT:=so
    endif
    ifeq ($(UNAME),Darwin)
			EXT=dylib
    endif
endif

all: $(rmobjs) $(objs) VM_JNI.o $(NAME).$(EXT) $(classes)
.PHONY: all

rmobjs:
	$(RM) $(objs)

%.o:
	$(MAKE) -C ../.. $(subst ../../,,$@) EXT_CFLAGS=-fPIC

%.class: %.java
	javac $<

VM_JNI.o: VM_JNI.cpp VM_JNI.h
	$(CXX) -c $(CFLAGS) VM_JNI.cpp -fPIC

$(NAME).$(EXT):
	$(CXX) $(CFLAGS) $(objs) VM_JNI.o ../../VM.cpp $(LDFLAGS) -o $(NAME).$(EXT)

clean:
	$(RM) $(classes) *.o $(NAME).$(EXT)
