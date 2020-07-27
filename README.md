# VM
a simple virtual machine is written in c++

to compile:
```bash
git clone https://github.com/radinParsaei/VM --recursive #clone this repository
cd VM
make
```
to build with gmplib:
```bash
make EXT_CFLAGS='-DUSE_GMP_LIB' EXT_LDFLAGS='-lgmp -lgmpxx'
```
to optimize generated VM use `-Os` and `-O3` in command-line arguments(EXT_CFLAGS)

you can build VM for esp8266 with esp8266 Arduino board version 2.3 and EARLIER with ArduinoSTL (see https://github.com/mike-matera/ArduinoSTL/pull/58) or without ArduinoSTL with any version of esp8266 or esp32
and for Arduino UNO, VM is tested with Arduino ide and platformio ide

use `make apple-universal2` to build universal binaries for macOS 11, Big Sur, with Xcode 12

use `emmake make wasm` to build repl and VM for wasm (VM need a VM binary file (default is out.bin) to add that file in wasm binary)

in windows have two ways:
1- use Cygwin
2- use MinGW + https://github.com/meganz/mingw-std-threads (run utility_scripts)


when VM is built output files are:
VM the VM main, repl a simple repl built with gnu readline if can, assembler, disassembler, and mkcc

mkcc (make c++ code from VM binary):
it converts VM binary files to c++ code
```
PUT TXTHello\n
PRINT
-------------------------
vm.run(PUT, "Hello\n");
vm.run(PRINT);
```

`cat VM.h | egrep '#define .+ [0-9]+' | cut -d' ' -f2` -> list all opcodes

# opcodes list in pesudo code
## EXIT:
close VM with exit status: pop()
## PUT:
put a value to VM stack
## POP:
pop one value from VM stack
## ADD:
`push(pop() + pop())`
## SUB:
`push(pop() - pop())`
## MUL:
`push(pop() * pop())`
## DIV:
`push(pop()/pop())`
## MOD:
`push(pop()%pop())`
## EQ:
`push(pop() == pop())` (`1 == "1"`), (`1 == True`)
## FEQ:
`push(pop() === pop())` (`1 !== "1"`) but (`1 === 1`), (`1 !== true`)
## GT:
`push(pop() > pop())`
## GE:
`push(pop() >= pop())`
## LT:
`push(pop() < pop())`
## LE:
`push(pop() <= pop())`
## LAND:
`push(pop() && pop())` (equals to `and` in python)
## LOR:
`push(pop() || pop())` (equals to `or` in python)
## AND:
`push(pop() & pop())` (bitwise)
## OR:
`push(pop() | pop())` (bitwise)
## NOT:
`push(~pop())` (bitwise)
## LNOT:
`push(!pop())` (equals to `not` in python)
## LSHIFT:
`push(pop() << pop())`
## RSHIFT:
`push(pop() >> pop())`
## XOR:
`push(pop() ^ pop())`
## NEG:
`if pop() is number: push(-pop()) else: push(reverse(pop()))`
## MEMSET:
`mem[pop()] = pop()`
## MEMGET:
`push(mem[pop()])`
## MEMSIZE:
`push(mem.size())`
## MEMPUT:
`mem.append(pop())`
## MEMINS:
`mem.insert(pop(), pop()) -> [0, 2]; insert 1, 1 -> [0, 1, 2]`
## MEMDEL:
`del mem[pop()]`
## TONUM:
`push(pop().toNumber())`
## TOTXT:
`push(pop().toString())`
## ISNUM:
`push(get().type == Number)`
## CANNUM:
`push(get().canNum())`
## TOBOOL:
`push(pop().toBool())`
## REC:
records programs
## END:
end recording
## RUN:
run recorded program
## REPEAT:
RUN * `pop()`
## BREAK:
break running recorded program
## IFTRUN:
`if (pop()) RUN`
## IFFRUN:
`if (!pop()) RUN`
## WTRUN:
```while(get()) {
  pop()
  RUN
}
```
## WFRUN:
```while(!get()) {
  pop()
  RUN
}
```
## THREAD:
run program on a real or proto thread
## DLCALL:
call libraries from files `*.vmso` or locally from "internalLibraryFunction"
PRINT:
`print(pop())`
