public class VM {
	static {
		System.loadLibrary("VM");
	}
	public VM() {
		init();
	}
	private native void runWithString(byte opcode, String data);
	private native void runWithDouble(byte opcode, double data);
	private native void init();
	public native void run(byte opcode);
	public void run(byte opcode, double data) {
		runWithDouble(opcode, data);
	}
	public void run(byte opcode, String data) {
		runWithString(opcode, data);
	}
	public static final byte EXIT = 0;
	public static final byte PUT = 1;
	public static final byte ADD = 2;
	public static final byte SUB = 3;
	public static final byte MUL = 4;
	public static final byte DIV = 5;
	public static final byte MOD = 6;
	public static final byte PRINT = 7;
	public static final byte DLCALL = 8;
	public static final byte REC = 9;
	public static final byte END = 10;
	public static final byte RUN = 11;
	public static final byte POP = 12;
	public static final byte LOGSTCK = 13;
	public static final byte PRINTLN = 14;
	public static final byte REPEAT = 15;
	public static final byte EQ = 16;
	public static final byte FEQ = 17;
	public static final byte GT = 18;
	public static final byte GE = 19;
	public static final byte LT = 20;
	public static final byte LE = 21;
	public static final byte LAND = 22;
	public static final byte LOR = 23;
	public static final byte AND = 24;
	public static final byte OR = 25;
	public static final byte NOT = 26;
	public static final byte LNOT = 27;
	public static final byte LSHIFT = 28;
	public static final byte RSHIFT = 29;
	public static final byte XOR = 30;
	public static final byte NEG = 31;
	public static final byte BREAK = 32;
	public static final byte WTRUN = 33;
	public static final byte WFRUN = 34;
	public static final byte IFTRUN = 35;
	public static final byte IFFRUN = 36;
	public static final byte THREAD = 37;
	public static final byte MEMSET = 38;
	public static final byte MEMGET = 39;
	public static final byte MEMSIZE = 40;
	public static final byte MEMPUT = 41;
	public static final byte MEMINS = 42;
	public static final byte MEMDEL = 43;
	public static final byte TONUM = 44;
	public static final byte TOTXT = 45;
	public static final byte ISNUM = 46;
	public static final byte CANNUM = 47;
}
