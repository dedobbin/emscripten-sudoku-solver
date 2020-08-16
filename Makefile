EMCC=emcc

all: main.c
	$(EMCC) main.c -o main.js -s WASM=1 -s ALLOW_TABLE_GROWTH=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap", "getValue", "setValue", "addFunction"]'		