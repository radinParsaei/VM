from ctypes import cdll

EXIT = 0
PUT = 1
POP = 2
ADD = 15
SUB = 16
MUL = 17
DIV = 18
MOD = 19
EQ = 20
FEQ = 21
GT = 22
GE = 23
LT = 24
LE = 25
LAND = 26
LOR = 27
AND = 28
OR = 29
NOT = 30
LNOT = 31
LSHIFT = 32
RSHIFT = 33
XOR = 34
NEG = 35
MEMSET = 50
MEMGET = 51
MEMSIZE = 52
MEMPUT = 53
MEMINS = 54
MEMDEL = 55
TONUM = 65
TOTXT = 66
ISNUM = 67
CANNUM = 68
REC = 75
END = 76
RUN = 77
REPEAT = 78
BREAK = 79
IFTRUN = 80
IFFRUN = 81
WTRUN = 82
WFRUN = 83
THREAD = 84
DLCALL = 100
PRINT = 101

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
            self.lib.runWithNull(opcode)
        elif type(data) == type(0):
            self.lib.runWithNumber(opcode, bytes(str(data), encoding="UTF-8"));
        elif type(data) == type(True):
            self.lib.runWithBoolean(opcode, data)
        else:
            self.lib.runWithString(opcode, bytes(str(data), encoding="UTF-8"));
