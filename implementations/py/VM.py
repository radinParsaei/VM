from ctypes import cdll

EXIT = 0
PUT = 1
ADD = 2
SUB = 3
MUL = 4
DIV = 5
MOD = 6
PRINT = 7
DLCALL = 8
REC = 9
END = 10
RUN = 11
POP = 12
LOGSTCK = 13
PRINTLN = 14
REPEAT = 15
EQ = 16
FEQ = 17
GT = 18
GE = 19
LT = 20
LE = 21
LAND = 22
LOR = 23
AND = 24
OR = 25
NOT = 26
LNOT = 27
LSHIFT = 28
RSHIFT = 29
XOR = 30
NEG = 31
BREAK = 32
WTRUN = 33
WFRUN = 34
IFTRUN = 35
IFFRUN = 36
THREAD = 37
MEMSET = 38
MEMGET = 39
MEMSIZE = 40
MEMPUT = 41
MEMINS = 42
MEMDEL = 43
TONUM = 44
TOTXT = 45
ISNUM = 46
CANNUM = 47

class VM:
    def __init__(self):
        try:
            self.lib = cdll.LoadLibrary("./libVMPy.dylib")
        except OSError:
            self.lib = cdll.LoadLibrary("./libVMPy.so")
        except OSError:
            self.lib = cdll.LoadLibrary("VMPy.dll")
        self.lib.init();

    def run(self, opcode, data = None):
        if data == None:
            self.lib.run(opcode)
        elif type(data) == type(0):
            self.lib.runWithNumber(opcode, bytes(str(data), encoding="UTF-8"));
        else:
            self.lib.runWithString(opcode, bytes(str(data), encoding="UTF-8"));
