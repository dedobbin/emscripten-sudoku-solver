EMCC=emcc

all: main.c
	$(EMCC) main.c -O2 -o main.js -s WASM=1 -s ALLOW_TABLE_GROWTH=1 -s EXTRA_EXPORTED_RUNTIME_METHODS='["cwrap", "getValue", "setValue", "addFunction"]'		

.PHONY: clean
clean:
	rm -f index.js main.wasm