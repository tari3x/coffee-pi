
# ROOT=${shell git rev-parse --show-toplevel}
ROOT=..

MAIN = ${ROOT}/_build/default/src/main.exe

main.exe: FORCE
	rm -f $(MAIN)
	rm -f main.exe
	dune build main.exe
	cp $(MAIN) $@

FORCE:
