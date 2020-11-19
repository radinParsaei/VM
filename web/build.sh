if [[ -z "$LIBS_FLAGS" ]]
then
	LIBS_FLAGS="-I../libs/test ../libs/test/test.cpp"
fi
emcc -c ../BigNumber/src/BigNumber/number.c
em++ -Os -std=c++14 -DSTD_INCLUDED -DUSE_READLINE -I../Value -I.. -I../libs -s WASM=1 --pre-js pre.js -DSTATIC_BUILD_MODE $LIBS_FLAGS -s MAIN_MODULE=1 -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" -I../BigNumber/src/BigNumber ../VM.cpp web.cpp number.o ../BigNumber/src/BigNumber/BigNumber.cpp -o VM.html
