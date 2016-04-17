.PHONY: clean all

.PRECIOUS: *.o

all: main.nes

clean:
	@rm -fv *.nes

reset.o: reset.s
	ca65 reset.s

nesasmhelpers.o: nesasmhelpers.s
	ca65 nesasmhelpers.s

%.o: %.c
	cc65 -Oi $< --add-source -t nes
	ca65 $*.s
	rm $*.s

%.nes: %.o reset.o nesasmhelpers.o
	ld65 -o $@ reset.o nesasmhelpers.o -C nes.cfg $< nes.lib
	rm nesasmhelpers.o reset.o
	mv main.nes ../emesh.nes
